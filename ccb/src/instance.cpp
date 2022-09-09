#include "../gfx/instance.h"

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
	// triangle 1
	v[0] = p.x+w;v[1] = p.y+h;v[2] = 1.0f;v[3] = 0.0f;
	v[4] = p.x;v[5] = p.y;v[6] = 0.0f;v[7] = 1.0f;
	v[8] = p.x+w;v[9] = p.y;v[10] = 1.0f;v[11] = 1.0f;

	// triangle 2
	v[12] = p.x+w;v[13] = p.y+h;v[14] = 1.0f;v[15] = 0.0f;
	v[16] = p.x;v[17] = p.y;v[18] = 0.0;v[19] = 1.0f;
	v[20] = p.x;v[21] = p.y+h;v[22] = 0.0f;v[23] = 0.0f;
	// TODO: find out why this pattern is the way it is?? o.O
	// FIXME: aged vertex pattern, combine index & elements?

	// generate textures
	glGenTextures(1,&tex);
} Instance::~Instance() {  }

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