#ifndef CCB_FEATURE_TEXT
#define CCB_FEATURE_TEXT

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "buffer.h"
#include "font.h"

#include "../gfx/shader.h"
#include "../mat/camera2d.h"

class Text
{
public:

	// construction
	Text() {  }
	Text(Font f);
	~Text() {  }

	// write
	uint32_t add(char c,glm::vec2 p);
	uint32_t add(uint8_t i,glm::vec2 p);
	void add(const char* s,glm::vec2 p);
	glm::vec2 add(std::string s,glm::vec2 p,float bwdt,float nline_offset);
	void clear();

	// preparation
	void texture();
	void enable_shader();
	void load();
	void prepare();

	// draw
	void render(int32_t amnt,glm::vec4 col);

	// uniform
	void set_scroll(glm::vec2 scroll);
	void set_scroll(glm::mat4 model);

private:

	// helper
	void load_vertex();
	uint8_t get_spritesheet_location(char c);

private:

	// cascabel
	Shader sT;
	Font font;
	Buffer buffer = Buffer();

	// vertices & texturing
	std::vector<float> ibv;
	GLuint ftexture;
};

#endif