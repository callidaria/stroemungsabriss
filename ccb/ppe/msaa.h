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
	void prepare();
	static inline void render() { glDrawArrays(GL_TRIANGLES,0,6); }

public:

	// shader
	Shader sfb = Shader();

	// buffers
	uint32_t colour_buffer,screen_buffer;

private:

	// extern
	uint16_t fbw,fbh;

	// essentials
	Buffer buffer = Buffer();
	uint32_t rfbo,wfbo,rbo;
};

#endif