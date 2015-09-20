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
        CAMERA_FREE = (int)CameraFlag::USER1,  // 自由视角摄像机
        CAMERA_FIX = (int)CameraFlag::USER2   // 固定视角摄像机
    };

    static Vec3 s_camera_offset;
	static World *getInstance();

    inline Terrain *terrain() {
        return _terrain;
    }
    inline float height(float x, float z) {
        return _terrain->getHeight(x, z);
    }
    inline DrawNode3D *draw_node() {
        return _drawNode;
    }

    void draw_grid(float cell = 10.f, float height = 0.f);
    void add_skybox();

    void camera_zoom(float factor);
    void camera_move(Vec2& factor);
    void camera_follow(Node *node);

    bool load_goods(char *file);

    void add_thing(Node *node, float x = 0.f, float z = 0.f);

    void show_click(const Vec3& v);
    bool conv2space(Vec3& v); //ignore v.z
    Camera *camera();

private:
	World();

    Terrain *_terrain;
    Camera *_camera_free;
    Camera *_camera_fix;

    DrawNode3D *_drawNode;
    PUParticleSystem3D *_pu_click_point;
};

#endif /* __WORLD_H__ */
