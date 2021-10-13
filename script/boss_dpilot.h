#pragma once

#include <iostream>
#include "../ccb/gfx/renderer2d.h"
#include "bullet_system.h"

class BossDPilot
{
public:
	static void load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys);
	static void update(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys);
};
