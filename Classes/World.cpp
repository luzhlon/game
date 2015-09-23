#include "World.h"
#include "DrawNode3D.h"
#include <string>

using namespace std;

Vec3 World::s_camera_offset = Vec3(0.f, 130.f, 100.f); 

extern Director *g_director;
extern Size g_win_size;
extern World *g_world;

Vec2 g_points[20] = {
    Vec2(591, -44),
    Vec2(-486, 565),
    Vec2(-469, 234),
    Vec2(-639, 280),
    Vec2(-178, 547),
    Vec2(329, 619),
    Vec2(234, 247),
    Vec2(-133, 36),
    Vec2(-45, 126),
    Vec2(289, 70),
    Vec2(444, 137),
    Vec2(591, 215),
    Vec2(561, 80),
    Vec2(531, 540),
    Vec2(653, 483),
    Vec2(391, -312),
    Vec2(-424, -418),
    Vec2(-42, -477),
    Vec2(-177, -118),
    Vec2(114, -228)
};

void _LogSize(const char *desc, const Size& size) {
    string str = desc;
    str += "%g, %g";
    log(str.c_str(), size.width, size.height);
}

void _LogVec3(const char *desc, Vec3& v3) {
    string str = desc;
    str += "%g, %g, %g";
    log(str.c_str(), v3.x, v3.y, v3.z);
}

QuatNode::QuatNode() {
    _child[0][0] = nullptr; // dd 
    _child[0][1] = nullptr; // du
    _child[1][0] = nullptr; // ud
    _child[1][1] = nullptr; // uu

#ifdef VIEW_TEST
    _draw_node = DrawNode3D::create();
    g_world->add_thing(_draw_node);
#endif
}

QuatNode::QuatNode(QuatNode *parent, Vec2& d, Vec2& u) {
    _child[0][0] = nullptr; // dd 
    _child[0][1] = nullptr; // du
    _child[1][0] = nullptr; // ud
    _child[1][1] = nullptr; // uu

    Parent = parent;
    dd.x = MIN(d.x, u.x);
    dd.y = MIN(d.y, u.y);
    uu.x = MAX(d.x, u.x);
    uu.y = MAX(d.y, u.y);

#ifdef VIEW_TEST
    _draw_node = DrawNode3D::create();
    g_world->add_thing(_draw_node);
#endif
}

QuatNode::~QuatNode() {
#ifdef VIEW_TEST
    _draw_node->removeFromParent();
#endif
    // 移除子节点
    for (int i = 0; i < 4; i++) {
        auto ch = _child[i / 2][i % 2];
        if (ch) delete ch;
    }
    if (!Parent) return;
    // 从父节点移除自己
}

bool QuatNode::Split(Vec2& sp) {
    if (!contained(sp)) return false;
    if (split()) unSplit();

    Vec2 v2;

    _child[0][0] = new QuatNode(this, dd, sp); // dd
    v2.x = dd.x;
    v2.y = uu.y;
    _child[0][1] = new QuatNode(this, v2, sp); // du
    v2.x = uu.x;
    v2.y = dd.y;
    _child[1][0] = new QuatNode(this, v2, sp); // ud
    _child[1][1] = new QuatNode(this, uu, sp); // uu

    _split = sp;

    return true;
}

void QuatNode::unSplit() {
    if (!split()) return;

    for (int i = 0; i < 4; i++) {
        int ix = i / 2;
        int iy = i % 2;
        auto ch = _child[ix][iy];
        if (ch) {
            delete ch;
            _child[ix][iy] = nullptr;
        }
    }

#ifdef VIEW_TEST
    Clear();
#endif
}

bool QuatNode::Export(char *file) {
    FILE *f = fopen(file, "w+");
    if (!f) return false;

    Export(f);

    fclose(f);
    return true;
}

void QuatNode::Export(FILE *f) {
    fprintf(f, "{\n");
    fprintf(f, "dduu (%g,%g) (%g,%g)\n", dd.x, dd.y, uu.x, uu.y);

    if (block()) {
        fprintf(f, "block %d\n", block());
    }
    if (split()) {
        fprintf(f, "split (%g,%g)\n", _split.x, _split.y);

        for (int i = 0; i < 4; i++) {
            auto child = _child[i / 2][i % 2];
            if (child) child->Export(f);
        }
    }

    fprintf(f, "}\n");
}

int QuatNode::Parse(const char *str) {
    Vec2 DD, UU, split;
    // 加载
    int block;
    const char *p = str;

    for (; *p; p++) 
        if (*p == '\n') break;
    p++;

    if (*p == '}') return p - str + 1;

    if (4 == sscanf(p, "dduu (%g,%g) (%g,%g)", &DD.x, &DD.y, &UU.x, &UU.y)) {
        dd = DD;
        uu = UU;
#ifdef VIEW_TEST
        DrawOutline();
#endif
    }

    for (; *p; p++) 
        if (*p == '\n') break;
    p++;

    if (*p == '}') return p - str + 1;

    if (1 == sscanf(p, "block %d", &block)) {
        _block = block;
#ifdef VIEW_TEST
        DrawBlock();
#endif
    } else if (2 == sscanf(p, "split (%g,%g)", &split.x, &split.y)) {
        Split(split);
#ifdef VIEW_TEST
        DrawSplit();
#endif
    }

    for (; *p; p++) 
        if (*p == '\n') break;
    p++;

    if (*p == '}') return p - str + 1;

    if (*p == '{') p += _child[0][0]->Parse(p);
    for (; *p; p++) 
        if (*p == '\n') break;
    p++;
    if (*p == '{') p += _child[0][1]->Parse(p);
    for (; *p; p++) 
        if (*p == '\n') break;
    p++;
    if (*p == '{') p += _child[1][0]->Parse(p);
    for (; *p; p++) 
        if (*p == '\n') break;
    p++;
    if (*p == '{') p += _child[1][1]->Parse(p);

    for (; *p; p++) 
        if (*p == '\n') break;
    p++;

    return p - str + 1;
}

QuatNode *QuatNode::Import(char *file) {
    //
    auto str = FileUtils::getInstance()->getStringFromFile(file);
    auto quat = new QuatNode();
    quat->Parse(str.c_str());
    return quat;
}

QuatNode *QuatNode::getChild(Vec2& pos) {     // 递归获得
    if (auto ret = getChildOnce(pos))
        return ret->getChild(pos);
    else
        return this;
}

QuatNode *QuatNode::getChildOnce(Vec2& pos) { // 不递归
    int ix = pos.x > _split.x ? 1 : 0; // x -> d : u
    int iy = pos.y > _split.y ? 1 : 0; // y -> d : u
    return _child[ix][iy];
}

bool QuatNode::contained(Vec2& pos) {
    if (pos.x < uu.x && pos.x > dd.x
        && pos.y < uu.y && pos.y > dd.y)
        return true;
    return false;
}

#ifdef VIEW_TEST
Color4F QuatNode::s_color = Color4F(1, 1, 0, 0);

void QuatNode::DrawBlock(float height) {
    for (float x = dd.x; x < uu.x; x += 3.f) {
        _draw_node->drawLine(Vec3(x, height, dd.y), Vec3(x, height, uu.y), s_color);
    }
}
void QuatNode::DrawSplit(float height) {
    if (_split.isZero()) return;
    _draw_node->drawLine(Vec3(_split.x, height, dd.y), Vec3(_split.x, height, uu.y), s_color);
    _draw_node->drawLine(Vec3(dd.x, height, _split.y), Vec3(uu.x, height, _split.y), s_color);
}
void QuatNode::DrawOutline(float height) {
    _draw_node->drawLine(Vec3(dd.x, height, dd.y), Vec3(uu.x, height, dd.y), s_color);
    _draw_node->drawLine(Vec3(dd.x, height, dd.y), Vec3(dd.x, height, uu.y), s_color);
    _draw_node->drawLine(Vec3(uu.x, height, uu.y), Vec3(dd.x, height, uu.y), s_color);
    _draw_node->drawLine(Vec3(uu.x, height, uu.y), Vec3(uu.x, height, dd.y), s_color);
}
void QuatNode::DrawHeight(DrawNode3D *draw) {
    float height = 300;
    auto color = Color4F(0, 0, 1, 0);
    if (block()) color = Color4F(1, 0, 0, 0);
    for (float i = dd.x; i < uu.x; i += 3) {
        draw->drawLine(Vec3(i, -300, dd.y), Vec3(i, height, dd.y), color);
    }
    for (float i = dd.x; i < uu.x; i += 3) {
        draw->drawLine(Vec3(i, -300, uu.y), Vec3(i, height, uu.y), color);
    }
    for (float i = dd.y; i < uu.y; i += 3) {
        draw->drawLine(Vec3(dd.x, -300, i), Vec3(dd.x, height, i), color);
    }
    for (float i = dd.y; i < uu.y; i += 3) {
        draw->drawLine(Vec3(uu.x, -300, i), Vec3(uu.x, height, i), color);
    }
}
#endif

World::World() {
    g_world = this;
    {
        _drawNode = DrawNode3D::create();
        addChild(_drawNode);
    }
    {
        Terrain::DetailMap r("TerrainTest/water.jpg"), g("TerrainTest/grass.jpg", 10), b("TerrainTest/road.jpg"), a("TerrainTest/greenskin.jpg", 20);

        Terrain::TerrainData data("TerrainTest/ground.jpg", "TerrainTest/alphamap.jpg", r, g, b, a, Size(32, 32), 330.0f, 6.f);
        _terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
        _terrain->setMaxDetailMapAmount(4);
        _terrain->setDrawWire(false);
        _terrain->setSkirtHeightRatio(3);
        _terrain->setLODDistance(64, 128, 192);
        addChild(_terrain);
        //_LogSize("[World]:Terrain size: ", _terrain->getContentSize());
        //_LogVec3("[World]:Terrain center: ", _terrain->getAABB().getCenter());
    }

    {
        _camera_free = Camera::createPerspective(60, g_win_size.width / g_win_size.height, 0.1f, 2000.f);
        _camera_free->setCameraFlag((CameraFlag)CAMERA_FREE);
        _camera_free->setPosition3D(s_camera_offset);
        addChild(_camera_free);

        _camera_fix = Camera::createPerspective(60, g_win_size.width / g_win_size.height, 0.1f, 2000.f);
        _camera_fix->setCameraFlag((CameraFlag)CAMERA_FIX);
        _camera_fix->setPosition3D(Vec3(0, 800, 0));
        _camera_fix->lookAt(Vec3::ZERO, Vec3(0,0,-1));
        addChild(_camera_fix);
    }

    {
        _pu_click_point = PUParticleSystem3D::create("texiao_point.pu");
        addChild(_pu_click_point);
    }

    draw_grid(10.f, _terrain->getHeight(Vec2::ZERO));
    add_skybox();

    load_goods("goods/plant/config");
    load_goods("goods/plant/config2");

    load_collision("root.txt");

    setCameraMask((unsigned short)CAMERA_FREE);

    for (int i = 0; i < 20; i++) _goods[i] = 0;
    schedule(schedule_selector(World::update_goods), 3.f); // 每隔3秒检测并生成一次goods
}

void World::load_collision(char *file) {
    _colli_root = QuatNode::Import(file);
    CC_ASSERT(_colli_root);
}

bool World::conv2space(Vec3& v) {
    Vec3 nearP(v), farP(v);
    nearP.z = 0.f; farP.z = 1.f;

    auto cam = camera();

    auto size = g_director->getWinSize();
    cam->unproject(size, &nearP, &nearP);
    cam->unproject(size, &farP, &farP);
    Vec3 dir = farP - nearP;
    dir.normalize();

    Vec3 collisionPoint(-999,-999,-999);
    bool ret = _terrain->getIntersectionPoint(Ray(nearP, dir), collisionPoint);
    v = collisionPoint;
    return ret;
}

void World::draw_grid(float cell, float height) {
    int count = 20;
    float distance = count * cell;
    //draw x
    for (int j = -20; j <= 20; j++)
    {
        _drawNode->drawLine(Vec3(-distance, height, cell * j), Vec3(distance, height, cell * j), Color4F(1, 0, 0, 1));
    }
    //draw z
    for (int j = -20; j <= 20; j++)
    {
        _drawNode->drawLine(Vec3(cell * j, height, -distance), Vec3(cell * j, height, distance), Color4F(0, 0, 1, 1));
    }
    //draw y
    _drawNode->drawLine(Vec3(0, -distance, 0), Vec3(0, distance, 0), Color4F(0, 1, 0, 1));
    _drawNode->setGlobalZOrder(_terrain->getGlobalZOrder() + 1);
}

bool World::is_collision(Vec2& pos) {
    static QuatNode *last = _colli_root;

    auto col = last->getChild(pos);
    // 找到包含Pos的区域
    while (!col->contained(pos)) {
        col = col->Parent;
        // 不在根结点表示的范围内
        if (!col) return true; 
    }

    last = col->getChild(pos);
    return last->block();
}

World *World::getInstance() {
    static World *world = nullptr;
    if (!world) {
        world = new World();
    }
    return world;
}

bool World::load_goods(char *file) {
    auto str = FileUtils::getInstance()->getStringFromFile(file);
    if (str.empty()) return false;

    String content(str);
    Array *lines = content.componentsSeparatedByString("\n");

    const char *format = "{ Vec3(%g, %g, %g), Vec3(%g, %g, %g), %g },";
    const char *p = str.c_str();
    static string cur_file;
    Vec3 pos3, rota;
    float scale;

    Ref *obj;
    CCARRAY_FOREACH(lines, obj) {
        String *line = (String *)obj;

        if (7 == sscanf(line->getCString(), format, &pos3.x, &pos3.y, &pos3.z, &rota.x, &rota.y, &rota.z, &scale)) {
            auto spr = Sprite3D::create(cur_file);
            spr->setRotation3D(rota);
            spr->setPosition3D(pos3);
            spr->setScale(scale);
            add_thing(spr, pos3.x, pos3.z);
        }
        else{
            cur_file = line->_string;
            cur_file += ".c3b";
            cur_file = "goods/plant/" + cur_file;
        }
    }

    return true;
}

void World::add_thing(Node *node, float x, float z) {
    addChild(node);
    node->setPositionX(x);
    node->setPositionY(height(x, z));
    node->setPositionZ(z);
    node->setCameraMask(getCameraMask());
}

void World::show_click(const Vec3& v) {
    _pu_click_point->setPosition3D(v);
    _pu_click_point->startParticleSystem();
}

Camera *World::camera() {
    switch (getCameraMask()) {
    case CAMERA_FREE:
        return _camera_free;
        break;
    case CAMERA_FIX:
        return _camera_fix;
        break;
    default:
        return nullptr;
    }
}

void World::add_skybox() {
    // create the second texture for cylinder
    auto _textureCube = TextureCube::create("skybox/left.jpg", "skybox/right.jpg",
                                       "skybox/top.jpg", "skybox/bottom.jpg",
                                       "skybox/front.jpg", "skybox/back.jpg");
    _textureCube->retain();
    //set texture parameters
    Texture2D::TexParams tRepeatParams;
    tRepeatParams.magFilter = GL_LINEAR;
    tRepeatParams.minFilter = GL_LINEAR;
    tRepeatParams.wrapS = GL_MIRRORED_REPEAT;
    tRepeatParams.wrapT = GL_MIRRORED_REPEAT;
    _textureCube->setTexParameters(tRepeatParams);
    
    // pass the texture sampler to our custom shader
    //state->setUniformTexture("u_cubeTex", _textureCube);
    
    // add skybox
    auto _skyBox = Skybox::create();
    _skyBox->retain();
    _skyBox->setCameraMask(CAMERA_FREE);
    _skyBox->setTexture(_textureCube);
    _skyBox->setScale(700.f);
    _skyBox->setGlobalZOrder(-1);
    this->addChild(_skyBox);
}

void World::camera_zoom(float factor) {
    switch (getCameraMask()) {
    case CAMERA_FIX:
        _camera_fix->setPositionY(_camera_fix->getPositionY() + factor);
        break;
    case CAMERA_FREE:
    {
         auto v = s_camera_offset;
         v.normalize();
         s_camera_offset += v * factor;
         break;
    }
    }
}

void World::camera_move(Vec2& factor) {
    switch (getCameraMask()) {
    case CAMERA_FIX:
    {
        auto pos = _camera_fix->getPosition3D();
        auto h = fabs(pos.y);
        pos.x -= factor.x * h / 1000.f;
        pos.z += factor.y * h / 1000.f;
        _camera_fix->setPosition3D(pos);
        pos.y = 0;
        _camera_fix->lookAt(pos, Vec3(0, 0, -1));
        break;
    }
    case CAMERA_FREE:
    {
        auto v = factor * 0.1f;
        //Rotate X-Z coordinate
        if (fabs(v.x) > fabs(v.y)) {
            Vec2 v_xz(s_camera_offset.x, s_camera_offset.z);
            v_xz.rotate(Vec2::ZERO, v.x * 0.02);
            //
            s_camera_offset.x = v_xz.x;
            s_camera_offset.z = v_xz.y;
        }
        else {
            /* //Rotate Y-Z coordinate
            Vec2 v_yz(s_camera_offset.y, s_camera_offset.z);
            v_yz.rotate(Vec2::ZERO, v.y * 0.02);
            //
            s_camera_offset.y = v_yz.x;
            s_camera_offset.z = v_yz.y; // */
            auto height = s_camera_offset.y + factor.y * 0.1;
            if (height < 80.f) break;
            s_camera_offset.y = height;
        }
        break;
    }
    }
}

void World::camera_follow(Node *node) {
    if (getCameraMask() == CAMERA_FREE) {
        _camera_free->setPosition3D(node->getPosition3D() + s_camera_offset);
        auto pos = node->getPosition3D();
        auto delta3 = _camera_free->getPosition3D() - pos;
        Vec2 delta(delta3.x, delta3.z);
        delta.rotate(Vec2::ZERO, CC_DEGREES_TO_RADIANS(180.f));
        pos.x += delta.x;
        pos.z += delta.y;
        _camera_free->lookAt(pos);
    }
}

void World::set_position(Node *node, Vec2& pos) {
    Vec3 p3;
    p3.x = pos.x;
    p3.z = pos.y;
    p3.y = _terrain->getHeight(pos);
    node->setPosition3D(p3);
}

GoodsGrass::GoodsGrass()
    : Goods(GRASS) {
    _sprite = Sprite3D::create("goods/plant/caoyuanche.c3b");
    _sprite->setScale(0.15f);
}

GoodsGrass::~GoodsGrass() {
    _sprite->removeFromParent();
}

float g_random(float scale = 1.f) {
    timeval psv;
    gettimeofday(&psv, 0);
    unsigned int tsrans = psv.tv_sec * 1000 + psv.tv_usec / 1000;
    srand(tsrans);
    return CCRANDOM_0_1() * scale;
}

void World::update_goods(float dt) {
    if (goods_count() > 7) return;
    int index;
    do {
        index = g_random(20);
    } while (_goods[index]);

    Goods good;
    good.type = (Goods::Type)(int)g_random(Goods::TYPE_NUMBER);
    good.count = g_random(20);
    good.index = index;

    if (_on_gen_goods) _on_gen_goods(&good);
}

int World::goods_count() {
    int count = 0;
    for (int i = 0; i < 20; i++)
    if (_goods[i]) count++;
    return count;
}

void World::dec_goods(Goods* good) {
    auto gd = _goods[good->index];
    if (gd) delete gd;
    _goods[good->index] = nullptr;
}

void World::add_goods(Goods* good) {
    Goods *gd;
    switch (good->type) {
    case Goods::GRASS:
        gd = new GoodsGrass();
        gd->count = good->count;
        add_thing(((GoodsGrass *)gd)->_sprite, g_points[good->index]);
        break;
    }
    if (_goods[good->index]) delete _goods[good->index];
    _goods[good->index] = gd;
}
//
bool World::get_goods(Vec2& pos, Goods *good) {
    for (auto gd : _goods) {
        if (!gd) continue;

        auto delta = g_points[gd->index] - pos;
        if (delta.length() < 20) {
            memcpy(gd, good, sizeof(Goods));
            return true;
        }
    }
    return false;
}
