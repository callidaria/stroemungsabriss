#pragma once

// !!check for irrelevant includes
#include <iostream>
#include <cstring>
#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#ifdef __WIN32__
#define STB_IMAGE_IMPLEMENTATION
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../ext/stb_image.h"
#endif
#else
#include <SOIL/SOIL.h>
#endif

class Font
{
public:
	Font(const char* ff,const char* tf,float w,float h);
	void texture();
	void setup();
public:
	float v[24],mw;
	int id[96],x[96],y[96],wdt[96],hgt[96],xo[96],yo[96],xa[96]; // !!trim down according to text.h
	const char* tp;
	unsigned int tex;
};