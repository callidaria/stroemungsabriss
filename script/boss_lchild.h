#pragma once

#include <iostream>
#include "../ccb/gfx/renderer3d.h"
#include "bullet_system.h"

class BossLChild
{
public:
	static void load(Renderer3D* r3d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg);
	static void update(Renderer3D* r3d,uint32_t &rnd_index,BulletSystem* bSys,glm::vec2 pPos,glm::vec2 ePos,
				int32_t* treg);
};