#ifndef CCB_GRAPHICS_PHYSICAL_MESH
#define CCB_GRAPHICS_PHYSICAL_MESH

#include "../mat/toolbox.h"

class PhysicalMesh
{
public:

	// construction
	PhysicalMesh(const char* obj_path,const char* itex_path,const char* inorm_path,
			const char* imtl_path,const char* iems_path,glm::vec3 position,float scale,
			glm::vec3 rotation,std::vector<float> &vl,uint32_t &mofs);
	~PhysicalMesh() {  }

	// setup
	void texture();

public:

	// data
	uint32_t tex_colour,tex_normal,tex_material,tex_emission;
	uint32_t size,offset;

	// transform
	glm::mat4 model = glm::mat4(1);

private:

	// information
	const char* tex_path,*norm_path,*mtl_path,*ems_path;
};

#endif