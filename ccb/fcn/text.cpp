#include "text.h"

/*
	TODO
*/
Text::Text()
{
	buffer.add_buffer();
	glGenTextures(1,&tex);
}

/*
	constructor(Font*)
	f: pointer to font, holding the .fnt and texture that are going to be used when rendering text
	purpose: create an entity to later add text and characters to
	WARNING: the created objects always have to lead with this contructor, else font texture breaks
*/
Text::Text(Font f)
	: font(f)
{
	buffer.add_buffer();
	glGenTextures(1,&tex);
}

/*
	add(char,vec2) -> int32_t
	c: character to add to the text entity
	p: position to add the character at
	returns: x-axis modification of cursor position after adding the given character
	purpose: add a single character to the text entity
*/
int32_t Text::add(char c,glm::vec2 p) // !!passing x increment like this is very bad pracice with public method
{
	// identifying sprite sheet position
	int i = 0;
	while (i<96) { // ??maybe alternate iteration until correct index that is more performant
		if (font.id[i]==(int)c) break;
		i++;
	}

	// character information write
	ibv.push_back(p.x);ibv.push_back(p.y);
	ibv.push_back(font.x[i]);ibv.push_back(font.y[i]);
	ibv.push_back(font.wdt[i]);ibv.push_back(font.hgt[i]);
	ibv.push_back(font.xo[i]);ibv.push_back(font.yo[i]);

	return font.xa[i]*(font.mw/83.0f);
	// ??do this with a vec2 pointer maybe & also with dynamic texdiv
}

/*
	add(const char*,vec2) -> void
	s: character array, also primitive string, to add to text entity
	p: position to add string to text entity at
	purpose: add a character array or primitive string to text entity at given position
*/
void Text::add(const char* s,glm::vec2 p)
{
	for (int i=0;i<strlen(s);i++) {
		if (s[i]!=' ') p.x += add(s[i],p);
		else p.x += 57.0f*(font.mw/83.0f);
	}
}

/*
	clear() -> void
	purpose: clear all character entries from text entity index buffer
*/
void Text::clear()
{ ibv.clear(); }

/*
	texture() -> void
	purpose: load font texture from pointed texture path
	WARNING: somehow it is necessary to use this additionally to the constructor when Text()
		was the first construction usage. yeah don't ask me why i don't understand this shit either
*/
void Text::texture()
{ Toolbox::load_texture(tex,font.tp); }

/*
	load() -> void
	c: camera and mainly coordinate system to render text vertices in relation to
	purpose: upload to buffer as well as compile and setup shader
*/
void Text::load()
{
	// setup
	load_vertex();
	sT.compile2d("shader/vertex_text.shader","shader/fragment_text.shader");
	buffer.bind_index();

	// index upload mapping
	sT.def_indexF(buffer.get_indices(),"offset",2,0,8);
	sT.def_indexF(buffer.get_indices(),"texpos",2,2,8);
	sT.def_indexF(buffer.get_indices(),"bounds",2,4,8);
	sT.def_indexF(buffer.get_indices(),"cursor",2,6,8);

	// load texture
	Toolbox::load_texture(tex,font.tp);

	// uniform variable upload
	Camera2D cam = Camera2D(1280.0f,720.0f);
	sT.upload_int("tex",0);
	sT.upload_float("wdt",font.mw);
	sT.upload_camera(cam);
}

/*
	prepare() -> void
	purpose: prepare text shader, buffer and gl settings to render added characters
*/
void Text::prepare()
{
	// gl settings
	glActiveTexture(GL_TEXTURE0);
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	// prepare shader & buffer
	sT.enable();
	buffer.bind();
}

/*
	render(int32_t,vec4) -> void
	amnt: amount of characters to render
	col: colour of rendered text in rgba format
	purpose: render given amount of text characters in given colour
*/
void Text::render(int32_t amnt,glm::vec4 col)
{
	buffer.upload_indices(ibv);
	sT.upload_vec4("colour",col); // ??shader uploads outside of prepare function
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,amnt);
}

/*
	set_scroll(mat4) -> void
	model: model matrix to upload to shader
	purpose: emulate a free text scrolling effect by common model transformation of sprites
*/
void Text::set_scroll(glm::mat4 model)
{ sT.upload_matrix("model",model); }

/*
	load_vertex() -> void (private)
	purpose: upload text vertices to buffer
*/
void Text::load_vertex() // !!no need to have this extra public vertex load function
{
	buffer.bind();
	buffer.upload_vertices(font.v,sizeof(font.v));
}