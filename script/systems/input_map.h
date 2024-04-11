#ifndef SCR_SYSTEMS_INPUT_MAP
#define SCR_SYSTEMS_INPUT_MAP

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/init.h"
#include "../../ccb/frm/frame.h"


enum InputID {

	// map: actions
	WIDE,
	FOCUS,
	CQCDEF,
	CQCATK,
	BOMB,

	// map: switches
	CHANGE,
	TARGET,

	// map: system
	PAUSE,
	DETAILS,
	RESTART,

	// map: movement
	UP,
	DOWN,
	LEFT,
	RIGHT,

	// map: universal
	CONFIRM,

	// final amount of input ids
	MAX_INPUTS,
};

struct IMapKeyReference
{
	SDL_Scancode key_id;
	uint8_t controller_id;
	bool is_axis;
};

class InputMap
{
public:

	// construction
	InputMap(Frame* frame);
	~InputMap() {  }

	// mapping
	void map_keyboard();
	void map_controller();
	void update();
	void update_triggers();

	// action translation
	glm::vec2 req_vectorized_direction();
	bool request(uint8_t request_id);

	// precalculate input
	void precalculate(uint8_t request_id);
	void precalculate_vector();
	void precalculate_dpad();
	void precalculate_all();
	void stick_to_dpad();

	// special
	inline bool get_input_triggered(uint8_t id) { return input_val[id]&&!input_trg[id]; }

private:

	// player interface helper
	std::string get_input_name(SDL_Scancode sc);
	std::string get_input_name(uint8_t cb,bool axis);

public:

	// precalculate input values
	bool input_val[InputID::MAX_INPUTS],input_trg[InputID::MAX_INPUTS];
	glm::vec2 move_dir;

	// information
	std::string key_name[InputID::MAX_INPUTS],cnt_name[InputID::MAX_INPUTS];

private:

	// cascabel
	Frame* m_frame;

	// input index maps
	IMapKeyReference kref[InputID::MAX_INPUTS] = {

		// map: actions
		{ .key_id = SDL_SCANCODE_C, .controller_id = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,true },
		{ .key_id = SDL_SCANCODE_Z, .controller_id = SDL_CONTROLLER_BUTTON_A,false },
		{ .key_id = SDL_SCANCODE_D, .controller_id = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,false },
		{ .key_id = SDL_SCANCODE_V, .controller_id = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,false },
		{ .key_id = SDL_SCANCODE_X, .controller_id = SDL_CONTROLLER_BUTTON_B,false },

		// map: switches
		{ .key_id = SDL_SCANCODE_F, .controller_id = SDL_CONTROLLER_BUTTON_Y,false },
		{ .key_id = SDL_SCANCODE_RSHIFT, .controller_id = SDL_CONTROLLER_BUTTON_RIGHTSTICK,false },

		// map: system
		{ .key_id = SDL_SCANCODE_ESCAPE, .controller_id = SDL_CONTROLLER_BUTTON_START,false },
		{ .key_id = SDL_SCANCODE_TAB, .controller_id = SDL_CONTROLLER_BUTTON_BACK,false },
		{ .key_id = SDL_SCANCODE_R, .controller_id = SDL_CONTROLLER_AXIS_TRIGGERLEFT,true },

		// map: movement
		{ .key_id = SDL_SCANCODE_UP, .controller_id = SDL_CONTROLLER_BUTTON_DPAD_UP,false },
		{ .key_id = SDL_SCANCODE_DOWN, .controller_id = SDL_CONTROLLER_BUTTON_DPAD_DOWN,false },
		{ .key_id = SDL_SCANCODE_LEFT, .controller_id = SDL_CONTROLLER_BUTTON_DPAD_LEFT,false },
		{ .key_id = SDL_SCANCODE_RIGHT, .controller_id = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,false },

		// map: universal
		{ .key_id = SDL_SCANCODE_RETURN, .controller_id = SDL_CONTROLLER_BUTTON_A,false },
	};
	// TODO: fill this not by hand but store key setup in config file
	//	keep this as a standard setup, should config read fail or a new config should be setup

	// references
	bool* key_actions[InputID::MAX_INPUTS],*cnt_actions[InputID::MAX_INPUTS];

	// axis references
	int32_t* cnt_udaxis,*cnt_lraxis;

	// replacement addresses
	bool rpl_bool = false;
	int32_t rpl_int = 0;
};
// TODO: make the input map dynamic for button reallocation
//	remap by InputID and through simple remap function called for each remapping
//	causes immediate remapping and reset is caught by initialization file reset and countermapping
//	(mdc)

#endif
