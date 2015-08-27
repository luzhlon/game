#include "ManSoldier.h"

bool ManSoldier::init() {
	m_sprite = Sprite3D::create("test.c3b");
	addChild(m_sprite);
    return true;
}
