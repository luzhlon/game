#include "ManSoldier.h"

bool ManSoldier::init() {
	m_sprite = Sprite::create("test/Cn18.png");
	addChild(m_sprite);
    return true;
}
