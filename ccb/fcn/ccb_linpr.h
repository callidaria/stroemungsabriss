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

enum CCBSpriteArgs {
	CSPR_COMMAND,
	CSPR_POSX,CSPR_POSY,
	CSPR_WIDTH,CSPR_HEIGHT,
	CSPR_TEXPATH,
	CSPR_ARGUMENT_COUNT
};

enum CCBAnimArgs {
	CANM_COMMAND,
	CANM_POSX,CANM_POSY,
	CANM_WIDTH,CANM_HEIGHT,
	CANM_TEXPATH,
	CANM_REPEAT,
	CANM_COUNT,
	CANM_FRAMES,
	CANM_TICKS,
	CANM_ARGUMENT_COUNT
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
