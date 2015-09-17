#ifndef _Soldier_H_
#define _Soldier_H_

#include <string>
#include "cocos2d.h"
#include "../Server/message.h"
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
        SOLDIER_TYPE_MAN,
        SOLDIER_TYPE_MAN2,

        TYPE_NUMBER
    };

    static const float s_full_blood;
    static const float s_step;
    static Soldier *s_soldiers[Type::TYPE_NUMBER];
    static Soldier *s_followed;
    static Vec3 s_camera_offset;

    static Soldier *create(int type_id);
    static Soldier *create(room_member *rm);
public:
	CREATE_FUNC(Soldier);
    bool init() override;

    void update(float dt) override; //帧刷新

    virtual bool init_soldier() { return true; } //子类重写

    inline void addState(State state) { //给Soldier添加状态
        _state = (State)(_state | state);
    }
    inline bool atState(State s) { //判断Soldier是否处于某个状态
        return _state & s;
    }
    inline void rmState(State state) { //移除某个状态
        _state = (State)(_state & (~state));
    }

    inline float speed() {
        return _speed;
    }
    inline void speed(float s) {
        _speed = s;
    }
    inline const string& name() {
        return _name;
    }
    inline const Vec3& target_point() {
        return _target_point;
    }
    inline void target_point(Vec3& v) {
        _target_point = v;
    }
    inline int role_id() {
        return (int)_role_id;
    }

    inline float getHeightOffset() {
        return _height_offset;
    }
    inline void setCameraFollowed() {
        s_followed = this;
    }

	inline void action_idle() {
		runAction(m_act_idle);
	}
	inline void action_walk() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_walk);
	}
	virtual void action_boxing() {
        addState(SOLDIER_STATE_ACTION);
        _cur_action = m_act_boxing;

		runAction(m_act_boxing);
	}
	inline void action_kick() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_kick);
	}
    inline void show_billboard(bool show) {
        _billboard->setVisible(show);
    }

    void move_stop(); //停止移动
    void move(Vec3& target); //移到到指定位置

    void net_move(Vec3& target);
    void net_move_stop();

    void begin_fight(); //准备开始战斗

    Vec3 get_head_pos(); //
    void show_blood_decline(float); //显示减血效果
    void CameraZoom(float factor); //拉近、拉远镜头
    void CameraRotate(Vec2 &v); //旋转Camera
    void updatePosition(float dt = 0.f); //刷新位置:根据目标点
    void updateHeight(); //刷新高度:根据所在X-Z面的位置
    void updateRotation(); //刷新朝向：根据目标点
    void updateBlood(); //刷新血量条
    void addThing(Node *); //往自身结点添加一个Node，根据自身的CameraMask
    void add2Billboard(Node *); //把节点添加到Billboard

private:
    State _state = SOLDIER_STATE_IDLE;

protected:
    string _name = "<unset>"; //头顶显示的名称
    float _speed = 10.f; //速度
    float _blood = 80.f; //血量
    Vec3  _target_point; //要移动到的目标点

    BillBoard *_billboard = nullptr;

    LoadingBar *_blood_bar;

    Action *_cur_action = nullptr;  //当前的动作
    PUParticleSystem3D *_cur_action_pu = nullptr;  //当前动作所使用的粒子系统

    float _height_offset;

    Type _role_id = (Type)(-1); //所属角色的ID
    room_member *_room_member = nullptr; // room_member 信息

    //五个基本动作
	ActionInterval *m_act_idle;
	ActionInterval *m_act_walk;
	ActionInterval *m_act_special;
	ActionInterval *m_act_boxing;
	ActionInterval *m_act_kick;
};

#endif
