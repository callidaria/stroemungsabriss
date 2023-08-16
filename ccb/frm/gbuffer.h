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
	void bind();

	// getter
	inline uint32_t get_colour() { return t_col; }
	inline uint32_t get_position() { return t_pos; }
	inline uint32_t get_normals() { return t_norm; }
	inline uint32_t get_materials() { return t_pbm; }
	inline uint32_t get_depth() { return t_depth; }

private:

	// content
	uint32_t buffer,wbuffer;
	uint32_t t_col,t_pos,t_norm,t_pbm,/*rb_depth,*/t_depth;
};

#endif