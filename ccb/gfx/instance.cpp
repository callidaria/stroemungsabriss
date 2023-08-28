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

	// fill index
	for (int i=0;i<INSTANCE_VALUES;i+=INSTANCE_REPEAT) {
		o[i] = 0;o[i+1] = 0;o[i+2] = 0;
		o[i+3] = 1;o[i+4] = 0;o[i+5] = 0;
	} // FIXME: this works but it is not beautiful. i imagine a shining lakeeeeuhhhhhhhh

	// generate textures
	glGenTextures(1,&tex);
}