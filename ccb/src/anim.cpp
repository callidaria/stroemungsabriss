#include "../gfx/anim.h"

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
{
	Toolbox::load_texture(tex,tp);
}

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
	ft++;
	if(ft>=30)
		ft=0;

	// calculate animation substage
	return ft/(fl/tn);
}
// FIXME: kill the branch. this is loopcode!

/*
	transform(vec2,vec2,float) -> mat4
	tp: new object position, resets previous model translation
	ts: new object scaling, resets previous model scaling
	tr: new object rotation, resets prevous model rotation
	returns: new model matrix comprised of given position, scaling and rotation
	DEPRECATED: replace this method by resolving the [FIXME]
*/
glm::mat4 Anim::transform(glm::vec2 tp,glm::vec2 ts,float tr)
{
	glm::mat4 trans = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x+sclx/2+tp.x,pos.y+scly/2+tp.y,0));
	trans = glm::rotate(trans,glm::radians(tr),glm::vec3(0,0,1));
	trans = glm::scale(trans,glm::vec3(ts.x,ts.y,1));
	trans = glm::translate(trans,glm::vec3(-(pos.x+sclx/2),-(pos.y+scly/2),0));
	return trans;
}  // FIXME: why return transformation and multiply outside if a model matrix member exists

/*
	transform(vec2,vec2,float,vec2) -> mat4
	tp: new object position, resets previous model translation
	ts: new object scaling, resets previous model scaling
	tr: new object rotation, resets prevous model rotation
	a: arbitrary axis to use as the animation objects pseudo-center
	returns: new model matrix comprised of given parameters around an arbitrary axis
*/
glm::mat4 Anim::transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a)
{
	float ax=a.x-(pos.x+sclx/2);float ay=a.y-(pos.y+scly/2);
	glm::mat4 trans = glm::translate(glm::mat4(1.0f),
			glm::vec3(pos.x+sclx/2+tp.x+ax,pos.y+scly/2+tp.y+ay,0));
		trans = glm::rotate(trans,glm::radians(tr),glm::vec3(0,0,1));
		trans = glm::scale(trans,glm::vec3(ts.x,ts.y,1));
		trans = glm::translate(trans,glm::vec3(-(pos.x+sclx/2+ax),-(pos.y+scly/2+ay),0));
		return trans;
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