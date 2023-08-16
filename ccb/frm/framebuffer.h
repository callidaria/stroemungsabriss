#ifndef CCB_FRAME_FRAMEBUFFER
#define CCB_FRAME_FRAMEBUFFER

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
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,const char* vsp,
			const char* fsp,bool float_buffer=false,bool depth_buffer=false);
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
			const char* vsp,const char* fsp,bool float_buffer=false,bool depth_buffer=false);

	// read
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,fbo); }

	// details
	void prepare();

	// write
	void render();
	void render(float ptrans);

	// getters
	inline uint32_t get_fbo() { return fbo; }
	inline uint32_t get_tex() { return tex; }
	inline uint32_t get_depth() { return dptex; }

	// setters
	inline void overwrite_texture(GLuint wTex) { tex = wTex; }

	// statics
	static inline void close() { glBindFramebuffer(GL_FRAMEBUFFER,0); }

private:

	// standard initialization
	void init(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer,bool depth_buffer);

public:

	// engine components
	Buffer buffer = Buffer();
	Shader s = Shader();

private:

	// buffers
	uint32_t fbo;
	uint32_t tex,dptex;//,rbo;

	// dimensions
	uint32_t frw,frh;
};

#endif