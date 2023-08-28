#include "sprite.h"

/*
	constructor(vec2,float,float,const char*)
	p: the sprites origin position
	w: width of the sprite
	h: height of the sprite
	t: path to sprite texture
	purpose: calculate vertex upload values and generate texture
*/
Sprite::Sprite(glm::vec2 p,float w,float h,const char* t)
	: pos(p),sclx(w),scly(h),texpath(t)
{
	v = Toolbox::create_sprite_canvas(p,w,h);
	glGenTextures(1,&tex);
}

/*
	transform(vec2,vec2,float) -> void
	tp: position to translate the sprite to
	ts: new scaling of position
	tr: rotation of the sprite around its center
	purpose: change all attributes of the sprites position at once without reverse translations
*/
void Sprite::transform(glm::vec2 tp,glm::vec2 ts,float tr)
{
	model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x+sclx/2+tp.x,pos.y+scly/2+tp.y,0));
	model = glm::rotate(model,glm::radians(tr),glm::vec3(0,0,1));
	model = glm::scale(model,glm::vec3(ts.x,ts.y,1));
	model = glm::translate(model,glm::vec3(-(pos.x+sclx/2),-(pos.y+scly/2),0));
}

/*
	transform(vec2,vec2,float,vec2) -> void
	tp: position to translate the sprite to
	ts: new scaling of position
	tr: rotation of the sprite around its center
	a: arbitrary axis to use as pseudo center to rotate, scale and translate around
	purpose: transform sprite position but around an arbitrary, externally defined axis
*/
void Sprite::transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a)
{
	float ax = a.x-(pos.x+sclx/2),ay = a.y-(pos.y+scly/2);
	model = glm::translate(glm::mat4(1.0f),glm::vec3(pos.x+sclx/2+tp.x+ax,pos.y+scly/2+tp.y+ay,0));
	model = glm::rotate(model,glm::radians(tr),glm::vec3(0,0,1));
	model = glm::scale(model,glm::vec3(ts.x,ts.y,1));
	model = glm::translate(model,glm::vec3(-(pos.x+sclx/2+ax),-(pos.y+scly/2+ay),0));
}

/*
	scale(float,float) -> void
	wscale: x-axis scale describing the sprites width
	hscale: y-axis scale describing the sprites height
	purpose: set the sprites scaling
*/
void Sprite::scale(float wscale,float hscale)
{
	model[0][0] = wscale;
	model[1][1] = hscale;
}

/*
	scale_arbit(float,float) -> void
	wscale: x-axis scale describing the sprites width
	hscale: y-axis scale describing the sprites height
	purpose: scale sprite canvas independent from origin position
*/
void Sprite::scale_arbit(float wscale,float hscale)
{
	model = glm::mat4(1.0f);
	translate(-pos);
	scale(wscale,hscale);
	translate(pos);
}