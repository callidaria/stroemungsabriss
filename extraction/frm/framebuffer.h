#ifndef CCB_FRAME_FRAMEBUFFER
#define CCB_FRAME_FRAMEBUFFER

#include "../fcn/init.h"
#include "../gfx/shader.h"
#include "../fcn/buffer.h"
#include "../mat/toolbox.h"

class FrameBuffer
{
public:

	// construction
	FrameBuffer(const char* vsp,const char* fsp,bool float_buffer=false,bool depth_buffer=false);
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,const char* vsp,
			const char* fsp,bool float_buffer=false,bool depth_buffer=false);

	// binding
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,fbo); }
	static inline void close() { glBindFramebuffer(GL_FRAMEBUFFER,0); }

	// details
	void prepare();

	// write
	void render();

private:

	void init(uint32_t fr_width,uint32_t fr_height,const char* vsp,const char* fsp,
			bool float_buffer,bool depth_buffer);

public:

	// engine components
	Buffer buffer = Buffer();
	Shader s = Shader();

	// buffer & textures
	uint32_t fbo,tex,dptex;
};

#endif