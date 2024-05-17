#ifndef SCR_STRUCTURES_FEATURE_BASE
#define SCR_STRUCTURES_FEATURE_BASE

#include <queue>

#include "../../ccb/gfx/renderer3d.h"
#include "../../ccb/gfx/rendereri.h"
#include "../../ccb/gfx/particle_system.h"

#include "../systems/bullet_system.h"
#include "../systems/input_map.h"

// enumeration of load instructions definitions, to add more instructions and dynamically scale
enum LoadInstruction
{
	LOAD_CASINO,
	LOAD_AIRFIELD,
	LOAD_AREA51
};

// refers to all basic cascabel tools and features
struct CascabelBaseFeature {
	Renderer3D* r3d;
	RendererI* rI;
	ParticleSystem* pSys;
	BulletSystem* bSys;
	InputMap* iMap;
	std::queue<LoadInstruction> ld;
};

#endif
