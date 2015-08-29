#ifndef _Soldier_H_
#define _Soldier_H_
#include "string"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

#define UPDATE_RATE 0.02f

class Soldier : public Node
{
public:
    enum Status {
        STOP,
        MOVE,

        END
    };
public:
	CREATE_FUNC(Soldier);
    bool init() override;

    enum { SoldierNumber = 3 };
    static Soldier *s_soldiers[SoldierNumber];
    //static bool loadAllSoldier();

    inline void setAngle(Vec2 v) {
        _angle = v.getNormalized();
    }

    inline float getSpeed() {
        return _speed;
    }
    inline void setSpeed(float s) {
        _speed = s;
    }

    inline void setStatus(Status s) {
        _status = s;
    }
    inline Status getStatus() {
        return _status;
    }

    void updatePos(float );

    inline void action_move() {
        setStatus(MOVE);
    }
    inline void action_move(Vec2 angle) {
        setAngle(angle);
        action_move();
    }

    inline void action_stop() {
        setStatus(STOP);
    }

	//virtual void MoveStep(int angle, int nStep = 1);
    //virtual void MoveTo(Vec2 vec);
	//virtual void MoveTo(int nStep, int angle) = 0;
protected:
    Sprite3D* _sprite = nullptr;
    Status _status = STOP;
    float _speed = 200.f;
    //float _oritation = 0.f;
    float _base_angle_y = 180.f; //dgree
    Vec2  _angle;

    int m_id = 0;//ID是这个对象的唯一的标识
	int m_hp;//当前HP
	int m_hp_max;//HP最大值
	int m_sp;//技能点
    /*
	Animation *m_animation[8]; //8个方向动画
    SpriteFrame* m_stand_frame[8]; //8个方向站立图片 // */
};
#endif
