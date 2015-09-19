#include "World.h"
#include "DrawNode3D.h"
#include <string>

using namespace std;

Vec3 World::s_camera_offset = Vec3(0.f, 60.f, 45.f); 

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

extern Size g_win_size;

World::World() {
    {
        Terrain::DetailMap r("TerrainTest/dirt.jpg"), g("TerrainTest/Grass2.jpg", 10), b("TerrainTest/road.jpg"), a("TerrainTest/GreenSkin.jpg", 20);

        Terrain::TerrainData data("TerrainTest/ground.jpg", "TerrainTest/alphamap.png", r, g, b, a, Size(32, 32), 330.0f, 6.f);
        _terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
        _terrain->setMaxDetailMapAmount(4);
        _terrain->setDrawWire(false);
        _terrain->setSkirtHeightRatio(3);
        _terrain->setLODDistance(64, 128, 192);
        addChild(_terrain);
        _LogSize("[World]:Terrain size: ", _terrain->getContentSize());
        _LogVec3("[World]:Terrain center: ", _terrain->getAABB().getCenter());
    }

    {
        _camera = Camera::createPerspective(60, g_win_size.width / g_win_size.height, 0.1f, 1000.f);
        _camera->setCameraFlag((CameraFlag)CAMERA_I);
        addChild(_camera);

        _camera_fix = Camera::createPerspective(60, g_win_size.width / g_win_size.height, 0.1f, 1000.f);
        _camera_fix->setCameraFlag((CameraFlag)CAMERA_FIX);
        _camera_fix->setPosition3D(Vec3(0, 120, 90));
        _camera_fix->lookAt(Vec3::ZERO);
        addChild(_camera_fix);
    }

    {
        _pu_click_point = PUParticleSystem3D::create("texiao_point.pu");
        addChild(_pu_click_point);
    }

    draw_grid(10.f, _terrain->getHeight(Vec2::ZERO));
    add_skybox();

    setCameraMask((unsigned short)CAMERA_FIX);
}

extern Director *g_director;

bool World::conv2space(Vec3& v) {
    Vec3 nearP(v), farP(v);
    nearP.z = 0.f; farP.z = 1.f;

    auto camera = get_camera();

    auto size = g_director->getWinSize();
    camera->unproject(size, &nearP, &nearP);
    camera->unproject(size, &farP, &farP);
    Vec3 dir = farP - nearP;
    dir.normalize();

    Vec3 collisionPoint(-999,-999,-999);
    bool ret = _terrain->getIntersectionPoint(Ray(nearP, dir), collisionPoint);
    v = collisionPoint;
    return ret;
}

void World::draw_grid(float cell, float height) {
    _drawNode = DrawNode3D::create();

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
    //_drawNode->setGlobalZOrder(-1.f);
    addChild(_drawNode);
}

World *World::getInstance() {
    static World *world = nullptr;
    if (!world) {
        world = new World();
    }
    return world;
}

void World::add_thing(Node *node, float x, float z) {
    addChild(node);
    node->setPositionX(x);
    node->setPositionY(getHeight(x, z));
    node->setPositionZ(z);
    node->setCameraMask(getCameraMask());
}

void World::show_point(const Vec3& v) {
    _pu_click_point->setPosition3D(v);
    _pu_click_point->startParticleSystem();
}

Camera *World::get_camera() {
    switch (getCameraMask()) {
    case CAMERA_I:
        return _camera;
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
    auto _textureCube = TextureCube::create("Sprite3DTest/skybox/left.jpg", "Sprite3DTest/skybox/right.jpg",
                                       "Sprite3DTest/skybox/top.jpg", "Sprite3DTest/skybox/bottom.jpg",
                                       "Sprite3DTest/skybox/front.jpg", "Sprite3DTest/skybox/back.jpg");
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
    //_skyBox->setCameraMask(s_CM[LAYER_SKYBOX]);
    _skyBox->setCameraMask(CAMERA_I);
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
    case CAMERA_I:
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
        auto cp = _camera_fix->getPosition3D();
        cp.x -= factor.x * 0.1;
        cp.z += factor.y * 0.1;
        _camera_fix->setPosition3D(cp);
        break;
    }
    case CAMERA_I:
    {
        auto v = factor;
        //Rotate X-Z coordinate
        if (fabs(v.x) > fabs(v.y)) {
            Vec2 v_xz(s_camera_offset.x, s_camera_offset.z);
            v_xz.rotate(Vec2::ZERO, v.x * 0.02);
            //
            s_camera_offset.x = v_xz.x;
            s_camera_offset.z = v_xz.y;
        }
        else {
            //Rotate Y-Z coordinate
            Vec2 v_yz(s_camera_offset.y, s_camera_offset.z);
            v_yz.rotate(Vec2::ZERO, v.y * 0.02);
            //
            s_camera_offset.y = v_yz.x;
            s_camera_offset.z = v_yz.y;
        }
    }
    }
}

void World::camera_follow(Node *node) {
    if (getCameraMask() == CAMERA_I) {
        _camera->setPosition3D(node->getPosition3D() + s_camera_offset);
        _camera->lookAt(node->getPosition3D());
    }
}
