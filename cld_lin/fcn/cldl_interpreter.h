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

class CLDLInterpreter
{
public:
	CLDLInterpreter(Renderer2D* r2d,Text* txt);
	void load_level(const char* path);
private:
	Renderer2D* m_r2d;
	Text* m_txt;
public:
	int spritesCount,charsCount;
};
