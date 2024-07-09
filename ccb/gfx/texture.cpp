#include "texture.h"

/*
	TODO
*/
Texture::Texture(const char* path,bool corrected)
{
	// setup
	image.format = GL_RGBA+corrected*0x7338;
	glGenTextures(1,&texture);

	// load texture data from source
	image.data = stbi_load(path,&image.width,&image.height,0,STBI_rgb_alpha);
}

/*
	TODO
*/
void Texture::gpu_upload()
{
	bind();
	glTexImage2D(GL_TEXTURE_2D,0,image.format,image.width,image.height,0,GL_RGBA,GL_UNSIGNED_BYTE,image.data);
}

/*
	TODO
*/
void Texture::cleanup()
{
	stbi_image_free(image.data);
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
