#ifndef CCB_FEATURE_INPUT
#define CCB_FEATURE_INPUT

#include "../mat/toolbox.h"
#include "../fcn/configuration.h"


// input
constexpr uint16_t KEYBOARD_INPUT_RANGE = 285;
constexpr uint8_t MOUSE_BUTTON_INPUT_RANGE = 5;
constexpr uint8_t CONTROLLER_AXIS_INPUT_RANGE = 6;
constexpr uint8_t CONTROLLER_BUTTON_INPUT_RANGE = 22;


// representing keyboard keys as an array
struct Keyboard
{
	bool ka[KEYBOARD_INPUT_RANGE] = { false };
};

// mouse buttons as array, as well as absolute and relative mouse positions. also mousewheel
struct Mouse
{
	// buttons
	bool mb[MOUSE_BUTTON_INPUT_RANGE] = { false };

	// mouse positions absolute and within frame context
	int32_t mx,my;
	float mxfr,myfr;

	// mousewheel
	int32_t mw;
};

// controller buttons and axis, modelled with xbox controller layout in mind
struct XBox
{
	int32_t xba[CONTROLLER_AXIS_INPUT_RANGE];
	bool xbb[CONTROLLER_BUTTON_INPUT_RANGE] = { false };
};


class Input
{
public:

	// construction
	Input() {  }
	~Input() {  }

	// system
	void update(bool& running);
	inline void close() { kill_controllers(); }

private:

	// hardware to software representation of controller devices
	void load_controllers();
	void kill_controllers();

public:

	// devices
	static inline Keyboard kb;
	static inline Mouse mouse;
	static inline std::vector<XBox> xb;

private:

	// api
	SDL_Event m_event;
	std::vector<SDL_GameController*> m_gc;

	// settings
	uint32_t const& axis_deadzone = Configuration::correlate_variable(VKEY_PERIPHERAL_AXIS_DEADZONE);
};

#endif
