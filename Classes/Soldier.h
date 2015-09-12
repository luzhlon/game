#ifndef _Soldier_H_
#define _Soldier_H_

#include <string>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;

#define UPDATE_RATE 0.02f

class Soldier : public Sprite3D
{
public:
    enum State {
        SOLDIER_STATE_IDLE = 1 << 0,
        SOLDIER_STATE_MOVE = 1 << 1,
        SOLDIER_STATE_ACTION = 1 << 2,

        STATE_END
    };
    enum Type {
        SOLDIER_TYPE_WOMAN = 0,
        TYPE_END
    };

    enum { SoldierNumber = 3 };

    static const float s_full_blood;
    static Soldier *s_soldiers[SoldierNumber];
    static Soldier *s_followed;
public:
	CREATE_FUNC(Soldier);
    bool init() override;

    static bool loadAllSoldier(); //加载所有类型的Soldier

    void update(float dt) override; //帧刷新

    virtual bool init_soldier() { return true; } //子类重写

    void addThing(Node *);

    inline void addState(State state) { //给Soldier添加状态
        _state = (State)(_state | state);
    }
    inline bool atState(State s) { //判断Soldier是否处于某个状态
        return _state & s;
    }
    inline void rmState(State state) { //移除某个状态
        _state = (State)(_state & (~state));
    }

    inline void setAngle(Vec2 v) {
        _angle = v.getNormalized();
    }

    inline float getSpeed() {
        return _speed;
    }
    inline void setSpeed(float s) {
        _speed = s;
    }

    inline void action_move() {
        addState(SOLDIER_STATE_MOVE);
    }
    inline void action_move(Vec2 angle) {
        setAngle(angle);
        action_move();
    }

    inline void action_stop() {
        rmState(SOLDIER_STATE_MOVE);
    }

    inline Vec3 &getCameraOffset() {
        return _camera_offset;
    }
    inline float getHeightOffset() {
        return _height_offset;
    }
    inline void setCameraFollowed() {
        s_followed = this;
    }

    void show_blood_decline(float);
    void CameraZoom(float factor);
    void CameraRotate(Vec2 &v);
    void updatePosition(float dt = 0.f);
    void updateRotation();
    void updateBlood();

private:
    State _state = SOLDIER_STATE_IDLE;

protected:
    string _name = "<unset>";
    float _speed = 10.f;
    float _base_angle_z;
    float _blood = 80.f;
    Vec2  _angle;

    BillBoard *_billboard = nullptr;

    LoadingBar *_blood_bar;

    Action *_cur_action = nullptr;  //当前的动作
    PUParticleSystem3D *_cur_action_pu = nullptr;  //当前动作所使用的粒子系统

    Vec3 _camera_offset = Vec3(0.f, 120.f, 60.f);
    float _height_offset = -10.f;

    int m_id = 0;//ID是这个对象的唯一的标识
	int m_hp;//当前HP
	int m_hp_max;//HP最大值
	int m_sp;//技能点
};
#endif
