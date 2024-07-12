#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include "../mat/toolbox.h"


// frame resolution preset translation
constexpr uint8_t AMOUNT_RESOLUTION_PRESETS = 3;
constexpr char* RESOLUTION_PRESET_IDENTIFIER = "iFrameResolutionPreset";
constexpr char* RESOLUTION_WIDTH_IDENTIFIER = "vFrameResolutionWidth";
constexpr char* RESOLUTION_HEIGHT_IDENTIFIER = "vFrameResolutionHeight";

// frame variables correlation
constexpr char* FRAME_FULLSCREEN_IDENTIFIER = "bFrameFullscreen";
constexpr char* FRAME_DISPLAY_IDENTIFIER = "iFrameTargetMonitor";

// input variable correlation
constexpr char* PERIPHERAL_DEADZONE_IDENTIFIER = "iGeneralPeripheralAxisDeadzone";


/*
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
*/
// DEPRECATED: key-value mapping has replaced previous variable handling

class Init
{
public:

	// interaction with initialization data structure
	static void run_init();
	static inline const uint32_t& correlate_variable(std::string key) { return iConfig[key]; };
	static void write_changes();

public:

	// constant collections
	static std::map<std::string,uint32_t> iConfig;

public:

	// preset translation
	static inline uint16_t resolutionWidthPresets[AMOUNT_RESOLUTION_PRESETS] = { 1280,1600,1920 };
	static inline uint16_t resolutionHeightPresets[AMOUNT_RESOLUTION_PRESETS] = { 720,900,1080 };
};

#endif
