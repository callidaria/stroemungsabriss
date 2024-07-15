#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include "../mat/toolbox.h"


// configuration variable key definitions
constexpr uint8_t AMOUNT_MAPPED_VARIABLES = 7;
constexpr uint8_t AMOUNT_RESOLUTION_PRESETS = 3;


// references to configuration variables, to reduce expensive correlation by caching common key results
struct Config
{
	// frame resolution preset
	uint32_t iFrameResolutionPreset;
	uint32_t vFrameResolutionWidth,vFrameResolutionHeight;

	// frame state
	uint32_t bFrameFullscreen;
	uint32_t iFrameTargetMonitor;

	// input
	uint32_t iGeneralForcePeripheral;
	uint32_t iGeneralPeripheralAxisDeadzone;
};

static inline Config g_Config;


// interaction tools with config file
class Configuration
{
public:

	// interaction with initialization data structure
	static void run_init();
	static void write_changes();

private:

	// constant collections
	static inline std::map<std::string,uint32_t> iConfig;

	// preset translation
	static inline uint16_t resolutionWidthPresets[AMOUNT_RESOLUTION_PRESETS] = { 1280,1600,1920 };
	static inline uint16_t resolutionHeightPresets[AMOUNT_RESOLUTION_PRESETS] = { 720,900,1080 };

	// variable mapping
	static inline std::string const VKEY_IDENTIFIER_MAP[AMOUNT_MAPPED_VARIABLES] = {

		// frame resolution preset translation
		"iFrameResolutionPreset",
		"vFrameResolutionWidth","vFrameResolutionHeight",

		// frame variable correlations
		"bFrameFullscreen",
		"iFrameTargetMonitor",

		// input variable correlation
		"iGeneralForcePeripheral",
		"iGeneralPeripheralAxisDeadzone",
	};
};

#endif
