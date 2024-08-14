#include "instance.h"

/*
	constructor(vec2,float,float,const char*) INSTANCE_VALUE_RANGE:!O(n)
	purpose: create a new instancable object
	\param p: origin position of instancable object
	\param w: origin x-axis scaling of all objects children
	\param h: origin y-axis scaling of all objects children
	\param t: path to file containing the parents texture
*/
Instance::Instance(glm::vec2 p,float w,float h,const char* t)
	: position(p),width(w),height(h)
{
	// generate textures
	tex = Texture(t);

	// fill index
	for (int i=0;i<INSTANCE_VALUE_RANGE;i+=INSTANCE_REPEAT)
		o[i] = 0,o[i+1] = 0,o[i+2] = 0,o[i+3] = 1,o[i+4] = 0,o[i+5] = 0;
}

/*
	TODO
*/
void Instance::texture()
{
	tex.gpu_upload();
	Texture::set_texture_parameter_clamp_to_edge();
	Texture::set_texture_parameter_linear_mipmap();
	tex.generate_mipmap();
	tex.cleanup();
}
