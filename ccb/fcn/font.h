#ifndef CCB_FEATURE_FONT
#define CCB_FEATURE_FONT

#include <iostream>
#include <cstring>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"

class Font
{
public:

	// construction
	Font() {  }
	Font(const char* ff,const char* tf,float w,float h);
	~Font() {  }

	// tools
	uint8_t get_spritesheet_location(char c);
	uint32_t estimate_textwidth(const char* word);

public:

	float v[24],mw;
	int32_t id[96],x[96],y[96],wdt[96],hgt[96],xo[96],yo[96],xa[96]; // !!trim down according to text.h
	const char* tp;
};

#endif
