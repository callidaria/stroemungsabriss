#ifndef CCB_CORE_IMPLEMENTATION
#define CCB_CORE_IMPLEMENTATION

#include "mat/camera3d.h"
#include "mat/lighting.h"

class Core
{
public:
	inline static Camera3D gCamera3D;
	inline static Lighting gLighting;  // TODO: remove from here
};

#endif