#include "gbuffer.h"

/*
	constructor(float,float)
	w_res: g-buffer's x-axis resolution
	h_res: g-buffer's y-axis resolution
	purpose: create the g-buffer based on given resolution
*/
GBuffer::GBuffer(float w_res,float h_res)
{
	// generating buffer & textures
	glGenFramebuffers(1,&buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,buffer);
	glGenTextures(1,&t_col);
	glGenTextures(1,&t_pos);
	glGenTextures(1,&t_norm);
	glGenTextures(1,&t_pbm);
	glGenRenderbuffers(1,&rb_depth);

	// define colour component
	glBindTexture(GL_TEXTURE_2D,t_col);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w_res,h_res,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,t_col,0);
	// data pattern: vec4(colour.x,colour.y,colour.z,specular.rgb)

	// define position component
	glBindTexture(GL_TEXTURE_2D,t_pos);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,t_pos,0);
	// data pattern: vec4(position.x,position.y,position.z,null)

	// define normal component
	glBindTexture(GL_TEXTURE_2D,t_norm);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,t_norm,0);
	// data pattern: vec4(normal.x,normal.y,normal.z,emission.rgb)

	// define physical based material component
	glBindTexture(GL_TEXTURE_2D,t_pbm);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT3,GL_TEXTURE_2D,t_pbm,0);
	// data pattern: vec4(metallic.rgb,roughness.rgb,ambient_occlusion.rgb,null)

	// define depth component
	glBindRenderbuffer(GL_RENDERBUFFER,rb_depth);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,w_res,h_res);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rb_depth);

	// compile buffer
	uint32_t g_components[4]
		= { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(4,g_components);

	// close gbuffer
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	bind() -> void
	purpose: bind the g-buffer
*/
void GBuffer::bind()
{ glBindFramebuffer(GL_FRAMEBUFFER,buffer); }

/*
	get_<component>() -> uint32_t
	returns: desired g-buffer component as texture
*/
uint32_t GBuffer::get_colour()
{ return t_col; }
uint32_t GBuffer::get_position()
{ return t_pos; }
uint32_t GBuffer::get_normals()
{ return t_norm; }
uint32_t GBuffer::get_materials()
{ return t_pbm; }