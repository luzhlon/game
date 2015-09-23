#ifndef __WORLD_H__
#define __WORLD_H__

#include <functional>
#include "cocos2d.h"
#include "DrawNode3D.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;

//#define VIEW_TEST

class QuatNode {
public:
    static QuatNode *Import(char *file);

    QuatNode();
    QuatNode(QuatNode *parent, Vec2& d, Vec2& u);
    bool Split(Vec2& sp); // 
    void unSplit();
    ~QuatNode();

    int Parse(const char *p);
    bool Export(char *file);
    void Export(FILE *f);

    QuatNode *getChild(Vec2& pos);    // 递归获得
    QuatNode *getChildOnce(Vec2& pos); // 不递归
    bool contained(Vec2& pos);
    inline void block(bool b) {
        _block = b;
    }
    inline bool block() {
        return _block;
    }
    inline bool split() { // splited ?
        return !_split.isZero();
    }

    bool _block = false; // is block area
 
    Vec2 dd, uu; /* u: up, d: down */
    Vec2 _split = Vec2::ZERO; /* point which splited the area */
    QuatNode *Parent = nullptr; // 
    QuatNode *_child[2][2];  // x,y

#ifdef VIEW_TEST
    void DrawBlock(float height = 0);
    void DrawSplit(float height = 0);
    void DrawOutline(float height = 0);
    void DrawHeight(DrawNode3D *draw);

    inline void Clear() {
        _draw_node->clear();
    }

    static Color4F s_color;
    DrawNode3D *_draw_node = nullptr;
#endif
};

struct Goods {
    enum Type {
        GRASS = 0,

        TYPE_NUMBER
    };
    Goods(Type t) { type = t; }
    Goods() {};

    virtual Vec2 get_position() { return Vec2::ZERO; }

    Type type;
    int  count;
    int  index;
};

struct GoodsGrass : public Goods {
    GoodsGrass();
    ~GoodsGrass();

    Vec2 get_position() override {
        auto p3 = _sprite->getPosition3D();
        return Vec2(p3.x, p3.z);
    }

    Sprite3D *_sprite = nullptr;
};

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
    void load_collision(char *file);

    void set_position(Node*, Vec2& pos);
    bool is_collision(Vec2& pos);
    void add_thing(Node *node, float x = 0.f, float z = 0.f);

    inline bool is_collision(Vec3& pos) {
        return is_collision(Vec2(pos.x, pos.z));
    }
    inline void add_thing(Node *node, Vec2& pos) {
        return add_thing(node, pos.x, pos.y);
    }
    inline void add_thing(Node *node, Vec3& pos) {
        return add_thing(node, pos.x, pos.z);
    }
    inline void on_gen_goods(std::function<void(Goods*)> cb) {
        _on_gen_goods = cb;
    }

    void update_goods(float dt); // 更新物品

    int goods_count();
    void dec_goods(Goods *good); // 
    void add_goods(Goods *good); 

    bool get_goods(Vec2& pos, Goods *);

    void show_click(const Vec3& v);
    bool conv2space(Vec3& v); //ignore v.z
    Camera *camera();

private:
	World();

    Terrain *_terrain;
    Camera *_camera_free;
    Camera *_camera_fix;
    QuatNode *_colli_root; // 根碰撞区域

    DrawNode3D *_drawNode;
    PUParticleSystem3D *_pu_click_point;

    Goods *_goods[20];
    std::function<void(Goods *)> _on_gen_goods  = nullptr;


#endif /* __WORLD_H__ */
