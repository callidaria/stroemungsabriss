#pragma once

#include "../mat/toolbox.h"

class PhysicalMesh
{
public:

	// construction
	PhysicalMesh(const char* obj_path,const char* itex_path,const char* inorm_path,
			const char* imtl_path,glm::vec3 position,float scale,glm::vec3 rotation,uint32_t &mofs);
	~PhysicalMesh() {  }

	// setup
	void texture();

public:

	// gpu values
	std::vector<float> verts;
	uint32_t tex_colour,tex_normal,tex_material;
	glm::mat4 model = glm::mat4(1);
	uint16_t size,offset;

private:

	// information
	const char* tex_path,*norm_path,*mtl_path;
};