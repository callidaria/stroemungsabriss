#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../gfx/renderer2d.h"

/**
 *		ATTENTION SHOPPERS
 *	THIS FILE IS WORK IN PROGRESS! FEATURES WILL BE ADDED IN THE FUTURE.
 *	PLEASE DONT JUDGE OR RELY ON THIS CODE. THANK YOU!
 * */

class CCBLInterpreter
{

public:

	CCBLInterpreter(Renderer2D* r2d,const char* path);

	// interaction
	uint16_t load_level();
	void write_level();

private:

	// cascabel
	Renderer2D* m_r2d;

	// source
	const char* lvpath;

public:

	// object information
	std::vector<glm::vec2> m_pos,a_pos;
	std::vector<float> m_width,a_width,m_height,a_height,a_row,a_column,a_frames,a_ts;
	std::vector<const char*> m_tex,a_tex;
	int spritesCount;
};