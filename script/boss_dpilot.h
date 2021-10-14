#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../ccb/gfx/renderer2d.h"
#include "bullet_system.h"

class BossDPilot
{
public:
	static void load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg);
	static void update(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,glm::vec2 pPos,glm::vec2 ePos,
				int32_t* treg);
};
