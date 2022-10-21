#include "framebuffer.h"

FrameBuffer::FrameBuffer() {  } // !!how bout no

FrameBuffer::FrameBuffer(int fr_width,int fr_height,const char* vsp,
		const char* fsp,bool float_buffer)
	: frw(fr_width),frh(fr_height)
{
	init(fr_width,fr_height,fr_width,fr_height,vsp,fsp,float_buffer);
}

FrameBuffer::FrameBuffer(int fr_width,int fr_height,int fr_wres,int fr_hres,const char* vsp,
		const char* fsp,bool float_buffer)
	: frw(fr_width),frh(fr_height)
{
	init(fr_width,fr_height,fr_wres,fr_hres,vsp,fsp,float_buffer);
}
// TODO: make the resolution of framebuffers dynamic (cambased)

void FrameBuffer::init(int fr_width,int fr_height,int fr_wres,int fr_hres,const char* vsp,const char* fsp,
		bool float_buffer)
{
	// setup
	buffer = Buffer();
	buffer.bind();
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
	glGenTextures(1,&tex);
	glGenRenderbuffers(1,&rbo);

	// buffer data
	float verts[] = { // ??adding index buffer object later ??space or time
		-1.0f,1.0f,0.0f,1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f,-1.0f,1.0f,0.0f,	// first triangle
		-1.0f,1.0f,0.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f		// second triangle
	}; buffer.upload_vertices(verts,sizeof(verts));

	s.compile2d(vsp,fsp);

	// framebuffer texture
	glBindTexture(GL_TEXTURE_2D,tex); // !!differentiation gives negative style points ??can it be fixed .relfrm
	if (float_buffer) glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,fr_width,fr_height,0,GL_RGBA,GL_FLOAT,NULL);
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,fr_width,fr_height,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D,0);

	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex,0);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,fr_width,fr_height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	glBindFramebuffer(GL_FRAMEBUFFER,0); // !!very clean ??actually nessessary
}

void FrameBuffer::bind() { glBindFramebuffer(GL_FRAMEBUFFER,fbo); }

void FrameBuffer::close() { glBindFramebuffer(GL_FRAMEBUFFER,0); }

void FrameBuffer::render()
{
	glActiveTexture(GL_TEXTURE0); // !!should be standard => after cleanup obsolete
	s.enable();
	buffer.bind();
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArrays(GL_TRIANGLES,0,6);
}

void FrameBuffer::render_wOverlay(GLuint itex,float ptrans)
{
	// setup
	glActiveTexture(GL_TEXTURE0); // !!please tidy this up
	s.enable();
	buffer.bind();
	glBindTexture(GL_TEXTURE_2D,tex);
	glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,itex);

	// render
	s.upload_vec2("fres",glm::vec2(frw,frh)); // !!do not do this in update
	s.upload_float("vgnt",0.44f+(float)(rand()%21)*0.001f);
	s.upload_float("ptrans",ptrans);
	s.upload_int("splash",1);
	glDrawArrays(GL_TRIANGLES,0,6);
}

unsigned int FrameBuffer::get_fbo() { return fbo; }

unsigned int FrameBuffer::get_tex() { return tex; }