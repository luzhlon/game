#ifndef _Soldier_H_
#define _Soldier_H_
#include "string"
#include "cocos2d.h"

USING_NS_CC;
using namespace std;

class Soldier :public Node
{
	static char *angle_str[8];
	static float step_long;
	static int   Vec2Angle(Vec2 &vec);
	static int   Vec2Angle(float x, float y);
public:
	CREATE_FUNC(Soldier);
	virtual bool init();

	//virtual void MoveStep(int angle, int nStep = 1);
	virtual void MoveTo(Vec2 vec);
	//virtual void MoveTo(int nStep, int angle) = 0;
	void StartMoveAction(Vec2 vec);//用于移动
	void endMoveAction();//自己调用，用于判断移动是否终止
	void update(float dt);
	void attack(int id);
protected:
	Sprite* m_sprite;
	char *m_type = NULL;
	int m_id = 0;//ID是这个对象的唯一的标识
	int m_hp;//当前HP
	int m_hp_max;//HP最大值
	int m_sp;//技能点
	int m_sp_max;//技能点最大值
	float m_speed; //人物移动速度
	bool isMoving = false;
	int ANT;//攻击力 攻击力与防御力我建议写在服务器的数据库里
	int DEF;//防御力
	int skill[5];//技能，最大为四个。0代表普通攻击,-1代表无
	int owner;//所属,0代表中立
	int soldierSpeed=5;//速度，速度值越大人物行走的越慢——。我暂时想到了这个方法
	int compareSpeed=0;//与speed比较相同就前进。	
	
	Animation *m_animation[8]; //8个方向动画
	SpriteFrame* m_stand_frame[8]; //8个方向站立图片
};
#endif
