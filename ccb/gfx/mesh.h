#ifndef CCB_GRAPHICS_MESH
#define CCB_GRAPHICS_MESH

#include "../mat/math.h"


struct Mesh
{
	// utility
	void load_mesh(std::string path);

	// data
	Transform3D transform;
};

#endif
