#pragma once

#include <queue>

#include "../../ccb/frm/frame.h"

#include "../../ccb/gfx/renderer2d.h"
#include "../../ccb/gfx/renderer3d.h"
#include "../../ccb/gfx/rendereri.h"

#include "../systems/bullet_system.h"
#include "../systems/input_map.h"

// enumeration of load instructions definitions, to add more instructions and dynamically scale
enum LoadInstruction
{
	LOAD_START,
	LOAD_MENU,
	LOAD_CASINO,
	LOAD_AIRFIELD,
	LOAD_DPILOT
};

// refers to all basic cascabel tools and features
struct CascabelBaseFeature {
	Frame* frame;
	Renderer2D* r2d;
	Renderer3D* r3d;
	RendererI* rI;
	BulletSystem* bSys;
	InputMap* iMap;
	std::queue<LoadInstruction> ld;
};