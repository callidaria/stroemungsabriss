#pragma once

#include <iostream>
#include <vector>
#include <GL/glew.h>

#include "../gfx/shader.h"
#include "../fcn/buffer.h"
#include "../mat/toolbox.h"

class FrameBuffer
{
public:

	// construction
	FrameBuffer() {  }
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,
		const char* vsp,const char* fsp,bool float_buffer);
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer);

	// read
	void bind();

	// write
	void render();
	void render(float ptrans);

	// getters
	GLuint get_fbo(); // !!check if used
	GLuint get_tex(); // !!check if used

	// setters
	void overwrite_texture(GLuint tex);

	// statics
	static void close();

private:

	// standard initialization
	void init(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer);

private:

	// engine components
	Buffer buffer = Buffer();
	Shader s = Shader();

	// buffers
	GLuint tex,rbo;
	GLuint fbo;
	uint32_t frw,frh;
};