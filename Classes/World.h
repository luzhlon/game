#ifndef __WORLD_H__
#define __WORLD_H__

#include "cocos2d.h"
#include "DrawNode3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;

class World : public Node
{
public:
    enum {
        CAMERA_I = CameraFlag::USER1
    };
	static World *getInstance();

    inline Terrain *getTerrain() {
        return _terrain;
    }
    inline Camera *getCamera() {
        return _camera;
    }
    inline DrawNode3D *getDrawNode() {
        return _drawNode;
    }

    void drawGrid(float cell = 10.f);
    void addSkybox();

    void addThing(Node *node, float x = 0.f, float z = 0.f);

    void showPoint(const Vec3& v);
    bool conv2space(Vec3& v); //ignore v.z
    //void unproject(const Size* viewport, Vec3* src, Vec3* dst);

    inline float getHeight(float x, float z) {
        return _terrain->getHeight(x, z);
    }
private:
	World();

    Terrain *_terrain;
    Camera *_camera;
    DrawNode3D *_drawNode;
    PUParticleSystem3D *_pu_clickPoint;
};

#endif /* __WORLD_H__ */