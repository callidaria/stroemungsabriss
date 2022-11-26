#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#ifdef __WIN32__
#define STB_IMAGE_IMPLEMENTATION
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../../include/stb_image.h"
#endif

#else
#include <SOIL/SOIL.h>
#endif

class Toolbox
{
public:
	static std::vector<float> create_sprite_canvas();
	static std::vector<float> create_sprite_canvas(glm::vec2 pos,float width,float height);
	static std::vector<float> create_sprite_canvas_triangled(glm::vec2 pos,float width,float height);
	static void load_texture(GLuint tex,const char* path);
	static void generate_elements(uint16_t i,std::vector<unsigned int> &ls);
};