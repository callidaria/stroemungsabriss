#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#ifdef __WIN32__
#define STB_IMAGE_IMPLEMENTATION
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../../include/stb_image.h"
#endif
#else
#include <SOIL/SOIL.h>
#endif

#include "../mat/toolbox.h"

class Sprite
{
public:

	// construction
	Sprite(glm::vec2 p,float w,float h,const char* t);
	~Sprite();

	// setup
	void texture();
	void setup();

	// transformation
	void transform(glm::vec2 tp,glm::vec2 ts,float tr);
	void transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a);
	void translate(glm::vec2 tp);
	void scale(float wscale,float hscale);
	void scale_arbit(float wscale,float hscale);
	void rotate(float rot);

public:

	std::vector<float> v;
	unsigned int tex;
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 pos;
	float sclx,scly;

private:

	const char* texpath;
};