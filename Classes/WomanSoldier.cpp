#include "WomanSoldier.h"

bool WomanSoldier::init() {
	m_sprite = Sprite::create("test/Cn17.png");
	addChild(m_sprite);
    return true;
}
