#include "texture.h"


/**
 *	TODO
*/

/*
	TODO
*/
Texture::Texture(std::string path,bool corrected)
	: m_Path(path)
{
	m_Format = GL_RGBA+corrected*0x7338;
}

/*
	TODO
*/
void Texture::load()
{
	m_Data = stbi_load(m_Path.c_str(),&m_Width,&m_Height,0,STBI_rgb_alpha);
}

/*
	TODO
*/
void Texture::upload()
{
	// upload new texture to gpu
	glGenTextures(1,&texture);
	bind();
	glTexImage2D(GL_TEXTURE_2D,0,m_Format,m_Width,m_Height,0,GL_RGBA,GL_UNSIGNED_BYTE,m_Data);

	// cleanup pixel data
	stbi_image_free(m_Data);
}

/*
	!O(1) /+load -> (static)
	purpose: define texture as mipmappable
*/
void Texture::set_texture_parameter_linear_mipmap()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
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

/*
	TODO
*/
void Texture::set_texture_border_colour(glm::vec4 colour)
{
	float border_colour[4] = { colour.x,colour.y,colour.z,colour.a };
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,border_colour);
}


/**
 *	TODO
*/

/*
	TODO
*/
void Glyph::upload()
{
	glGenTextures(1,&texture);
	bind();
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,width,height,0,GL_RED,GL_UNSIGNED_BYTE,buffer);
}


/**
 *	TODO
*/

/*
	TODO
*/
FrameBuffer::FrameBuffer()
{
	glGenFramebuffers(1,&m_Buffer);
}

/*
	TODO
*/
FrameBuffer::FrameBuffer(uint8_t n_components)
{
	// generate framebuffer
	glGenFramebuffers(1,&m_Buffer);

	// generate framebuffer textures
	m_ColourComponents.resize(n_components);
	glGenTextures(n_components,&m_ColourComponents[0]);
}

/*
	TODO
*/
void FrameBuffer::add_colour_component(float width,float height,bool floatbuffer,uint8_t component)
{
	uint32_t t_Component = GL_COLOR_ATTACHMENT0+component;
	glBindTexture(GL_TEXTURE_2D,m_ColourComponents[component]);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA+floatbuffer*0x6f12,width,height,0,GL_RGBA,
			GL_UNSIGNED_INT+floatbuffer,NULL);
	Texture::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,t_Component,GL_TEXTURE_2D,m_ColourComponents[component],0);
}

/*
	TODO
*/
void FrameBuffer::add_depth_component(float width,float height)
{
	// generate and bind depthbuffer texture
	glGenTextures(1,&m_DepthComponent);
	glBindTexture(GL_TEXTURE_2D,m_DepthComponent);

	// specify depthbuffer
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,width,height,0,GL_DEPTH_COMPONENT,GL_UNSIGNED_INT,NULL);
	Texture::set_texture_parameter_nearest_unfiltered();
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,m_DepthComponent,0);
}

/*
	TODO
*/
void FrameBuffer::combine_attachments()
{
	uint32_t t_Attachments[m_ColourComponents.size()];
	for (uint8_t i=0;i<m_ColourComponents.size();i++) t_Attachments[i] = GL_COLOR_ATTACHMENT0+i;
	glDrawBuffers(m_ColourComponents.size(),t_Attachments);
}
