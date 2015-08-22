#include "HorseSoldier.h"

bool HorseSoldier::init() {
    m_sprite = Sprite::createWithTexture();
    addChild(m_sprite);
    return true;
}
