#include "physical_mesh.h"

/*
	constructor(const char*,const char*,const char*,const char*,const char*,const char*,vec3,float,
			vec3,uint32_t&)
	purpose: construct a mesh, holding information for physical based rendering
	\param obj_path: path to .obj file, holding geometric informations of meshes vertices
	\param itex_path: path to objects texture
	\param inorm_path: path to the normal map
	\param imtl_path: path to the information about surfaces metalness
	\param irgh_path: path to the information about surfaces roughness
	\param iamo_path: path to the information about the objects self-inflicted ambient occlusion
	\param position: objects origin position
	\param scale: the objects initial scale multiplier
	\param rotation: standard rotation of the object after load
	\param mofs: tbc value of current vertex counting head
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
// TODO: join material, roughness & ambient occlusion/displacement into one texture

/*
	texture() -> void
	purpose: load textures & maps for this mesh
*/
void PhysicalMesh::texture()
{
	Toolbox::load_texture_repeat(tex_colour,tex_path,true);
	Toolbox::load_texture_repeat(tex_normal,norm_path,false);
	Toolbox::load_texture_repeat(tex_metal,mtl_path,false);
	Toolbox::load_texture_repeat(tex_rough,rgh_path,false);
	Toolbox::load_texture_repeat(tex_amocc,amo_path,false);
}