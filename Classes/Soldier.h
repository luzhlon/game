#ifndef _Soldier_H_
#define _Soldier_H_

#include <string>
#include <list>
#include <functional>
#include "cocos2d.h"
#include "../Server/message.h"
#include "ui/CocosGUI.h"
#include "cocostudio/CocoStudio.h"
#include "Particle3D/PU/CCPUParticleSystem3D.h"
#include "Skill.h"

USING_NS_CC;
using namespace ui;
using namespace std;
using namespace cocostudio;

class Soldier : public Sprite3D
{
public:
    enum State {
        SOLDIER_STATE_IDLE = 0,
        SOLDIER_STATE_MOVE,
        SOLDIER_STATE_SKILL,
        SOLDIER_STATE_DEATH,
        SOLDIER_STATE_REVIVE,

        STATE_NUMBER
    };
    enum Type {
        SOLDIER_TYPE_WOMAN = 0,
        SOLDIER_TYPE_MAN,
        SOLDIER_TYPE_MAN2,

        TYPE_NUMBER
    };
    enum Buff {
        BUFF_NULL = 0,
        BUFF_SPEED,

        BUFF_NUMBER
    };
    typedef function<void(Soldier *)> Callback;

    static Soldier *s_followed; // 摄像机所跟随的Soldier

    static Soldier *create(int type_id);
    static Soldier *create(room_member *rm);

public:
	CREATE_FUNC(Soldier);
    bool init() override;

    void update(float dt) override; //帧刷新

    virtual bool init_soldier() { return true; } //子类重写

    /*
    inline void addState(State state) { //给Soldier添加状态
        _state = (State)(_state | state);
    }
    inline bool atState(State s) { //判断Soldier是否处于某个状态
        return _state & s;
    }
    inline void rmState(State state) { //移除某个状态
        _state = (State)(_state & (~state));
    } // */

    inline State state() {
        return _state;
    }
    inline bool death() {
        return _state == SOLDIER_STATE_DEATH;
    }
    inline float speed() {
        return _speed;
    }
    inline void speed(float s) {
        _speed = s;
    }
    inline int grass() {
        return _grass;
    }
    inline void grass(int count) {
        _grass = count < 0 ? 0 : count;
    }
    inline float blood() {
        return _blood;
    }
    inline void onDeath(Callback cb) {
        _on_death = cb;
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
    inline room_member *member() {
        return _room_member;
    }
    inline float angle() {
        return getRotation3D().y - _base_angle;
    }
    void angle(float agl) {
        auto rota = getRotation3D();
        rota.y = _base_angle + agl;
        return setRotation3D(rota);
    }

    inline void setCameraFollowed() {
        s_followed = this;
    }

    inline void show_billboard(bool show) {
        _billboard->setVisible(show);
    }
    //UI
    void load_ui();
    Text *name_text();
    Text *blood_dec_text();
    LoadingBar *blood_bar();

    void set_blood(float blood); // 设置血量，同时更新血量显示的UI
    inline void add_blood(float blood) { // 加血，传递负数则为减血
        set_blood(_blood + blood);
    }
    //Config
    bool load_config(char *path);

    void switch_state(State state, void *data = nullptr); // 切换到某个状态
    void begin_fight(); //准备开始战斗

    virtual bool do_skill(SkillBase* skill); // 执行某个技能
    virtual bool on_skill(SkillBase *skill); // 受到某个技能攻击时
    //virtual void be_injured(float dt); // 受到伤害时，延时调用

    virtual Skill *get_skill(Skill::Type ); // 获取一个关于此人物的技能实例

    inline void move_stop() { //停止移动
        switch_state(SOLDIER_STATE_IDLE);
    }
    inline void move_to(Vec2& target) { //移到到指定位置
        switch_state(SOLDIER_STATE_MOVE, &target);
    }
    void move_by(float distance);
    Vec2 get_point(float angle, float distance);

    Vec3 get_head_pos(); //
    void show_blood_decline(float); //显示减血效果
    void updatePosition(float dt = 0.f); //刷新位置:根据目标点
    void updateRotation(); //刷新朝向：根据目标点
    void updateBlood(); //刷新血量条
    void add_thing(Node *); //往自身结点添加一个Node，根据自身的CameraMask
    void add2Billboard(Node *); //把节点添加到Billboard

private:
    State _state = SOLDIER_STATE_IDLE;
    Buff _buff = BUFF_NULL;

protected:
    friend class Player;

    Type _role_id = (Type)(-1); //所属角色的ID
    room_member *_room_member = nullptr; // room_member 信息
    string _name = "<unset>"; //头顶显示的名称

    float _speed = 30.f; //速度
    float _vitality = 2.f; // 元气，生命力

    float _full_blood = 300.f; // 满血
    float _blood = 300.f; //血量
    int _grass = 10;  // 初始粮草数

    Vec3  _target_point; //要移动到的目标点
    float _base_angle = 90.f; // 基准角度

    //UI
    BillBoard *_billboard = nullptr;

    // 受到攻击时待处理的技能
    //list<SkillBase *> _skill_list;
    Callback _on_death = [](Soldier *sol) {
        sol->switch_state(SOLDIER_STATE_DEATH);
    }; // 

    PUParticleSystem3D *_cur_action_pu = nullptr;  //当前动作所使用的粒子系统

    //五个基本动作
	ActionInterval *m_act_idle = nullptr;
	ActionInterval *m_act_walk = nullptr;
	ActionInterval *m_act_special = nullptr;
	ActionInterval *m_act_boxing = nullptr;
	ActionInterval *m_act_kick = nullptr;
};

#endif
