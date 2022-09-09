#pragma once

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
	glm::mat4 transform(glm::vec2 tp,glm::vec2 ts,float tr);
	glm::mat4 transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a);
	void scale(float wscale,float hscale);
	// TODO: add rotation feature

public:

	std::vector<float> v;
	glm::mat4 model = glm::mat4(1.0f);
	GLuint tex;
	glm::vec2 pos;
	float sclx,scly;
	uint8_t r,c;

private:

	const char* tp;
	float tn,fl,ft = 0;
};