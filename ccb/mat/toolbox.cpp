#include "toolbox.h"

/*
	create_sprite_canvas() -> std::vector<float>
	returns: primitive canvas vertices without camera ready coordinate system and element draw
*/
std::vector<float> Toolbox::create_sprite_canvas()
{
	std::vector<float> out = {
		-1.0f,1.0f,0.0f,1.0f,-1.0f,-1.0f,0.0f,0.0f,1.0f,-1.0f,1.0f,0.0f,	// first triangle
		-1.0f,1.0f,0.0f,1.0f,1.0f,-1.0f,1.0f,0.0f,1.0f,1.0f,1.0f,1.0f		// second triangle
	};
	return out;
}

/*
	create_sprite_canvas(vec2,float,float) -> std::vector<float>
	pos: origin position of canvas
	width: vertex distanced width of canvas
	height: vertex distanced height of canvas
	returns: created canvas vertices to later base 2D object generation on
*/
std::vector<float> Toolbox::create_sprite_canvas(glm::vec2 pos,float width,float height)
{
	std::vector<float> out = {
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y,0.0f,1.0f
	};
	return out;
}

std::vector<float> Toolbox::create_sprite_canvas_triangled(glm::vec2 pos,float width,float height)
{
	std::vector<float> out = {
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y,0.0f,1.0f,
		pos.x,pos.y+height,0.0f,0.0f,
	};
	return out;
}

/*
	load_texture(GLuint,const char*) -> void
	tex: reference to be associated with texture value when bound
	path: path to file holding texture value
	purpose: load texture value, generate mipmap and associate it with given texture reference
*/
void Toolbox::load_texture(GLuint tex,const char* path)
{
	// setup
	int width,height;
	glBindTexture(GL_TEXTURE_2D,tex);

	// load texture data from source
#ifdef __WIN32__

	unsigned char* image = stbi_load(path,&width,&height,0,STBI_rgb_alpha);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	stbi_image_free(image);

#else

	unsigned char* image = SOIL_load_image(path,&width,&height,0,SOIL_LOAD_RGBA);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
	SOIL_free_image_data(image);

#endif

	// texture parameters & mipmap
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	generate_elements(uint16_t,std::vector<uint16_t>&) -> void
	i: objects index to use to rasterize element value generation
	ls: element list input to add generated elements to
	purpose: generate buffer elements based on object list index
*/
void Toolbox::generate_elements(uint16_t i,std::vector<unsigned int> &ls)
{
	ls.push_back(i*4);ls.push_back(i*4+1);ls.push_back(i*4+2);		// map first triangle
	ls.push_back(i*4+2);ls.push_back(i*4+3);ls.push_back(i*4+0);	// map second triangle
}