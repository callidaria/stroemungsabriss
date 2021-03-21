#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "../gfx/shader.h"
#include "../mat/camera2d.h"
#include "font.h"

#define MAX_CHAR_COUNT 4096

class Text
{
public:
	Text();
	Text(Font* f);
	int add(char c,glm::vec2 p);
	void add(const char* s,glm::vec2 p);
	void load_vertex();
	void load_wcam(Camera2D* c);
	void prepare();
	void render(int amnt,glm::vec4 col);
private:
	Shader sT;Font* font;
	unsigned int vao,vbo,ibo,ind;
	float ibv[8*MAX_CHAR_COUNT];
};
