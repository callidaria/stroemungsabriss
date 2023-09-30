#include "anim.h"

/*
	constructor(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t) !O(1)
	purpose: create a sprite with multiple textures. has the ability to be automatically animated
	\param p: position of animated object or multitextured sprite
	\param w: describes the objects width
	\param h: describes the objects height
	\param t: path to spritesheet
	\param row: equidistant texture collection rows on spritesheet
	\param col: equidistant texture collection columns on spritesheet
	\param itn: number of textures the spritesheet contains
	\param f: ticks the animation should take to fully iterate through all textures
*/
Anim::Anim(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,uint8_t itn,uint8_t f)
		: pos(p),sclx(w),scly(h),tp(t),r(row),c(col),fl(f),tn(itn)
{ glGenTextures(1,&tex); }

/*
	setup() -> uint8_t
	purpose: ready components for object rendering and calculate animation substage from raster
	\returns animation substage, representing which texture is shown at the moment,
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
