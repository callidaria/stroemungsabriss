#include "instanced_anim.h"

/*
	TODO
*/
InstancedAnim::InstancedAnim(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,
		uint8_t itn,uint8_t f)
	: tpath(t)
{
	// create canvas & generate texture
	v = Toolbox::create_sprite_canvas_triangled(p,w,h);
	glGenTextures(1,&tex);
}

/*
	TODO
*/
void InstancedAnim::texture()
{
	Toolbox::load_texture(tex,tpath);
}

/*
	TODO
*/
void InstancedAnim::setup()
{
	glBindTexture(GL_TEXTURE_2D,tex);
}