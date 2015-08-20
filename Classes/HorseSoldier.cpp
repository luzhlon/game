#include "HorseSoldier.h"

HorseSoldier::HorseSoldier()
{
	m_type = "qm2";
}

HorseSoldier::~HorseSoldier()
{
}

bool HorseSoldier::init() {
	return Soldier::init();
}
