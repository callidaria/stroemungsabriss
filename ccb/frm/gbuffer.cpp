#include "gbuffer.h"

/*
	TODO
*/
GBuffer::GBuffer(float w_res,float h_res)
{
	// generating buffer & textures
	glGenFramebuffers(1,&buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,buffer);
	glGenTextures(1,&t_col);
	glGenTextures(1,&t_pos);
	glGenTextures(1,&t_norm);
	glGenRenderbuffers(1,&rb_depth);

	// define colour component
	glBindTexture(GL_TEXTURE_2D,t_col);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w_res,h_res,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,t_col,0);

	// define position component
	glBindTexture(GL_TEXTURE_2D,t_pos);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,t_pos,0);

	// define normal component
	glBindTexture(GL_TEXTURE_2D,t_norm);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,t_norm,0);

	// define depth component
	/*glBindRenderbuffer(GL_RENDERBUFFER,rb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,w_res,h_res);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rb_depth);*/

	// compile buffer
	uint32_t g_components[3] = { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3,g_components);

	// close gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	TODO
*/
void GBuffer::bind()
{ glBindFramebuffer(GL_FRAMEBUFFER,buffer); }

/*
	TODO
*/
uint32_t GBuffer::get_colour()
{ return t_col; }
uint32_t GBuffer::get_position()
{ return t_pos; }
uint32_t GBuffer::get_normals()
{ return t_norm; }