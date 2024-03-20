#ifndef CCB_FEATURE_LEVELINTERPRETER
#define CCB_FEATURE_LEVELINTERPRETER

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "../gfx/renderer2d.h"

/**
 *		ATTENTION SHOPPERS
 *	THIS FILE IS WORK IN PROGRESS! FEATURES WILL BE ADDED IN THE FUTURE.
 *	PLEASE DONT JUDGE OR RELY ON THIS CODE. THANK YOU!
*/

enum CCBArgs {
	COMMAND,
	POS_X,POS_Y,
	WIDTH,HEIGHT,
	TEXPATH,
	REPEAT,
	COUNT,
	FRAMES,
	TICKS,
	ARGUMENT_COUNT
};

struct InterpreterSpriteData {
	glm::vec2 position;
	float width,height;
	const char* texpath;
};

struct InterpreterAnimData {
	glm::vec2 position;
	float width,height;
	const char* texpath;
	uint8_t row,column,frames,ticks;
};

class CCBLInterpreter
{
public:

	CCBLInterpreter(Renderer2D* r2d,const char* path);

	// interaction
	uint16_t load_level();
	void write_level();
	void delete_level();

private:

	// cascabel
	Renderer2D* m_r2d;

	// source
	const char* lvpath;

public:

	// object information
	std::vector<InterpreterSpriteData> sprite_data;
	std::vector<InterpreterAnimData> anim_data;
};

#endif