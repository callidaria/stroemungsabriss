#include "msaa.h"

/*
	constructor()
	[...]
*/
MSAA::MSAA() {  }

/*
	constructor(const char*,const char*,uint32_t,uint32_t,int)
	[...]
*/
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

	// buffer upload and shader compilation
	buffer.bind();
	buffer.upload_vertices(verts,sizeof(verts));
	sfb.compile2d(vsp,fsp);

	// generate and bind read framebuffer
	glGenFramebuffers(1,&rfbo);
	glBindFramebuffer(GL_FRAMEBUFFER,rfbo);

	// generate multisampled colour buffer
	glGenTextures(1,&colbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,colbuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,los,GL_RGB,fbw,fbh,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,
			colbuffer,0);

	// renderbuffer creation and multisampling settings
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,los,GL_DEPTH24_STENCIL8,fbw,fbh);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);

	// generate and bind write framebuffer
	glGenFramebuffers(1,&wfbo);
	glBindFramebuffer(GL_FRAMEBUFFER,wfbo);

	// generate screen buffer
	glGenTextures(1,&scrbuffer);
	glBindTexture(GL_TEXTURE_2D,scrbuffer);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,fbw,fbh,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,scrbuffer,0);
}

/*
	bind() -> void
	[...]
*/
void MSAA::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,rfbo);
}

/*
	blit() -> void
	[...]
*/
void MSAA::blit()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER,rfbo);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,wfbo);
	glBlitFramebuffer(0,0,fbw,fbh,0,0,fbw,fbh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	render() -> void
	[...]
*/
void MSAA::render()
{
	// prepare
	sfb.enable();
	buffer.bind();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,scrbuffer);

	// draw
	glDrawArrays(GL_TRIANGLES,0,6);
}