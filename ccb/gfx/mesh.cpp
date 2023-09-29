#include "mesh.h"

/*
	constructor(const char*,const char*,const char*,const char*,const char*,vec3,float,
			vec3,uint16_t*) vertices:!O(n)
	purpose: create components, read vertex data from source file and calculate normals
	\param m: path to .obj file to load the mesh vertices from
	\param t: path to main texture file
	\param sm: path to specular map file
	\param nm: path to normal map file
	\param em: path to emission map file
	\param pos: the mesh objects origin position
	\param scale: the mesh objects origin scaling
	\param rot: the mesh objects origin rotation
	\param vl: vertex array to save loaded vertex data to
	\param mofs: memory offset, holding future memory position for created mesh
*/
Mesh::Mesh(const char* m,const char* t,const char* sm,const char* nm,const char* em,
		glm::vec3 pos,float scale,glm::vec3 rot,std::vector<float> &vl,uint32_t &mofs)
	: texpath(t),smpath(sm),nmpath(nm),empath(em),ofs(mofs)
{
	// generate textures
	glGenTextures(1,&tex),glGenTextures(1,&specmap),
			glGenTextures(1,&normap),glGenTextures(1,&emitmap);

	// load object
	size = Toolbox::load_object(m,vl,pos,scale,rot);
	mofs += size;
}

/*
	texture() -> void !O(1)
	purpose: upload texture data to shader program to be used as sampler2D
*/
void Mesh::texture()
{
	Toolbox::load_texture_repeat(tex,texpath,true);
	Toolbox::load_texture_repeat(specmap,smpath,true);
	Toolbox::load_texture_repeat(normap,nmpath);
	Toolbox::load_texture_repeat(emitmap,empath,true);
}
