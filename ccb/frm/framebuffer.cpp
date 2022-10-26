#include "framebuffer.h"

/*
	PARAMETER DEFINITIONS:
	fr_wres: frame resolution width
	fr_hres: frame resolution height
	vsp: path to vertex shader file
	fsp: path to fragment shader file
	float_buffer: indicates if constructed object is a float buffer
*/

/*
	constructor(uint32_t,uint32_t,const char*,const char*,bool)
	fr_width: width of frame, in this case identical to frame resolution width
	fr_height: height of frame, in this case identical to frame resolution height
	purpose: creates framebuffer object to change original visuals through shaders
*/
FrameBuffer::FrameBuffer(uint32_t fr_width,uint32_t fr_height,const char* vsp,
		const char* fsp,bool float_buffer)
	: frw(fr_width),frh(fr_height)
{
	init(fr_width,fr_height,fr_width,fr_height,vsp,fsp,float_buffer);
}

/*
	constructor(uint32_t,uint32_t,uint32_t,uint32_t,const char*,const char*,bool)
	fr_width: width of frame
	fr_height: height of frame
	purpose: creates framebuffer object to change original visuals through shaders
*/
FrameBuffer::FrameBuffer(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer)
	: frw(fr_width),frh(fr_height)
{
	init(fr_width,fr_height,fr_wres,fr_hres,vsp,fsp,float_buffer);
} // TODO: make the resolution of framebuffers dynamic (cambased)

/*
	init(uint32_t,uint32_t,uint32_t,uint32_t,const char*,const char*,bool) -> void
	fr_width: width of frame
	fr_height: height of frame
	purpose: complete initialization of framebuffer objects
*/
void FrameBuffer::init(uint32_t fr_width,uint32_t fr_height,uint32_t fr_wres,uint32_t fr_hres,
		const char* vsp,const char* fsp,bool float_buffer)
{
	// setup
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glGenTextures(1,&tex);
	glGenRenderbuffers(1,&rbo);

	// buffer data
	std::vector<float> verts = Toolbox::create_sprite_canvas();
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	s.compile2d(vsp,fsp);

	// texture setup
	glBindTexture(GL_TEXTURE_2D,tex); // !!differentiation gives negative style points ??can it be fixed .relfrm
	if (float_buffer)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,fr_width,fr_height,0,GL_RGBA,GL_FLOAT,NULL);
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,fr_width,fr_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,0);

	// framebuffer texture
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex,0);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,fr_width,fr_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	bind() -> void
	purpose: binds the framebuffer object to read and save following draws until close()
*/
void FrameBuffer::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
}

/*
	render() -> void
	purpose: draw buffer in combination with given shader manipulations
*/
void FrameBuffer::render()
{
	// gl setup
	glActiveTexture(GL_TEXTURE0);

	// buffer & shader
	s.enable();
	buffer.bind();

	// draw
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArrays(GL_TRIANGLES,0,6);
}

/*
	render(float) -> void
	ptrans: progression of menu's transformation animation after start was pressed
	purpose: render menu transformation between two shader states
	DEVELOPED EXCLUSIVELY FOR : yomisensei
*/
void FrameBuffer::render(float ptrans)
{
	// setup
	glActiveTexture(GL_TEXTURE0); // !!please tidy this up
	s.enable();
	buffer.bind();
	glBindTexture(GL_TEXTURE_2D,tex);

	// render
	s.upload_vec2("fres",glm::vec2(frw,frh)); // !!do not do this in update
	s.upload_float("vgnt",0.44f+(float)(rand()%21)*0.001f);
	s.upload_float("ptrans",ptrans);
	glDrawArrays(GL_TRIANGLES,0,6);
}

/*
	get_fbo() -> GLuint
	returns: frame buffer object
*/
GLuint FrameBuffer::get_fbo()
{
	return fbo;
}

/*
	get_tex() -> GLuint
	returns: frame buffer texture
*/
GLuint FrameBuffer::get_tex()
{
	return tex;
}

/*
	close() -> void (static)
	purpose: unbinds any bound framebuffer
*/
void FrameBuffer::close()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}