#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../gfx/shader.h"
#include "../mat/camera2d.h"
#include "buffer.h"
#include "font.h"

#define MAX_CHAR_COUNT 4096

class Text
{
public:

	// construction
	Text();
	Text(Font* f);

	// write
	int add(char c,glm::vec2 p);
	void add(const char* s,glm::vec2 p);
	void clear();

	// preparation
	void load(Camera2D* c);
	void prepare();

	// draw
	void render(int amnt,glm::vec4 col);

	// uniform
	void set_scroll(glm::mat4 model);

private:

	void load_vertex();

private:

	Shader sT;
	Font* m_font;
	Buffer buffer;
	std::vector<float> ibv;
};