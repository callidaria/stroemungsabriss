#include "text.h"

/*
	constructor(Font*)
	f: pointer to font, holding the .fnt and texture that are going to be used when rendering text
	purpose: create an entity to later add text and characters to
*/
Text::Text(Font f)
	:font(f)
{
	buffer.add_buffer();
	glGenTextures(1,&ftexture);
}

/*
	add(char,vec2) -> uint32_t
	c: character to add to the text entity
	p: position to add the character at
	purpose: add a single character to the text entity
	returns: x-axis modification of cursor position after adding the given character
*/
uint32_t Text::add(char c,glm::vec2 p) // !!passing x increment like this is very bad pracice with public method
{
	// identifying sprite sheet position & write
	uint8_t i = get_spritesheet_location(c);
	return add(i,p);
}

/*
	add(uint8_t,vec2) -> uint32_t
	i: id of character to add
	p: cursor position
	purpose: add a single character to the text entity identified by it's font id
	returns: x-axis modification of cursor position after adding the given character
*/
uint32_t Text::add(uint8_t i,glm::vec2 p)
{
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
	add(string,vec2,float,float) -> vec2
	s: string of characters to write to text entity
	p: origin position of cursor to specify writing position start
	bwdt: width of text space until cursor reaches border & automatically inserts breakline
	nline_offset: distance the cursor should travel on the y-axis for a breakline
	purpose: add string of characters within borders, automatically inserting breaklines
	returns: new cursor position after text writing is finished
*/
glm::vec2 Text::add(std::string s,glm::vec2 p,float bwdt,float nline_offset)
{
	// pry apart words from given string
	std::vector<std::string> wrds;
	std::string crr_word = "";
	for (auto c:s) {
		if (c==' ') {
			wrds.push_back(crr_word);
			crr_word = "";
		} else crr_word += c;
	} wrds.push_back(crr_word);

	// add found words without breaking border width
	float crr_width = 0;
	for (auto wrd:wrds) {

		// calculate estimated word width
		uint16_t estm_wwidth = 0;
		std::vector<uint8_t> char_ids;
		for (auto c:wrd) {
			uint8_t ssloc = get_spritesheet_location(c);
			estm_wwidth += font.xa[ssloc]*(font.mw/83.0f);	// ??outdated
			char_ids.push_back(ssloc);
		}

		// break line if word violates border width
		bool br_line = (crr_width+estm_wwidth)>bwdt;
		p.x -= crr_width*br_line;p.y -= nline_offset*br_line;
		crr_width *= !br_line;

		// add characters to text entity
		for (auto ic:char_ids) p.x += add(ic,p);

		// add space after word & update current width
		float wordspacing = 57.0f*(font.mw/83.0f);
		p.x += wordspacing;
		crr_width += estm_wwidth+wordspacing;
		// FIXME: duplicate code!
	}

	// return cursor position
	return glm::vec2(crr_width,p.y);
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
{ Toolbox::load_texture(ftexture,font.tp); }

/*
	enable_shader() -> void
	purpose: enable text shader outside of text class
*/
void Text::enable_shader()
{ sT.enable(); }

/*
	load(Camera2D*) -> void
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
	sT.upload_float("wdt",font.mw);
	sT.upload_camera(Camera2D(1280.0f,720.0f));
	texture();
}
// FIXME: don't calculate a new camera every time you load a new text. it's always the same one!

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
	glBindTexture(GL_TEXTURE_2D,ftexture);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,amnt);
}

/*
	set_scroll(vec2) -> void
	scroll: distance and direction the text should scroll scroll towards
	purpose: scroll the text entity
*/
void Text::set_scroll(glm::vec2 scroll)
{
	glm::mat4 model = glm::translate(glm::mat4(1.0f),glm::vec3(scroll.x,scroll.y,0));
	sT.upload_matrix("model",model);
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

/*
	get_spritesheet_location(char) -> uint8_t
	c: char to get spritesheet location id for
	purpose: get spritesheet id for desired char, to reference char gfx in font
	returns: spritesheet id of given char
*/
uint8_t Text::get_spritesheet_location(char c)
{
	int i = 0;
	while (i<96) { // ??maybe alternate iteration until correct index that is more performant
		if (font.id[i]==(int)c) break;
		i++;
	}
	return i;
}
// FIXME: optimize please