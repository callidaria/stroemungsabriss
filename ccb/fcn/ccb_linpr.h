#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../gfx/renderer2d.h"
#include "text.h"

/**
 *		ATTENTION SHOPPERS
 *	THIS FILE IS WORK IN PROGRESS! FEATURES WILL BE ADDED IN THE FUTURE.
 *	PLEASE DONT JUDGE OR RELY ON THIS CODE. THANK YOU!
 * */

class CCBLInterpreter
{
public:
	CCBLInterpreter(Renderer2D* r2d,Text* txt,const char* path);
	int load_level();
	void write_level();
private:
	Renderer2D* m_r2d;
	Text* m_txt;
	const char* lvpath;
public:
	std::vector<glm::vec2> m_pos;
	std::vector<float> m_width,m_height;
	std::vector<const char*> m_tex;
	int spritesCount,charsCount;
};
