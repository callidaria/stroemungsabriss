#pragma once

#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../mat/toolbox.h"

class GBuffer
{
public:

	// construction
	GBuffer() {  }
	GBuffer(float w_res,float h_res);
	~GBuffer() {  }

	// bind
	void bind();

	// getter
	uint32_t get_colour();
	uint32_t get_position();
	uint32_t get_normals();
	uint32_t get_materials();

private:

	// content
	uint32_t buffer;
	uint32_t t_col,t_pos,t_norm,t_pbm,rb_depth;
};