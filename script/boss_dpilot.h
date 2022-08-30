#pragma once

#include <iostream>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "../ccb/gfx/renderer2d.h"
#include "bullet_system.h"

#define E 2.7182818284

// 1st phase, 1st attack
void flaredrop(BulletSystem*,int32_t*,glm::vec2 ePos);
void mines(BulletSystem*,int32_t*,glm::vec2 ePos);
void directional_sweep(BulletSystem*,int32_t*,glm::vec2 pPos,glm::vec2 ePos);
void whirlpool(BulletSystem*,int32_t*,glm::vec2 ePos);

// 1st phase, switching phase attack
void bomb(BulletSystem*,int32_t*);

class BossDPilot
{
public:
	static void load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg);
	static void update(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,glm::vec2 pPos,
			glm::vec2 ePos,int32_t* treg);
};
