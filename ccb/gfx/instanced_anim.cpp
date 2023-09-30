#include "instanced_anim.h"

/*
	constructor(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t)
			IANIMATION_VALUE_RANGE:!O(n)
	purpose: create instanced animation object & setup upload data
	\param p: origin position of instance animation source object
	\param w: standard width of instance animation source object
	\param h: standard height of instance animation source object
	\param t: path to texture holding animation or state spritesheet
	\param row: number of subtexture rows contained by linked spritesheet
	\param col: number of subtexture columns contained by linked spritesheet
	\param itn: total number of subtextures contained by linked spritesheet
	\param f: ticks a total iterations through all subtextures on the spritesheet should take
*/
InstancedAnim::InstancedAnim(glm::vec2 p,float w,float h,const char* t,uint8_t irow,uint8_t icol,
		uint8_t itn,uint8_t f)
	: position(p),width(w),height(h),tpath(t),row(irow),col(icol),st_count(itn),tick_cap(f)
{
	// generate texture
	glGenTextures(1,&tex);

	// setup instance values
	for (int idx=0;idx<IANIMATION_VALUE_RANGE;idx+=IANIMATION_REPEAT)
		i[idx] = 0,i[idx+1] = 0,i[idx+2] = 0,i[idx+3] = 1,i[idx+4] = 0,i[idx+5] = 0;

	// precalculate ticks per subtexture
	tps = tick_cap/st_count;
}

/*
	setup(Shader*) -> void !O(1)
	purpose: setup animated instance draw
	\param shader: shader program to upload saved uniform variables to
	\param purpose: takes all actions necessary to render instances from source object
*/
void InstancedAnim::setup(Shader* shader)
{
	glBindTexture(GL_TEXTURE_2D,tex);
	shader->upload_int("row",row),shader->upload_int("col",col);
}

/*
	update(float) -> void IANIMATION_MCAP:!O(n)
	purpose: update ticks of all object related instances & upload current texture index accordingly
	\param dtime: time delta to bind animation updates to ingame speed, not gfx update frequency
*/
void InstancedAnim::update(float dtime)
{
	unsigned int tex_index,tex_x,tex_y;
	for (int idx=0;idx<IANIMATION_MCAP;idx++) {

		// calculate subtexture position
		tex_index = ticks[idx]/tps;
		tex_x = tex_index%col;tex_y = tex_index/col;
		i[idx*IANIMATION_REPEAT+4] = tex_x;i[idx*IANIMATION_REPEAT+5] = tex_y;

		// increment update ticks
		ticks[idx] += dtime;
		ticks[idx] *= (ticks[idx]<tick_cap);
	}
}
