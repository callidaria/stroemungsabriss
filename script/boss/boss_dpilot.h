#pragma once

#include <iostream>
#include <math.h>

#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>

#include "../struct/feature_base.h"

#include "../systems/bullet_system.h"

#define E 2.7182818284

// 1st phase, 1st attack
void flaredrop(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos);
void mines(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos);
void directional_sweep(BulletSystem* bSys,int32_t* treg,glm::vec2 pPos,glm::vec2 ePos);
void whirlpool(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos);

// 1st phase, switching phase attack
void bomb(BulletSystem*,int32_t*);

class BossDPilot
{
public:
	static void load(CascabelBaseFeature* ccbf,uint32_t &rnd_index,int32_t* treg);
	static void update(CascabelBaseFeature* ccbf,uint32_t &rnd_index,glm::vec2 &ePos,int32_t* treg);
};