#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include "../mat/toolbox.h"

class MeshAnimation
{
public:

	// construction
	MeshAnimation(const char* path,const char* itex_path,uint32_t &mofs);
	~MeshAnimation() {  }

	// load
	void texture();

public:

	// vertex data
	std::vector<float> verts;
	uint32_t tex;
	uint16_t ofs,size = 0;

private:

	// vertex information
	const char* tex_path;
	std::vector<float> vaddress;
};

#endif