#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>


// variable keymap
inline std::vector<const char*> iKeys = {

	"iFrameResolutionPreset",
	"vFrameResolution","vFrameResolution",
	"bFrameFullscreen",
	"iFrameTargetMonitor",

	"iGraphicsTargetLibrary",
	"iGraphicsPresetLevel",

	"iGeneralLanguageWrittenText",
	"iGeneralLanguageSpokenWord",
	"iGeneralForcePeripheral",

	"fAudioMasterVolume",
	"fAudioVoicesVolume",
	"fAudioMusicVolume",
	"fAudioEffectsVolume",
};

enum InitVariable
{
	FRAME_RESOLUTION_PRESET,
	FRAME_RESOLUTION_WIDTH,FRAME_RESOLUTION_HEIGHT,
	FRAME_SET_FULLSCREEN,
	FRAME_DISPLAY_ID,

	GRAPHICS_TARGET_LIBRARY,
	GRAPHICS_PRESET_LEVEL,

	GENERAL_TEXT_LANGUAGE,
	GENERAL_VOICES_LANGUAGE,
	GENERAL_PERIPHERAL_PREFERENCE,

	AUDIO_VOLUME_MASTER,
	AUDIO_VOLUME_VOICES,
	AUDIO_VOLUME_MUSIC,
	AUDIO_VOLUME_EFFECTS,

	VARIABLE_ERROR,
	VARIABLE_KEY_LENGTH = VARIABLE_ERROR
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
