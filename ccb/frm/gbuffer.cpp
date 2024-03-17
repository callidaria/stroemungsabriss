#include "gbuffer.h"

/*
	!O(1) /load -> (public)
	purpose: create a g-buffer based on the given resolution
	\param wres: g-buffer's x-axis resolution
	\param hres: g-buffer's y-axis resolution
	NOTE: constructing will immediately bind framebuffer for setup
*/
GBuffer::GBuffer(float wres,float hres)
	: w_res(wres),h_res(hres)
{
	// generating framebuffer
	glGenFramebuffers(1,&buffer);
	bind();
}

/*
	!O(1)m /function -> (public)
	purpose: register new colour component for gbuffer
	\param fcomp: if registered component is float buffer component
	NOTE: buffer has to be bound beforehand
*/
void GBuffer::add_colour_component(bool fcomp)
{
	// define component
	uint32_t t_component,def_component = GL_COLOR_ATTACHMENT0+t_colour_components.size();
	glGenTextures(1,&t_component);
	glBindTexture(GL_TEXTURE_2D,t_component);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA+fcomp*0x6F12,w_res,h_res,0,GL_RGBA,
			GL_UNSIGNED_INT+fcomp,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,def_component,GL_TEXTURE_2D,t_component,0);
	t_colour_components.push_back(t_component),def_colour_components.push_back(def_component);
}

/*
	!O(1)m /function -> (public)
	purpose: register new depth component for gbuffer
	\returns memory index of added depth value component
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
	!O(1) /load -> (public)
	purpose: load all added components to finalize gbuffer definition
	NOTE: buffer will be unbound automatically
*/
void GBuffer::finalize_buffer()
{
	glDrawBuffers(def_colour_components.size(),&def_colour_components[0]);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}
