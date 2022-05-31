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
	Text();
	Text(Font f);
	int add(char c,glm::vec2 p);
	void add(const char* s,glm::vec2 p);
	void clear();
	void load_vertex();
	void load_wcam(Camera2D* c);
	void prepare();
	void render_propag(int amnt,glm::vec4 col);
	void render(int amnt,glm::vec4 col);
	void set_scroll(glm::mat4 model);
private:
	Shader sT;
	Font font;
	Buffer buffer;
	std::vector<float> ibv;
};
