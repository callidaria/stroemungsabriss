#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"

constexpr uint32_t INSTANCE_MCAP = 4096;

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

	std::vector<float> v;
	GLuint tex;
	glm::vec2 o[INSTANCE_MCAP] = { glm::vec2(0) };

private:

	const char* tp;
};

// FIXME: fix private/public as well as includes