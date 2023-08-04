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
			const char* fsp,bool float_buffer);
	FrameBuffer(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
			const char* vsp,const char* fsp,bool float_buffer);

	// read
	void bind();

	// details
	void prepare();
	//void create_depth_texture();

	// write
	void render();
	void render(float ptrans);

	// getters
	GLuint get_fbo();
	GLuint get_tex();
	GLuint get_depth();

	// setters
	void overwrite_texture(GLuint tex);

	// statics
	static void close();

private:

	// standard initialization
	void init(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer);

public:

	// engine components
	Buffer buffer = Buffer();
	Shader s = Shader();

private:

	// buffers
	GLuint tex,dptex,rbo;
	GLuint fbo;
	uint32_t frw,frh;
};

#endif