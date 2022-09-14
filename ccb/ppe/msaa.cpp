#include "msaa.h"

MSAA::MSAA() {  }

MSAA::MSAA(const char* vsp,const char* fsp,uint32_t bw,uint32_t bh,int los)
	: fbw(bw),fbh(bh)
{
	// create and upload canvas vertices
	float verts[] = {
		-1.0f,1.0f,0.0f,1.0f,
		-1.0f,-1.0f,0.0f,0.0f,
		1.0f,-1.0f,1.0f,0.0f,
		-1.0f,1.0f,0.0f,1.0f,
		1.0f,-1.0f,1.0f,0.0f,
		1.0f,1.0f,1.0f,1.0f
	};  // FIXME: streamline this mess
	buffer.bind();
	buffer.upload_vertices(verts,sizeof(verts));

	// compile shader program
	sfb.compile2d(vsp,fsp);

	// generate and bind buffer
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	// multisample texture creation and settings
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,tex);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,los,GL_RGB,fbw,fbh,GL_TRUE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);

	// set multisample texture as framebuffer texture
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,tex,0);

	// renderbuffer creation and multisampling settings
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,los,GL_DEPTH24_STENCIL8,fbw,fbh);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void MSAA::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);
}

void MSAA::blit(FrameBuffer* fb)
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fb->get_fbo());
	glBlitFramebuffer(0,0,fbw,fbh,0,0,fbw,fbh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
}

void MSAA::render()
{
	// prepare
	sfb.enable();
	buffer.bind();

	// texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex);

	// draw
	glDrawArrays(GL_TRIANGLES,0,6);
}

void MSAA::close()
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}