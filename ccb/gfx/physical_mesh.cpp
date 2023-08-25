#include "physical_mesh.h"

/*
	constructor(const char*,const char*,const char*,const char*,vec3,float,vec3,uint32_t&)
	purpose: construct a mesh, holding information for physical based rendering
	\param obj_path: path to .obj file, holding geometric informations of meshes vertices
	\param itex_path: path to objects texture
	\param inorm_path: path to the normal map
	\param imtl_path: path to the information about surfaces materials
	\param iems_path: path to the emission map
	\param position: objects origin position
	\param scale: the objects initial scale multiplier
	\param rotation: standard rotation of the object after load
	\param mofs: tbc value of current vertex counting head
*/
PhysicalMesh::PhysicalMesh(const char* obj_path,const char* itex_path,const char* inorm_path,
		const char* imtl_path,const char* iems_path,glm::vec3 position,float scale,
		glm::vec3 rotation,uint32_t &mofs)
	: tex_path(itex_path),norm_path(inorm_path),mtl_path(imtl_path),ems_path(iems_path),offset(mofs)
{
	// generate textures
	glGenTextures(1,&tex_colour);
	glGenTextures(1,&tex_normal);
	glGenTextures(1,&tex_material);
	glGenTextures(1,&tex_emission);

	// load object
	Toolbox::load_object(obj_path,verts,position,scale,rotation);

	// save and increase offset for phyiscal mesh render index
	size = verts.size()/TOOLBOX_OBJECT_LOAD_REPEAT;
	mofs += size;
}

/*
	texture() -> void
	purpose: load textures & maps for this mesh
*/
void PhysicalMesh::texture()
{
	Toolbox::load_texture_repeat(tex_colour,tex_path,true);
	Toolbox::load_texture_repeat(tex_normal,norm_path,false);
	Toolbox::load_texture_repeat(tex_material,mtl_path,false);
	Toolbox::load_texture_repeat(tex_emission,ems_path,false);
}