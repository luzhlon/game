#ifndef _Soldier_H_
#define _Soldier_H_
#include "string"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Soldier : public Node
{
    /*
	static char *angle_str[8];
	static float step_long;
	static int   Vec2Angle(Vec2 &vec);
    static int   Vec2Angle(float x, float y); // */
public:
	CREATE_FUNC(Soldier);
	virtual bool init();

    enum { SoldierNumber = 3 };
    static Soldier *s_soldiers[SoldierNumber];
    static bool loadAllSoldier(); //

	//virtual void MoveStep(int angle, int nStep = 1);
    //virtual void MoveTo(Vec2 vec);
	//virtual void MoveTo(int nStep, int angle) = 0;
protected:
	Sprite* m_sprite;
	int m_id = 0;//ID是这个对象的唯一的标识
	int m_hp;//当前HP
	int m_hp_max;//HP最大值
	int m_sp;//技能点
    /*
	Animation *m_animation[8]; //8个方向动画
    SpriteFrame* m_stand_frame[8]; //8个方向站立图片 // */
};
#endif
