#ifndef CCB_GRAPHICS_MESH_ANIMATION
#define CCB_GRAPHICS_MESH_ANIMATION

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <vector>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "../mat/toolbox.h"

// rotary joints for animation information
struct ColladaJoint {
	std::string id;
	glm::mat4 trans,inv_trans;
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

	// system
	friend std::ostream &operator<<(std::ostream &os,const MeshAnimation& obj);

private:

	// read helpers
	std::vector<float> extract_array_data(std::vector<std::string> raw_data);
	std::vector<std::string> parameters_from_line(std::string line);

	// interpretation
#ifdef LIGHT_SELFIMPLEMENTATION_COLLADA_LOAD
	static ColladaJoint rc_assemble_joint_hierarchy(std::ifstream &file);
#else
	static ColladaJoint rc_assemble_joint_hierarchy(aiNode* joint);
#endif

	// system
	static void rc_print_joint_tree(std::ostream &os,ColladaJoint cjoint,uint8_t depth);

public:

	// vertex data
	std::vector<float> verts;
	std::vector<uint32_t> elems;
	uint32_t tex;
	uint32_t ofs,size = 0;

	// rigging data
	ColladaJoint jroot;

	// transformation
	glm::mat4 model = glm::mat4(1.0f);

private:

	// vertex information
	const char* tex_path;
	std::vector<float> vaddress;
};

#endif