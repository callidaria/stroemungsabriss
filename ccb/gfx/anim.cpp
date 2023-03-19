#include "anim.h"

/*
	constructor(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t)
	p: position of animated object or multitextured sprite
	w: describes the objects width
	h: describes the objects height
	t: path to spritesheet
	row: equidistant texture collection rows on spritesheet
	col: equidistant texture collection columns on spritesheet
	itn: number of textures the spritesheet contains
	f: ticks the animation should take to fully iterate through all textures
	purpose: create a sprite with multiple textures. has the ability to be automatically animated
*/
Anim::Anim(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,uint8_t itn,uint8_t f)
		: pos(p),sclx(w),scly(h),tp(t),r(row),c(col),fl(f),tn(itn)
{
	v = Toolbox::create_sprite_canvas(p,w,h);
	glGenTextures(1,&tex);
} Anim::~Anim() {  }

/*
	texture() -> void
	purpose: upload texture data to shader program to be used as sampler2D
*/
void Anim::texture()
{ Toolbox::load_texture(tex,tp); }

/*
	setup() -> uint8_t
	purpose: ready components for object rendering and calculate animation substage from raster
	returns: animation substage, representing which texture is shown at the moment,
		if the object should be animated
*/
uint8_t Anim::setup()
{
	glBindTexture(GL_TEXTURE_2D,tex);

	// iterate animation raster
	bool nr_incr = ft<30;
	ft += nr_incr-ft*!nr_incr;

	// calculate animation substage
	return ft/(fl/tn);
}

/*
	scale(float,float) -> void
	wscale: x-axis scale describing the sprites width
	hscale: y-axis scale describing the sprites height
	purpose: sets scaling of animation. does not depend on former scaling
*/
void Anim::scale(float wscale,float hscale)
{
	model[0][0] = wscale;
	model[1][1] = hscale;
}