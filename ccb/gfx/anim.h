#ifndef CCB_GRAPHICS_ANIMATION
#define CCB_GRAPHICS_ANIMATION

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"
#include "shader.h"

class Anim
{
public:

	// construction
	Anim(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,uint8_t itn,uint8_t f);
	~Anim() {  }

	// setup
	inline void texture() { Toolbox::load_texture(tex,tp); }
	uint8_t setup();
	// TODO: add setup method without animation timing

public:

	// data
	uint32_t tex;

	// values
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 pos;
	float sclx,scly;
	uint8_t r,c;
	// TODO: ?maybe also save origin position for 3D renderer objects; fast arbitrary transform

private:

	const char* tp;
	float tn,fl,ft = 0;
};

#endif
