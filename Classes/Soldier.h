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
    static const float s_step;
    static Soldier *s_soldiers[SoldierNumber];
    static Soldier *s_followed;
public:
	CREATE_FUNC(Soldier);
    bool init() override;

    static bool loadAllSoldier(); //加载所有类型的Soldier

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
    inline const Vec3& target_point() {
        return _target_point;
    }
    inline void target_point(Vec3& v) {
        _target_point = v;
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

	inline void action_idle() {
		runAction(m_act_idle);
	}
	inline void action_walk() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_walk);
	}
	inline void action_run() {
        addState(SOLDIER_STATE_ACTION);
		runAction(m_act_run);
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

    void move_stop(); //停止移动
    void move(Vec3& target); //移到到指定位置

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
    float _speed = 30.f; //速度
    float _blood = 80.f; //血量
    Vec3  _target_point; //要移动到的目标点

    BillBoard *_billboard = nullptr;

    LoadingBar *_blood_bar;

    Action *_cur_action = nullptr;  //当前的动作
    PUParticleSystem3D *_cur_action_pu = nullptr;  //当前动作所使用的粒子系统

    Vec3 _camera_offset = Vec3(0.f, 60.f, 45.f); //
    float _height_offset = -10.f;

    int m_id = 0;//ID是这个对象的唯一的标识
	int m_hp;//当前HP
	int m_hp_max;//HP最大值
	int m_sp;//技能点

	Animate3D *m_act_idle;
	Action *m_act_walk;
	Animate3D *m_act_run;
	Animate3D *m_act_boxing;
	Animate3D *m_act_kick;
};

#endif
