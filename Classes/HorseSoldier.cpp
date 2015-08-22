#include "HorseSoldier.h"

bool HorseSoldier::init() {
	m_sprite = Sprite::create("test/Cn16.png");
    addChild(m_sprite);
    return true;
}
