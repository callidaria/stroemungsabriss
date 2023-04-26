#include "physical_mesh.h"

/*
	TODO
*/
PhysicalMesh::PhysicalMesh(const char* obj_path,const char* itex_path,const char* inorm_path,
		const char* imtl_path,const char* irgh_path,const char* iamo_path,glm::vec3 position,
		float scale,glm::vec3 rotation,uint32_t &mofs)
	: tex_path(itex_path),norm_path(inorm_path),mtl_path(imtl_path),rgh_path(irgh_path),
		amo_path(iamo_path),offset(mofs)
{
	// generate textures
	glGenTextures(1,&tex_colour);
	glGenTextures(1,&tex_normal);
	glGenTextures(1,&tex_metal);
	glGenTextures(1,&tex_rough);
	glGenTextures(1,&tex_amocc);
	// FIXME: reduce

	// load object
	Toolbox::load_object(obj_path,verts,position,scale,rotation);

	// save and increase offset for phyiscal mesh render index
	size = verts.size()/14;
	mofs += size;
}

/*
	TODO
*/
void PhysicalMesh::texture()
{
	Toolbox::load_texture_repeat(tex_colour,tex_path,true);
	Toolbox::load_texture_repeat(tex_normal,norm_path,false);
	Toolbox::load_texture_repeat(tex_metal,mtl_path,false);
	Toolbox::load_texture_repeat(tex_rough,rgh_path,false);
	Toolbox::load_texture_repeat(tex_amocc,amo_path,false);
}