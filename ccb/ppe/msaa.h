#pragma once

#include <iostream>

#include "../frm/framebuffer.h"
#include "../gfx/shader.h"
#include "../fcn/buffer.h"

class MSAA
{
public:

	// construction
	MSAA();
	MSAA(const char* vsp,const char* fsp,uint32_t bw,uint32_t bh,int los);
	// TODO: make resolution dynamic

	// action
	void bind();
	void blit(FrameBuffer* fb);

	// draw
	void render();

	// getter
	unsigned int get_tex() { return tex; }

	// static
	static void close();

private:

	Shader sfb;
	uint32_t fbw,fbh;
	unsigned int tex,rbo;
	Buffer buffer = Buffer();
	unsigned int fbo;
};