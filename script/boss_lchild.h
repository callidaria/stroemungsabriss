#pragma once

#include <iostream>

#include "struct/feature_base.h"

#include "bullet_system.h"

class BossLChild
{
public:
	static void load(CascabelBaseFeature* ccbf,uint32_t &rnd_index,int32_t* treg);
	static void update(CascabelBaseFeature* ccbf,uint32_t &rnd_index,glm::vec2 &ePos,int32_t* treg);
};