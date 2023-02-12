#include "toolbox.h"

/*
	calculate_vecangle(vec2,vec2) -> float (static)
	a: first vector, 0 degrees towards the origin
	b: second vector, returned degrees from first vector
	returns: angle between vectors in radians
*/
float Toolbox::calculate_vecangle(glm::vec2 a,glm::vec2 b)
{
	return glm::acos(glm::dot(a,b)/(glm::length(a)*glm::length(b)));
}

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
	PARAMETER DEFINITIONS:
	pos: origin position of canvas
	width: vertex distanced width of canvas
	height: vertex distanced height of canvas
*/

/*
	create_sprite_canvas(vec2,float,float) -> std::vector<float> (static)
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

/*
	create_sprite_canvas_triangled(vec2,float,float) -> std::vector<float> (static)
	returns: the same canvas as create_sprite_canvas, but without relying on element buffer
*/
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
	PARAMETER DEFINITIONS:
	tex: reference to be associated with texture value when bound
	path: path to file holding texture value
*/

/*
	load_texture(GLuint,const char*) -> void (static)
	purpose: load texture value, generate mipmap and associate it with given texture reference
*/
void Toolbox::load_texture(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture(GLuint,const char*,float) -> void (static)
	overloads previous load_texture()
	bias: mipmapping level-of-detail bias
	purpose: load mipmapped texture with modified lod bias
*/
void Toolbox::load_texture(GLuint tex,const char* path,float bias)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture parameters & mipmap
	set_texture_parameter_clamp_to_edge();
	set_texture_parameter_linear_mipmap();
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,bias);
	glGenerateMipmap(GL_TEXTURE_2D);
}

/*
	load_texture_unfiltered(GLuint,const char*) -> void (static)
	purpose: load texture without mipmapping
*/
void Toolbox::load_texture_unfiltered(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture paramteres without mipmap
	set_texture_parameter_clamp_to_edge();
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	load_texture_repeat(GLuint,const char*) -> void (static)
	purpose: load repeating mipmapped texture
*/
void Toolbox::load_texture_repeat(GLuint tex,const char* path)
{
	// bind and load texture data
	load_texture_function_head(tex,path);

	// texture parameters repeating
	set_texture_parameter_texture_repeat();
	set_texture_parameter_linear_mipmap();
	glGenerateMipmap(GL_TEXTURE_2D);
}
// TODO: break edge cases apart and let the statii be determined separately with additional methods

/*
	generate_elements(uint16_t,std::vector<uint16_t>&) -> void (static)
	i: objects index to use to rasterize element value generation
	ls: element list input to add generated elements to
	purpose: generate buffer elements based on object list index
*/
void Toolbox::generate_elements(uint16_t i,std::vector<unsigned int> &ls)
{
	ls.push_back(i*4);ls.push_back(i*4+1);ls.push_back(i*4+2);		// map first triangle
	ls.push_back(i*4+2);ls.push_back(i*4+3);ls.push_back(i*4+0);	// map second triangle
}

/*
	set_texture_parameter_clamp_to_edge() -> void (private,static)
	purpose: define texture as to be stretched out towards the borders
*/
void Toolbox::set_texture_parameter_clamp_to_edge()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
}

/*
	set_texture_parameter_texture_repeat() -> void (private,static)
	purpose: define texture as repeatable
*/
void Toolbox::set_texture_parameter_texture_repeat()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_REPEAT);
}

/*
	set_texture_parameter_linear_mipmap() -> void (private,static)
	purpose: define texture as mipmappable
*/
void Toolbox::set_texture_parameter_linear_mipmap()
{
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	load_texture_function_head(GLuint tex,const char* path) -> void (private,static)
	purpose: load texture value from given file
*/
void Toolbox::load_texture_function_head(GLuint tex,const char* path)
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
}