#include "msaa.h"

/*
	constructor()
	purpose: creates a placeholder object for multisampling
*/
MSAA::MSAA() {  }

/*
	constructor(const char*,const char*,uint32_t,uint32_t,int)
	vsp: target vertex shader path
	fsp: target fragment shader path
	bw: renderbuffer width
	bh: renderbuffer height
	los: level of multisampling
	purpose: create an object for multisampling
*/
MSAA::MSAA(const char* vsp,const char* fsp,uint16_t bw,uint16_t bh,int los)
	: fbw(bw),fbh(bh)
{
	// buffer upload
	std::vector<float> verts = Toolbox::create_sprite_canvas();
	buffer.bind();
	buffer.upload_vertices(verts);

	// canvas creation, buffer upload and shader compilation
	sfb.compile2d(vsp,fsp);

	// generate and bind read framebuffer
	glGenFramebuffers(1,&rfbo);
	glBindFramebuffer(GL_FRAMEBUFFER,rfbo);

	// generate multisampled colour buffer
	glGenTextures(1,&colbuffer);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,colbuffer);
	glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE,los,GL_RGBA,fbw,fbh,GL_TRUE);
	glBindTexture(GL_TEXTURE_2D_MULTISAMPLE,0);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D_MULTISAMPLE,
			colbuffer,0);

	// renderbuffer creation and multisampling settings
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	glRenderbufferStorageMultisample(GL_RENDERBUFFER,los,GL_DEPTH24_STENCIL8,fbw,fbh);
	glBindFramebuffer(GL_RENDERBUFFER,0);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);

	// generate and bind write framebuffer
	glGenFramebuffers(1,&wfbo);
	glBindFramebuffer(GL_FRAMEBUFFER,wfbo);

	// generate screen buffer
	glGenTextures(1,&scrbuffer);
	glBindTexture(GL_TEXTURE_2D,scrbuffer);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fbw,fbh,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,scrbuffer,0);
}

/*
	bind() -> void
	purpose: bind multisampling framebuffer
*/
void MSAA::bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER,rfbo);
}

/*
	blit() -> void
	purpose: blit framebuffers, read from multisample buffer, write to screen buffer
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
	purpose: render multisampled buffer to msaa canvas
*/
void MSAA::render()
{
	prepare();
	glDrawArrays(GL_TRIANGLES,0,6);
}

/*
	render(GLuint) -> void
	ovltex: overlay texture to be combined with base texture
	purpose: render base combined with overlay texture if base texture has colour length > 0
*/
void MSAA::render(GLuint ovltex)
{
	// prepare & upload overlay texture
	prepare();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,ovltex);
	sfb.upload_int("overlay",1);

	// draw
	glDrawArrays(GL_TRIANGLES,0,6);
	glActiveTexture(GL_TEXTURE0);
}

/*
	get_buffer() -> GLuint
	returns: screen buffer holding current multisampled frame
*/
GLuint MSAA::get_buffer()
{
	return scrbuffer;
}

/*
	prepare() -> void
	purpose: used to prepare all render methods
*/
void MSAA::prepare()
{
	// buffer & shader
	sfb.enable();
	buffer.bind();

	// gl setup
	glDisable(GL_DEPTH_TEST);

	// texture upload
	glBindTexture(GL_TEXTURE_2D,get_buffer());
}