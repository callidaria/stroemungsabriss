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
	MeshAnimation(const char* path,uint32_t &mofs);
	~MeshAnimation() {  }

public:

	// vertex data
	std::vector<float> verts;
	uint16_t ofs,size = 0;

private:

	// vertex address
	std::vector<float> vaddress;
};

#endif