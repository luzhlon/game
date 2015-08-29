#include <math/CCMath.h>
#include "Soldier.h"
#include "WomanSoldier.h"

//#include "ManSoldier.h"
//#include "HorseSoldier.h"

/*
char *Soldier::angle_str[8] = { "right", "ur", "up", "ul", "left", "dl", "down", "dr" };
float Soldier::step_long = 10.0;

int Soldier::Vec2Angle(float x, float y) {
	if (x == 0.0) {
		return y > 0.0 ? 2 : 6;
	}
	double rate_225 = tan(0.3927); //22.5
	double rate_675 = tan(1.178); //67.5
	double rate = abs(y) / abs(x);
	if (rate <= rate_225) {
		return x > 0.0 ? 0 : 4;
	}
	if (rate >= rate_675) {
		return y > 0.0 ? 2 : 6;
	}
	if (x > 0.0) {
		return y > 0.0 ? 1 : 7;
	}
	else {
		return y > 0.0 ? 3 : 5;
	}
}

int Soldier::Vec2Angle(Vec2 &vec) {
	return Vec2Angle(vec.x, vec.y);
}

bool Soldier::init(){
	SpriteFrameCache* frameCache = SpriteFrameCache::getInstance();
	//将plist加载到缓存池
	CCAssert(m_type, "Soldier type is null");
	frameCache->addSpriteFramesWithFile(
		StringUtils::format("./Soldier/%s.plist", m_type), 
		StringUtils::format("./Soldier/%s.png", m_type));
	//
	Vector<SpriteFrame*> frameVec[8];
	for (int i = 0; i < 8; i++){ //帧序
		for (int j = 0; j < 8; j++){ //角度
			frameVec[j].pushBack(
				frameCache->getSpriteFrameByName(StringUtils::format("%s%d.png", angle_str[j], 8 - i)));
		}
	}
	//
	auto ani_cache = AnimationCache::sharedAnimationCache();
	for (int i = 0; i < 8; i++){
		//将站立的动作添加到m_stand_frame中
		m_stand_frame[i] = 
			frameCache->getSpriteFrameByName(StringUtils::format("%s9.png", angle_str[i]));
		//创建动画
		m_animation[i] = Animation::createWithSpriteFrames(frameVec[i]);
		m_animation[i]->setLoops(-1);
		m_animation[i]->setDelayPerUnit(0.2);
		//将动画加到动画缓存中
		ani_cache->addAnimation(m_animation[i], angle_str[i]);
	}
	//初始化精灵
	m_sprite = Sprite::create();
	//m_sprite->setSpriteFrame();
	//m_sprite->initWithSpriteFrame(m_stand_frame[1]);
	addChild(m_sprite);
	return true;
}
// */

Soldier *Soldier::s_soldiers[Soldier::SoldierNumber];

/*
bool Soldier::loadAllSoldier() {
    bool loaded = true;
    loaded = loaded && (s_soldiers[0] = HorseSoldier::create());
    loaded = loaded && (s_soldiers[1] = WomanSoldier::create());
    loaded = loaded && (s_soldiers[2] = ManSoldier::create());
    if(loaded) {
        return true;
    } else {
        return false;
    }
} // */

bool Soldier::init() {
    schedule(schedule_selector(Soldier::updatePos), UPDATE_RATE);
    return true;
}

void Soldier::updatePos(float dt) {
    switch(getStatus()) {
    case MOVE:
    {
        Vec2 pos = _sprite->getPosition();
        pos += Vec2(_speed * _angle.x * UPDATE_RATE,
                    _speed * _angle.y * UPDATE_RATE);

        Vec3 rota = _sprite->getRotation3D();
        rota.y = _base_angle_y
                + CC_RADIANS_TO_DEGREES(_angle.getAngle());
        _sprite->setRotation3D(rota);
        _sprite->setPosition(pos);
    }
    break;
    case STOP:
    break;
    }
}
