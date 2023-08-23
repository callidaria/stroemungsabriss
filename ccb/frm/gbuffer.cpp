#include "gbuffer.h"

/*
	constructor(float,float)
	w_res: g-buffer's x-axis resolution
	h_res: g-buffer's y-axis resolution
	purpose: create the g-buffer based on given resolution
*/
GBuffer::GBuffer(float w_res,float h_res)
{
	// generating framebuffer
	glGenFramebuffers(1,&buffer);
	glBindFramebuffer(GL_FRAMEBUFFER,buffer);

	// define colour component
	glGenTextures(1,&t_colour);
	glBindTexture(GL_TEXTURE_2D,t_colour);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,w_res,h_res,0,GL_RGBA,GL_UNSIGNED_BYTE,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,t_colour,0);
	// data pattern: vec4(colour.x,colour.y,colour.z,specular.rgb)

	// define position component
	glGenTextures(1,&t_position);
	glBindTexture(GL_TEXTURE_2D,t_position);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT1,GL_TEXTURE_2D,t_position,0);
	// data pattern: vec4(position.x,position.y,position.z,null)

	// define normal component
	glGenTextures(1,&t_normals);
	glBindTexture(GL_TEXTURE_2D,t_normals);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT2,GL_TEXTURE_2D,t_normals,0);
	// data pattern: vec4(normal.x,normal.y,normal.z,emission.rgb)

	// define physical based material component
	glGenTextures(1,&t_materials);
	glBindTexture(GL_TEXTURE_2D,t_materials);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA16F,w_res,h_res,0,GL_RGBA,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT3,GL_TEXTURE_2D,t_materials,0);
	// data pattern: vec4(metallic.rgb,roughness.rgb,ambient_occlusion.rgb,null)

	// define depth component
	glGenTextures(1,&t_depth);
	glBindTexture(GL_TEXTURE_2D,t_depth);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,w_res,h_res,0,GL_DEPTH_COMPONENT,
			GL_UNSIGNED_INT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,t_depth,0);

	// define drawable g-buffer components & close
	uint32_t g_components[CCB_GBUFFER_COMPONENT_COUNT]
		= { GL_COLOR_ATTACHMENT0,GL_COLOR_ATTACHMENT1,GL_COLOR_ATTACHMENT2,GL_COLOR_ATTACHMENT3 };
	glDrawBuffers(CCB_GBUFFER_COMPONENT_COUNT,g_components);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}