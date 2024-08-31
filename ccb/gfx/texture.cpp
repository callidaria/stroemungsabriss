#include "texture.h"


/**
 *	TODO
*/

/*
	TODO
*/
Texture::Texture(std::string path,bool corrected)
	: m_path(path)
{
	m_format = GL_RGBA+corrected*0x7338;
}

/*
	TODO
*/
void Texture::load()
{
	int32_t width = 0,height = 0;
	m_data = stbi_load(m_path.c_str(),&m_width,&m_height,0,STBI_rgb_alpha);
}

/*
	TODO
*/
void Texture::upload()
{
	// upload new texture to gpu
	glGenTextures(1,&texture);
	bind();
	glTexImage2D(GL_TEXTURE_2D,0,m_format,m_width,m_height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_data);

	// cleanup pixel data
	stbi_image_free(m_data);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as mipmappable
*/
void Texture::set_texture_parameter_linear_mipmap()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as simply linearly filtered
*/
void Texture::set_texture_parameter_linear_unfiltered()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as unfiltered
*/
void Texture::set_texture_parameter_nearest_unfiltered()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
}

/*
	!O(1) /+load -> (static)
	purpose: define filtered cubemap texture
*/
void Texture::set_cubemap_texture_parameters()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	!O(1) /+load -> (static)
	purpose: define filtered cubemap with multiple levels of detail
*/
void Texture::set_cubemap_texture_parameters_mipmap()
{
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as to be stretched out towards the edge
*/
void Texture::set_texture_parameter_clamp_to_edge()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as to be scaled up towards custom borders, to avoid ratio manipulation
*/
void Texture::set_texture_parameter_clamp_to_border()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as repeatable
*/
void Texture::set_texture_parameter_repeat()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}


/**
 *	TODO
*/

/*
	TODO
*/
FrameBuffer::FrameBuffer(uint8_t n_components)
{
	// generate framebuffer
	glGenFramebuffers(1,&m_buffer);

	// generate framebuffer textures
	m_colour_components.reserve(n_components);
	glGenTextures(n_components,&m_colour_components[0]);
}

/*
	TODO
*/
void FrameBuffer::add_colour_component(float width,float height,bool floatbuffer,uint8_t component)
{
	uint32_t cmp = GL_COLOR_ATTACHMENT0+component;
	glBindTexture(GL_TEXTURE_2D,m_colour_components[component]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA+floatbuffer*0x6f12,width,height,0,GL_RGBA,
			GL_UNSIGNED_INT+floatbuffer,NULL);
	Texture::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,cmp,GL_TEXTURE_2D,m_colour_components[component],0);
}

/*
	TODO
*/
void FrameBuffer::add_depth_component(float width,float height)
{
	// generate and bind depthbuffer texture
	uint32_t t_depth_component;
	glGenTextures(1,&t_depth_component);
	glBindTexture(GL_TEXTURE_2D,t_depth_component);

	// specify depthbuffer
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_INT,NULL);
	Texture::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,t_depth_component,0);
}

/*
	TODO
*/
void FrameBuffer::combine_attachments()
{
	uint32_t attachments[m_colour_components.size()];
	for (uint8_t i=0;i<m_colour_components.size();i++) attachments[i] = GL_COLOR_ATTACHMENT0+i;
	glDrawBuffers(m_colour_components.size(),attachments);
}

/*
	TODO
*/
void FrameBuffer::upload_components()
{
	for (uint8_t i=0;i<m_colour_components.size();i++)
	{
		glActiveTexture(GL_TEXTURE0+i);
		glBindTexture(GL_TEXTURE_2D,m_colour_components[i]);
	}
	glActiveTexture(GL_TEXTURE0);
}
