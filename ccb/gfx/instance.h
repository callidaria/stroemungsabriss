#ifndef CCB_GRAPHICS_INSTANCE
#define CCB_GRAPHICS_INSTANCE

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"

constexpr uint32_t INSTANCE_MCAP = 4096;
constexpr uint32_t INSTANCE_REPEAT = 6;
constexpr uint32_t INSTANCE_VALUES = INSTANCE_MCAP*INSTANCE_REPEAT;

class Instance
{
public:

	// construction
	Instance(glm::vec2 p,float w,float h,const char* t);
	~Instance() {  }

	// setup
	inline void texture() { Toolbox::load_texture(tex,tp); }
	inline void setup() { glBindTexture(GL_TEXTURE_2D,tex); }

public:

	std::vector<float> v;
	GLuint tex;
	float o[INSTANCE_VALUES];

private:

	const char* tp;
};

#endif

// FIXME: fix private/public as well as includes