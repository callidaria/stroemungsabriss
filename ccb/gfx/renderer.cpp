#include "renderer.h"


/**
 * TODO: expand
*/

/*
	TODO
*/
Sprite::Sprite(glm::vec2 position,float width,float height,const char* texpath)
	: position(position),width(width),height(height),texpath(texpath)
{
	glGenTextures(1,&texture);
}


/**
 * TODO: expand
*/

Atlas::Atlas(glm::vec2 position,float width,float height,const char* texpath,
		uint8_t rows,uint8_t columns,uint8_t frames,uint8_t span)
	: position(position),width(width),height(height),texpath(texpath),
		rows(rows),columns(columns),frames(frames),span(span)
{
	glGenTextures(1,&texture);
}


/**
 * TODO: expand
*/

/*
	!O(1) /load -> (public)
	purpose: create renderer object to subsequently add 2D objects to and draw them
*/
Renderer::Renderer()
{
	sprite_buffer.add_buffer();
}

/*
	!O(1)m /+load -> (public)
	purpose: add new sprite to renderer
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	\param t: path to file containing sprite texture
	\returns: memory index the sprite can be referenced by later
*/
uint16_t Renderer::add_sprite(glm::vec2 p,float w,float h,const char* t)
{
	sprites.push_back(Sprite(p,w,h,t));
	return sprites.size()-1;
}

/*
	!O(1)m /+load -> (public)
	purpose: add new spritesheet to renderer
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	\param t: path to file containing spritesheet
	\param r: rows on spritesheet
	\param c: columns on spritesheet
	\param f: number of frames held by spritesheet
	\param s: frames the animation takes to fully iterate through all textures
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t Renderer::add_sprite(glm::vec2 p,float w,float h,const char* t,uint8_t r,uint8_t c,uint8_t f,uint8_t s)
{
	atlas.push_back(Atlas(p,w,h,t,r,c,f,s));
	return atlas.size()-1;
}

/*
	!O(n)m .amount of added objects /load -> (public)
	purpose: combine vertex and texture loading and compile shader program
*/
void Renderer::load()
{
	// memory allocation for vertices and elements
	size_t t_vsize = sprite_vertices.size(), t_esize = sprite_elements.size();
	sprite_vertices.resize(t_vsize+(sprites.size()+atlas.size())*PATTERN_SPRITE_VERTEX_REPEAT);
	sprite_elements.resize(t_esize+(sprites.size()+atlas.size())*PATTERN_SPRITE_ELEMENT_REPEAT);

	// write sprite vertex values to upload list
	size_t i_velem = t_vsize/PATTERN_SPRITE_LOAD_REPEAT;
	for (Sprite& s : sprites) {
		Toolbox::create_sprite_canvas(sprite_vertices,t_vsize,s.position,s.width,s.height);
		Toolbox::generate_elements(t_esize,i_velem,sprite_elements);
	}

	// write animation vertex values to upload list
	for (Atlas& a : atlas) {
		Toolbox::create_sprite_canvas(sprite_vertices,t_vsize,a.position,a.width,a.height);
		Toolbox::generate_elements(t_esize,i_velem,sprite_elements);
	}

	// upload to buffers
	sprite_buffer.bind();
	sprite_buffer.upload_vertices(sprite_vertices), sprite_buffer.upload_elements(sprite_elements);

	// compile shader
	sprite_shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");

	// load textures
	for (Sprite& s : sprites) Toolbox::load_texture(s.texture,s.texpath);
	for (Atlas& a : atlas) Toolbox::load_texture(a.texture,a.texpath);
	sprite_shader.upload_int("tex",0);

	// coordinate system
	sprite_shader.upload_camera();
}

/*
	!O(1) /+function -> (public)
	purpose: prepare shader and buffer to render sprites and animated spritesheets
*/
void Renderer::prepare_sprites()
{
	sprite_shader.enable();
	sprite_buffer.bind();
}

/*
	!O(1) /update -> (public)
	purpose: draw indexed sprite
	\param i: sprite index
*/
void Renderer::render_sprite(uint16_t i)
{
	sprite_shader.upload_matrix("model",sprites[i].model);
	glBindTexture(GL_TEXTURE_2D,sprites[i].texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
}

/*
	!O(1) /update -> (public)
	purpose: same basic functionality as the normal render_sprite but with previous texture overwrite
	\param i: sprite index
	\param tex: texture to overwrite the normal sprite texture before render
*/
void Renderer::render_sprite_overwritten(uint16_t i,uint32_t tex)
{
	sprite_shader.upload_matrix("model",sprites[i].model);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
}

/*
	!O(1) /update -> (public)
	purpose: permanently render a specific frame of a spritesheet
	\param i: spritesheet index
	\param pos: two-dimensional index of spritesheet frame to draw
*/
void Renderer::render_sprite_frame(uint16_t i,glm::vec2 pos)
{
	// shader upload
	sprite_shader.upload_int("row",atlas[i].rows);
	sprite_shader.upload_int("col",atlas[i].columns);
	sprite_shader.upload_vec2("i_tex",pos);
	sprite_shader.upload_matrix("model",atlas[i].model);

	// draw sprite
	glBindTexture(GL_TEXTURE_2D,atlas[i].texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sprites.size())*6*sizeof(uint32_t)));
}

/*
	!O(1) /update -> (public)
	purpose: animate spritesheet and automatically iterate through all contained frames
	\param i: spritesheet index
*/
void Renderer::render_sprite_animated(uint16_t i)
{
	glBindTexture(GL_TEXTURE_2D,atlas[i].texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sprites.size())*6*sizeof(uint32_t)));
}
