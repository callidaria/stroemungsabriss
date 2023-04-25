#include "mesh.h"

/*
	constructor(const char*,const char*,const char*,const char*,const char*,vec3,float,
			vec3,uint16_t*)
	m: path to .obj file to load the mesh vertices from
	t: path to main texture file
	sm: path to specular map file
	nm: path to normal map file
	em: path to emission map file
	ip: the mesh objects origin position
	is: the mesh objects initial scaling
	ir: the mesh objects initial rotation
	mofs: memory offset, describing at which position in vertex memory the created mesh starts
	purpose: create components, read vertex data from source file and calculate normals
*/
Mesh::Mesh(const char* m,const char* t,const char* sm,const char* nm,const char* em,
		glm::vec3 ip,float is,glm::vec3 ir,unsigned int* mofs)
	: texpath(t),smpath(sm),nmpath(nm),empath(em),pos(ip),scl(is),rot(ir),ofs(*mofs)
{
	// generate textures
	glGenTextures(1,&tex);		// object texture
	glGenTextures(1,&specmap);	// specular map
	glGenTextures(1,&normap);	// normal map
	glGenTextures(1,&emitmap);	// light emission map

	// load object
	Toolbox::load_object(m,v,pos,scl,rot);

	// save and increase offset for mesh render index
	size = v.size()/14;
	*mofs += size;
} Mesh::~Mesh() {  }

/*
	texture() -> void
	purpose: upload texture data to shader program to be used as sampler2D
*/
void Mesh::texture()
{
	Toolbox::load_texture_repeat(tex,texpath,true);
	Toolbox::load_texture_repeat(specmap,smpath,true);
	Toolbox::load_texture_repeat(normap,nmpath);
	Toolbox::load_texture_repeat(emitmap,empath,true);
}