#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

class MeshAnimation
{
public:

	// construction
	MeshAnimation(const char* path);
	~MeshAnimation() {  }

private:

	// vertex data
	std::vector<float> verts,elements;
};

#endif