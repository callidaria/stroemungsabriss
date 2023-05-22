#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>

#include "../mat/toolbox.h"

// rotary joints for animation information
struct ColladaJoint {
	std::string id;
	glm::mat4 trans;
	std::vector<ColladaJoint> children;
};
// TODO: correlate string ids with equivalent integers after read

class MeshAnimation
{
public:

	// construction
	MeshAnimation(const char* path,const char* itex_path,uint32_t &mofs);
	~MeshAnimation() {  }

	// load
	void texture();

private:

	// read helpers
	std::vector<float> extract_array_data(std::vector<std::string> raw_data);
	std::vector<std::string> parameters_from_line(std::string line);

	// interpretation
	ColladaJoint assemble_joint_hierarchy(std::ifstream &file);

public:

	// vertex data
	std::vector<float> verts;
	uint32_t tex;
	uint16_t ofs,size = 0;

	// rigging data
	ColladaJoint jroot = { "ROOT",glm::mat4(1.0f),{  } };

	// transformation
	glm::mat4 model = glm::mat4(1.0f);

private:

	// vertex information
	const char* tex_path;
	std::vector<float> vaddress;
};

#endif