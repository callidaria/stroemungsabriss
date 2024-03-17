#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>


constexpr uint8_t AMOUNT_RESOLUTION_PRESETS = 3;


struct InitVariableInfo {
	const char* name;
	const bool restart_system_on_change;
};

// variable keymap
const inline std::vector<InitVariableInfo> iKeys = {

	{ "iFrameResolutionPreset",true },
	{ "vFrameResolutionWidth",false },{ "vFrameResolutionHeight",false },
	{ "bFrameFullscreen",true },
	{ "iFrameTargetMonitor",true },

	{ "iGraphicsTargetLibrary",true },
	{ "iGraphicsPresetLevel",false },

	{ "iGeneralLanguageWrittenText",false },
	{ "iGeneralLanguageSpokenWord",false },
	{ "iGeneralForcePeripheral",false },
	{ "iGeneralPeripheralAxisDeadzone",false },
	{ "iGeneralPeripheralAxisRange",false },

	{ "fAudioMasterVolume",false },
	{ "fAudioVoicesVolume",false },
	{ "fAudioMusicVolume",false },
	{ "fAudioEffectsVolume",false },
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
	GENERAL_PERIPHERAL_AXIS_DEADZONE,
	GENERAL_PERIPHERAL_AXIS_RANGE,

	AUDIO_VOLUME_MASTER,
	AUDIO_VOLUME_VOICES,
	AUDIO_VOLUME_MUSIC,
	AUDIO_VOLUME_EFFECTS,

	VARIABLE_KEY_LENGTH,
	VARIABLE_ERROR = VARIABLE_KEY_LENGTH
};

class Init
{
public:

	Init(const char* path);
	static uint32_t find_iKey(const char* key);
	static void write_changes();

private:

	std::vector<uint32_t> read_cartesian(std::string val);

public:

	// constant collections
	static inline uint32_t iConfig[InitVariable::VARIABLE_KEY_LENGTH];

private:

	// preset translation
	static inline uint16_t resolutionWidthPresets[AMOUNT_RESOLUTION_PRESETS] = { 1280,1600,1920 };
	static inline uint16_t resolutionHeightPresets[AMOUNT_RESOLUTION_PRESETS] = { 720,900,1080 };
};

#endif
