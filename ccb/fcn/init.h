#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#define INTEGER_KEY_LENGTH 4

class Init
{
public:
	Init(const char* path);
	uint32_t rINT(uint32_t id);
private:
	uint32_t find_iKey(const char* key);
public:
	// keys
	const uint32_t FRAME_RESOLUTION_WIDTH = 0;
	const uint32_t FRAME_RESOLUTION_HEIGHT = 1;
	const uint32_t FRAME_SET_FULLSCREEN = 2;
	const uint32_t FRAME_DISPLAY_ID = 3;
private:
	// key identification dictionaries
	std::vector<const char*> iKeys = { "frameResWidth:","frameResHeight:","frameFullScreen:",
		"frameDisplayID:" };

	// constant collections
	uint32_t iConfig[INTEGER_KEY_LENGTH];
};
