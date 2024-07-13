#ifndef CCB_FEATURE_INITIALIZATION
#define CCB_FEATURE_INITIALIZATION

#include "../mat/toolbox.h"


// configuration variable key definitions
// frame resolution preset translation
constexpr uint8_t AMOUNT_RESOLUTION_PRESETS = 3;
constexpr char VKEY_RESOLUTION_PRESET[] = "iFrameResolutionPreset";
constexpr char VKEY_RESOLUTION_WIDTH[] = "vFrameResolutionWidth";
constexpr char VKEY_RESOLUTION_HEIGHT[] = "vFrameResolutionHeight";

// frame variables correlation
constexpr char VKEY_FRAME_FULLSCREEN[] = "bFrameFullscreen";
constexpr char VKEY_FRAME_DISPLAY[] = "iFrameTargetMonitor";

// input variable correlation
constexpr char VKEY_PERIPHERAL_FORCED[] = "iGeneralPeripheralAxisDeadzone";
constexpr char VKEY_PERIPHERAL_AXIS_DEADZONE[] = "iGeneralPeripheralAxisDeadzone";


// interaction tools with config file
class Configuration
{
public:

	// interaction with initialization data structure
	static void run_init();
	static inline uint32_t const& correlate_variable(std::string key) { return iConfig[key]; };
	static void write_changes();

private:

	// constant collections
	static inline std::map<std::string,uint32_t> iConfig;

	// preset translation
	static inline uint16_t resolutionWidthPresets[AMOUNT_RESOLUTION_PRESETS] = { 1280,1600,1920 };
	static inline uint16_t resolutionHeightPresets[AMOUNT_RESOLUTION_PRESETS] = { 720,900,1080 };
};

#endif
