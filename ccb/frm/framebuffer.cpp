#include "framebuffer.h"

/*
	constructor(uint32_t,uint32_t,const char*,const char*,bool) !O(1)b
	purpose: creates framebuffer object to change original visuals through shaders
	\param fr_width: width of frame, in this case identical to frame resolution width
	\param fr_height: height of frame, in this case identical to frame resolution height
	\param vsp: path to vertex shader file
	\param fsp: path to fragment shader file
*/
FrameBuffer::FrameBuffer(uint32_t fr_width,uint32_t fr_height,const char* vsp,
		const char* fsp,bool float_buffer,bool depth_buffer)
	: frw(fr_width),frh(fr_height)
{
	// setup
	glGenFramebuffers(1,&fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,fbo);

	// buffer data
	std::vector<float> verts = Toolbox::create_sprite_canvas();
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	s.compile2d(vsp,fsp);

	// setup colour texture
	glGenTextures(1,&tex);
	glBindTexture(GL_TEXTURE_2D,tex);
	if (float_buffer)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,fr_width,fr_height,0,GL_RGBA,GL_FLOAT,NULL);
	else glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,fr_width,fr_height,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	Toolbox::set_texture_parameter_linear_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,tex,0);
	// FIXME: kill branch by switching between floatbuffer setup by index math

	// setup depth map
	if (depth_buffer) {
		glGenTextures(1,&dptex);
		glBindTexture(GL_TEXTURE_2D,dptex);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,fr_width,fr_height,0,GL_DEPTH_COMPONENT,
				GL_UNSIGNED_INT,NULL);
		Toolbox::set_texture_parameter_nearest_unfiltered();
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,dptex,0);
	}
}
// TODO: make the resolution of framebuffers dynamic (cambased)

/*
	prepare() -> void
	purpose: prepare framebuffer's shader and buffer for any purpose
*/
void FrameBuffer::prepare()
{
	s.enable();
	buffer.bind();
}

/*
	render() -> void
	purpose: draw buffer in combination with given shader manipulations
*/
void FrameBuffer::render()
{
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArrays(GL_TRIANGLES,0,6);
}

/*
	render(float) -> void
	ptrans: progression of menu's transformation animation after start was pressed
	purpose: render menu transformation between two shader states
	DEVELOPED EXCLUSIVELY FOR : yomisensei
*/
void FrameBuffer::render(float mtransition)
{
	// setup
	s.enable();
	buffer.bind();
	glBindTexture(GL_TEXTURE_2D,tex);

	// render
	s.upload_vec2("ratio",glm::vec2(frw,frh)); // !!do not do this in update
	s.upload_float("vignette",.44f+(float)(rand()%21)*.001f);
	s.upload_float("mtransition",mtransition);
	glDrawArrays(GL_TRIANGLES,0,6);
}
// FIXME: an extra implementation for this seems a little unelegant
