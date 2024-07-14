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
constexpr char VKEY_PERIPHERAL_FORCED[] = "iGeneralForcePeripheral";
constexpr char VKEY_PERIPHERAL_AXIS_DEADZONE[] = "iGeneralPeripheralAxisDeadzone";


// references to configuration variables, to reduce expensive correlation
struct Config
{
	// frame resolution preset
	uint32_t& iFrameResolutionPreset;
	uint32_t& vFrameResolutionWidth,&vFrameResolutionHeight;

	// frame state
	uint32_t& bFrameFullscreen;
	uint32_t& iFrameTargetMonitor;

	// input
	uint32_t& iGeneralForcePeripheral;
	uint32_t& iGeneralPeripheralAxisDeadzone;
};
// TODO: create automized referencing


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
