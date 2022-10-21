#pragma once

#include <iostream>

#include "../mat/toolbox.h"
#include "../fcn/buffer.h"
#include "../gfx/shader.h"

class MSAA
{
public:

	// construction
	MSAA();
	MSAA(const char* vsp,const char* fsp,uint16_t bw,uint16_t bh,int los);
	// TODO: make resolution dynamic

	// action
	void bind();
	void blit();

	// draw
	void render();

	// export
	GLuint get_buffer();

private:

	// extern
	Shader sfb;
	uint16_t fbw,fbh;

	// essentials
	Buffer buffer = Buffer();
	GLuint rfbo,wfbo,rbo,colbuffer,scrbuffer;
};