#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"

class Instance
{
public:

	// construction
	Instance(glm::vec2 p,float w,float h,const char* t);
	~Instance();

	// setup
	void texture();
	void setup();

public:

	float v[24];
	GLuint tex;
	glm::vec2 o[4096] = { glm::vec2(0) };

private:

	const char* tp;
};