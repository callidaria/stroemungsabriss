#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#define INTEGER_KEY_LENGTH 4
// TODO: change to constexpr

class Init
{

public:

	Init(const char* path);

	uint32_t rINT(uint32_t id) { return iConfig[id]; }

private:

	uint32_t find_iKey(const char* key);
	std::vector<uint32_t> read_cartesian(std::string val);

public:

	// keys
	const uint32_t FRAME_RESOLUTION_WIDTH = 0;
	const uint32_t FRAME_RESOLUTION_HEIGHT = 1;
	const uint32_t FRAME_SET_FULLSCREEN = 2;
	const uint32_t FRAME_DISPLAY_ID = 3;

private:

	// key identification dictionaries
	std::vector<const char*> iKeys = { "vFrameResolution","vFrameResolution","bFrameFullscreen",
		"iFrameTargetMonitor" };
	// TODO: generate this list automatically, when reading the file

	// constant collections
	uint32_t iConfig[INTEGER_KEY_LENGTH];

};

#endif