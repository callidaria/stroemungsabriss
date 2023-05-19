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
	~Anim();

	// setup
	void texture();
	uint8_t setup();

	// transformation
	void scale(float wscale,float hscale);
	// TODO: add transformation feature
	// TODO: add rotation feature
	// TODO: add setup method without animation timing

public:

	// system
	std::vector<float> v;
	GLuint tex;

	// values
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 pos;
	float sclx,scly;
	uint8_t r,c;

private:

	const char* tp;
	float tn,fl,ft = 0;
};

#endif