#ifndef CCB_CORE_IMPLEMENTATION
#define CCB_CORE_IMPLEMENTATION

#include <queue>

#include "frm/frame.h"
#include "gfx/renderer.h"

#include "gfx/renderer3d.h"
#include "gfx/rendereri.h"
#include "gfx/particle_system.h"

#include "mat/camera3d.h"
#include "mat/lighting.h"


// enumeration of load instructions definitions, to add more instructions and dynamically scale
enum LoadInstruction
{
	LOAD_CASINO,
	LOAD_AIRFIELD,
	LOAD_AREA51
};
// FIXME: do this somewhere else. also load queue has to be changed fundamentally


struct Core
{

	// engine
	inline static Frame gFrame = Frame("黄泉先生");
	inline static Renderer gRenderer;

	// replacable
	inline static Renderer3D gR3D;
	inline static RendererI gRInst;
	inline static ParticleSystem gPSys;
	inline static std::queue<LoadInstruction> gLI;

	// math
	inline static Camera3D gCamera3D;
	inline static Lighting gLighting;  // TODO: remove from here
};

#endif
