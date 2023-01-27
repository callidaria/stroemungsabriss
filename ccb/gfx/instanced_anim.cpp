#include "instanced_anim.h"

/*
	constructor(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t)
	p: origin position of instance animation source object
	w: standard width of instance animation source object
	h: standard height of instance animation source object
	t: path to texture holding animation or state spritesheet
	row: number of subtexture rows contained by linked spritesheet
	col: number of subtexture columns contained by linked spritesheet
	itn: total number of subtextures contained by linked spritesheet
	f: ticks a total iterations through all subtextures on the spritesheet should take
*/
InstancedAnim::InstancedAnim(glm::vec2 p,float w,float h,const char* t,uint8_t irow,uint8_t icol,
		uint8_t itn,uint8_t f)
	: tpath(t),row(irow),col(icol),st_count(itn),tick_cap(f)
{
	// create canvas & generate texture
	v = Toolbox::create_sprite_canvas_triangled(p,w,h);
	for (int idx=0;idx<IANIMATION_VALUES;idx+=IANIMATION_REPEAT) {
		i[idx] = 0, i[idx+1] = 0, i[idx+2] = 0,
		i[idx+3] = 1, i[idx+4] = 0, i[idx+5] = 0;
	} glGenTextures(1,&tex);

	// calculate ticks per subtexture
	tps = tick_cap/st_count;
}

/*
	texture() -> void
	purpose: textures the instanced animation canvas once the shader has been compiled and bound
*/
void InstancedAnim::texture()
{ Toolbox::load_texture(tex,tpath); }

/*
	setup(Shader*) -> void
	shader: shader program to upload saved uniform variables to
	purpose: takes all actions necessary to render instances from source object
*/
void InstancedAnim::setup(Shader* shader)
{
	// upload texture
	glBindTexture(GL_TEXTURE_2D,tex);

	// upload uniform variables
	shader->upload_int("row",row);
	shader->upload_int("col",col);
}

/*
	update(float) -> void
	dtime: time delta to bind animation updates to ingame speed, not gfx update frequency
	purpose: update ticks of all object related instances & upload current texture index accordingly
*/
void InstancedAnim::update(float dtime)
{
	unsigned int tex_index,tex_x,tex_y;
	for (int idx=0;idx<IANIMATION_MCAP;idx++) {

		// calculate subtexture position
		tex_index = ticks[idx]/tps,
		tex_x = tex_index%col, tex_y = tex_index/col,
		i[idx*IANIMATION_REPEAT+4] = tex_x, i[idx*IANIMATION_REPEAT+5] = tex_y;

		// increment update ticks
		ticks[idx] += dtime;
		ticks[idx] *= (ticks[idx]<tick_cap);
	}
}

/*
	reset_tick(uint16_t) -> void
	idx: memory index in offset list to reset ticks of
	purpose: reset tick counter at given memory index
*/
void InstancedAnim::reset_tick(uint16_t idx)
{
	ticks[idx] = 0;
}