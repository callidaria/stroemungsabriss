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
	FrameBuffer(); // !!trim down usages if possible
	FrameBuffer(int fr_width,int fr_height,const char* vsp,const char* fsp,bool float_buffer);
	FrameBuffer(int fr_width,int fr_height,int fr_wres,int fr_hres,const char* vsp,const char* fsp,
		bool float_buffer);

	void init(int fr_width,int fr_height,int fr_wres,int fr_hres,const char* vsp,const char* fsp,
		bool float_buffer);
	void bind(); 	// binds the framebuffer
	void close(); 	// closes every framebuffer ??maybe get this into frame.h
	void render(); 	// renders the framebuffer
	void render_wOverlay(float ptrans);

	//getters
	unsigned int get_fbo(); // !!check if used
	unsigned int get_tex(); // !!check if used
private:
	Shader s = Shader();
	GLuint tex,rbo;
	Buffer buffer = Buffer();
	unsigned int fbo;
	uint32_t frw,frh;
};