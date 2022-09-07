#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#ifdef __WIN32__
#define STB_IMAGE_IMPLEMENTATION
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../../include/stb_image.h"
#endif
#else
#include <SOIL/SOIL.h>
#endif
#include "shader.h"

class Anim
{
public:

	// construction
	Anim(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,uint8_t itn,uint8_t f);

	// setup
	void texture();
	uint8_t setup();

	// transformation
	glm::mat4 transform(glm::vec2 tp,glm::vec2 ts,float tr);
	glm::mat4 transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a);
	void scale(float wscale,float hscale);
	// TODO: add rotation feature

private:

	glm::vec2 pos;
	float sclx,scly;
	const char* tp;

public:

	float v[16];
	glm::mat4 model = glm::mat4(1.0f);
	unsigned int tex,r,c;
	float tn, fl; float ft = 0;

	// FIXME: private/public mishandling
};