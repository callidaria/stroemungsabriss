#include "instance.h"

/*
	constructor(vec2,float,float,const char*)
	p: origin position of instancable object
	w: origin x-axis scaling of all objects children
	h: origin y-axis scaling of all objects children
	t: path to file containing the parents texture
	purpose: create a new instancable object
*/
Instance::Instance(glm::vec2 p,float w,float h,const char* t)
	: tp(t)
{
	// create canvas
	v = Toolbox::create_sprite_canvas_triangled(p,w,h);

	// generate textures
	glGenTextures(1,&tex);
}

/*
	texture() -> void
	purpose: load texture of instancable object from given path
*/
void Instance::texture()
{
	Toolbox::load_texture(tex,tp);
}

/*
	setup() -> void
	purpose: bind texture before rendering
*/
void Instance::setup()
{
	glBindTexture(GL_TEXTURE_2D,tex);
}