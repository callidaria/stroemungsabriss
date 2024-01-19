#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>


// key identification dictionaries
inline std::vector<const char*> iKeys = {
	"vFrameResolution","vFrameResolution","bFrameFullscreen","iFrameTargetMonitor"
};

enum InitVariable
{
	FRAME_RESOLUTION_WIDTH,
	FRAME_RESOLUTION_HEIGHT,
	FRAME_SET_FULLSCREEN,
	FRAME_DISPLAY_ID,
	VARIABLE_KEY_LENGTH
};

class Init
{
public:

	Init(const char* path);
	static uint32_t find_iKey(const char* key);

private:

	std::vector<uint32_t> read_cartesian(std::string val);

public:

	// constant collections
	static inline uint32_t iConfig[InitVariable::VARIABLE_KEY_LENGTH];
};

#endif
