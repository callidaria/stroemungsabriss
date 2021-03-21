#pragma once

#include <iostream>
#include <GL/glew.h>
#include "../gfx/shader.h"

class FrameBuffer
{
public:
	FrameBuffer(); // !!trim down usages if possible
	FrameBuffer(int fr_width, int fr_height, const char* vsp, const char* fsp, bool float_buffer);

	void bind(); 	// binds the framebuffer
	void close(); 	// closes every framebuffer ??maybe get this into frame.h
	void render(); 	// renders the framebuffer
	void render_wOverlay(uint32_t atex,uint32_t btex,uint32_t ctex,float ptrans);

	//getters
	unsigned int get_fbo(); // !!check if used
	unsigned int get_tex(); // !!check if used
private:
	Shader s;
	unsigned int tex,rbo;
	unsigned int vao,vbo,fbo;
	uint32_t frw,frh;
};
