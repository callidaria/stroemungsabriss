#ifndef CCB_CORE_IMPLEMENTATION
#define CCB_CORE_IMPLEMENTATION

#include "frm/frame.h"
#include "gfx/renderer.h"

#include "mat/camera3d.h"
#include "mat/lighting.h"


class Core
{
public:

	// engine
	inline static Frame gFrame = Frame("黄泉先生");
	inline static Renderer gRenderer = Renderer();

	// math
	inline static Camera3D gCamera3D;
	inline static Lighting gLighting;  // TODO: remove from here
};

#endif
