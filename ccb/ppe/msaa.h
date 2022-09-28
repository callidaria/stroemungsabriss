#pragma once

#include <iostream>

#include "../fcn/buffer.h"
#include "../gfx/shader.h"

class MSAA
{
public:

	// construction
	MSAA();
	MSAA(const char* vsp,const char* fsp,uint32_t bw,uint32_t bh,int los);
	// TODO: make resolution dynamic

	// action
	void bind();
	void blit();

	// draw
	void render();

private:

	// extern
	Shader sfb;
	uint32_t fbw,fbh;

	// essentials
	Buffer buffer = Buffer();
	GLuint rfbo,wfbo,rbo,colbuffer,scrbuffer;
};