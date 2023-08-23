#ifndef CCB_FRAME_GBUFFER
#define CCB_FRAME_GBUFFER

#include <iostream>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include "../mat/toolbox.h"

constexpr uint8_t CCB_GBUFFER_COMPONENT_COUNT = 4;

class GBuffer
{
public:

	// construction
	GBuffer() {  }
	GBuffer(float w_res,float h_res);
	~GBuffer() {  }

	// bind
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,buffer); }

public:

	// component textures
	uint32_t t_colour,t_position,t_normals,t_materials,t_depth;

private:

	// content
	uint32_t buffer;
};

#endif