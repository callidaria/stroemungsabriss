#ifndef CCB_POSTPROCESSING_MSAA
#define CCB_POSTPROCESSING_MSAA

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
	~MSAA() {  }

	// action
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,rfbo); }
	void blit();

	// draw
	void render();
	void render(GLuint ovltex);

private:

	void prepare();

public:

	// buffers
	uint32_t colour_buffer,screen_buffer;

private:

	// extern
	Shader sfb = Shader();
	uint16_t fbw,fbh;

	// essentials
	Buffer buffer = Buffer();
	uint32_t rfbo,wfbo,rbo;
};

#endif