#pragma once

#include "../../ccb/frm/frame.h"

#include "../../ccb/gfx/renderer2d.h"
#include "../../ccb/gfx/renderer3d.h"
#include "../../ccb/gfx/rendereri.h"

#include "../systems/bullet_system.h"
#include "../systems/player.h"

struct CascabelBaseFeature {
	Frame* frame;
	Renderer2D* r2d;
	Renderer3D* r3d;
	RendererI* rI;
	BulletSystem* bSys;
	Player* player;
};