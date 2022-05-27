#pragma once

#include <iostream>
#include "../frm/framebuffer.h"
#include "../gfx/shader.h"
#include "../fcn/buffer.h"

class MSAA
{
public:
	MSAA() {  }
	MSAA(const char* vsp,const char* fsp,uint32_t bw,uint32_t bh,int los) // !!make resolution dynamic
		: fbw(bw),fbh(bh)
	{
		float verts[] = {
			-1.0f,1.0f,0.0f,1.0f,
			-1.0f,-1.0f,0.0f,0.0f,
			1.0f,-1.0f,1.0f,0.0f,
			-1.0f,1.0f,0.0f,1.0f,
			1.0f,-1.0f,1.0f,0.0f,
			1.0f,1.0f,1.0f,1.0f
		};
		buffer = Buffer();
		buffer.bind();
		buffer.upload_vertices(verts,sizeof(verts));
		sfb.compile2d(vsp,fsp);
		glGenFramebuffers(1,&fbo);glBindFramebuffer(GL_FRAMEBUFFER,fbo);
		glGenTextures(1,&tex);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,tex);
		glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,los,GL_RGB,fbw,fbh,GL_TRUE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,tex,0);
		glGenRenderbuffers(1,&rbo);
		glBindRenderbuffer(GL_RENDERBUFFER,rbo);
		glRenderbufferStorageMultisample(GL_RENDERBUFFER,los,GL_DEPTH24_STENCIL8,fbw,fbh);
		glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);
		glBindFramebuffer(GL_FRAMEBUFFER,0);
	}
	void bind() { glBindFramebuffer(GL_FRAMEBUFFER,fbo); }
	void blit(FrameBuffer* fb)
	{
		glBindFramebuffer(GL_READ_FRAMEBUFFER,fbo);
		glBindFramebuffer(GL_DRAW_FRAMEBUFFER,fb->get_fbo());
		glBlitFramebuffer(0,0,fbw,fbh,0,0,fbw,fbh,GL_COLOR_BUFFER_BIT,GL_NEAREST);
	}
	void close() { glBindFramebuffer(GL_FRAMEBUFFER,0); }
	void render()
	{
		glActiveTexture(GL_TEXTURE0);
		sfb.enable();
		buffer.bind();
		glBindTexture(GL_TEXTURE_2D,tex);
		glDrawArrays(GL_TRIANGLES,0,6);
	}
	unsigned int get_tex() { return tex; }
private:
	Shader sfb;
	uint32_t fbw,fbh;
	unsigned int tex,rbo;
	Buffer buffer;
	unsigned int fbo;
};
