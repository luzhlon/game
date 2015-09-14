#include "World.h"
#include "DrawNode3D.h"
#include "AppDelegate.h"
#include <string>

using namespace std;

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

World::World() {
    {
        Terrain::DetailMap r("TerrainTest/dirt.jpg"), g("TerrainTest/Grass2.jpg", 10), b("TerrainTest/road.jpg"), a("TerrainTest/GreenSkin.jpg", 20);

        Terrain::TerrainData data("TerrainTest/heightmap16.jpg", "TerrainTest/alphamap.png", r, g, b, a, Size(32, 32), 40.0f, 2);
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
        _camera = Camera::createPerspective(60, AppDelegate::width() / AppDelegate::height(), 0.1f, 800.f);
        _camera->setCameraFlag((CameraFlag)CAMERA_I);
        addChild(_camera);
    }

    {
        _pu_clickPoint = PUParticleSystem3D::create("texiao_point.pu");
        addChild(_pu_clickPoint);
    }

    drawGrid();
    addSkybox();

    setCameraMask((unsigned short)CAMERA_I);
}

extern Director *g_director;

bool World::conv2space(Vec3& v) {
    Vec3 nearP(v), farP(v);
    nearP.z = 0.f; farP.z = 1.f;

    auto size = g_director->getWinSize();
    _camera->unproject(size, &nearP, &nearP);
    _camera->unproject(size, &farP, &farP);
    Vec3 dir = farP - nearP;
    dir.normalize();

    Vec3 collisionPoint(-999,-999,-999);
    bool ret = _terrain->getIntersectionPoint(Ray(nearP, dir), collisionPoint);
    v = collisionPoint;
    return ret;
}
/*
void World::unproject(const Size* viewport, Vec3* src, Vec3* dst)
{
    assert(dst);
    
    assert(viewport->width != 0.0f && viewport->height != 0.0f);
    Vec4 screen(src->x / viewport->width, ((viewport->height - src->y)) / viewport->height, src->z, 1.0f);

    auto viewProjection = g_director->getMatrix(MATRIX_STACK_TYPE::MATRIX_STACK_PROJECTION);
    
    screen.x = screen.x * 2.0f - 1.0f;
    screen.y = screen.y * 2.0f - 1.0f;
    screen.z = screen.z * 2.0f - 1.0f;
    
    viewProjection.getInversed().transformVector(screen, &screen);
    
    if (screen.w != 0.0f)
    {
        screen.x /= screen.w;
        screen.y /= screen.w;
        screen.z /= screen.w;
    }
    
    dst->set(screen.x, screen.y, screen.z);
} // */

void World::drawGrid(float cell) {
    _drawNode = DrawNode3D::create();

    int count = 20;
    float distance = count * cell;
    //draw x
    for (int j = -20; j <= 20; j++)
    {
        _drawNode->drawLine(Vec3(-distance, 0, cell * j), Vec3(distance, 0, cell * j), Color4F(1, 0, 0, 1));
    }
    //draw z
    for (int j = -20; j <= 20; j++)
    {
        _drawNode->drawLine(Vec3(cell * j, 0, -distance), Vec3(cell * j, 0, distance), Color4F(0, 0, 1, 1));
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

void World::addThing(Node *node, float x, float z) {
    addChild(node);
    node->setPositionY(getHeight(x, z));
    node->setCameraMask(CAMERA_I);
}

void World::showPoint(const Vec3& v) {
    _pu_clickPoint->setPosition3D(v);
    _pu_clickPoint->startParticleSystem();
}

void World::addSkybox() {
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
