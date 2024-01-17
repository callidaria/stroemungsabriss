#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

constexpr uint16_t INTEGER_KEY_LENGTH = 4;

enum InitVariable
{
	FRAME_RESOLUTION_WIDTH,
	FRAME_RESOLUTION_HEIGHT,
	FRAME_SET_FULLSCREEN,
	FRAME_DISPLAY_ID
};

class Init
{
public:

	Init(const char* path);

private:

	uint32_t find_iKey(const char* key);
	std::vector<uint32_t> read_cartesian(std::string val);

public:

	// constant collections
	static inline uint32_t iConfig[INTEGER_KEY_LENGTH];

private:

	// key identification dictionaries
	std::vector<const char*> iKeys = { "vFrameResolution","vFrameResolution","bFrameFullscreen",
		"iFrameTargetMonitor" };
	// TODO: generate this list automatically, when reading the file
};

#endif
