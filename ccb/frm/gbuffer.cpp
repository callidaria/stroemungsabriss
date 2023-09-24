#include "gbuffer.h"

/*
	constructor(float,float)
	w_res: g-buffer's x-axis resolution
	h_res: g-buffer's y-axis resolution
	purpose: create a g-buffer based on the given resolution
	NOTE: contructing will immediately bind framebuffer for setup
*/
GBuffer::GBuffer(float wres,float hres)
	: w_res(wres),h_res(hres)
{
	// generating framebuffer
	glGenFramebuffers(1,&buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,buffer);
}

/*
	TODO
	NOTE: buffer has to be bound beforehand
*/
void GBuffer::add_colour_component(bool fcomp)
{
	// define component
	uint32_t t_component;
	glGenTextures(1,&t_component);
	glBindTexture(GL_TEXTURE_2D,t_component);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA+fcomp*0x6F12,w_res,h_res,0,GL_RGBA,
			GL_UNSIGNED_INT+fcomp,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0+t_colour_components.size(),
			GL_TEXTURE_2D,t_component,0);
	t_colour_components.push_back(t_component);
}

/*
	TODO
	NOTE: buffer has to be bound beforehand
*/
uint8_t GBuffer::add_depth_component()
{
	uint32_t t_depth;
	glGenTextures(1,&t_depth);
	glBindTexture(GL_TEXTURE_2D,t_depth);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,w_res,h_res,0,GL_DEPTH_COMPONENT,
			GL_UNSIGNED_INT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,t_depth,0);
	t_value_components.push_back(t_depth);
	return t_value_components.size()-1;
}

/*
	TODO
	NOTE: buffer will be unbound automatically
*/
void GBuffer::finalize_buffer()
{
	for (uint8_t i=0;i<t_colour_components.size();i++) glDrawBuffer(GL_COLOR_ATTACHMENT0+i);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
