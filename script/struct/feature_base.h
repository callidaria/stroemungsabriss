#ifndef SCR_STRUCTURES_FEATURE_BASE
#define SCR_STRUCTURES_FEATURE_BASE

#include <vector>
#include <queue>

#include "../../ccb/frm/frame.h"

#include "../../ccb/gfx/renderer.h"
#include "../../ccb/gfx/renderer3d.h"
#include "../../ccb/gfx/rendereri.h"
#include "../../ccb/gfx/particle_system.h"

#include "../../ccb/mat/camera3d.h"
#include "../../ccb/mat/lighting.h"

#include "../systems/bullet_system.h"
#include "../systems/input_map.h"

// enumeration of load instructions definitions, to add more instructions and dynamically scale
enum LoadInstruction
{
	LOAD_START,
	LOAD_MENU,
	LOAD_CASINO,
	LOAD_CARDTABLE,
	LOAD_AIRFIELD,
	LOAD_DPILOT
};

// refers to all basic cascabel tools and features
struct CascabelBaseFeature {
	Frame* frame;
	Renderer* rnd;
	Renderer3D* r3d;
	RendererI* rI;
	ParticleSystem* pSys;
	BulletSystem* bSys;
	InputMap* iMap;
	std::queue<LoadInstruction> ld;
};

#endif
