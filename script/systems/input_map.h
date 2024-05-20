#ifndef SCR_SYSTEMS_INPUT_MAP
#define SCR_SYSTEMS_INPUT_MAP

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/core.h"
#include "../../ccb/fcn/init.h"


enum InputID : uint8_t {
	WIDE,
	FOCUS,
	CQCDEF,
	CQCATK,
	BOMB,
	CHANGE,
	TARGET,
	PAUSE,
	DETAILS,
	RESTART,
	UP,
	DOWN,
	LEFT,
	RIGHT,
	CONFIRM,
	MAX_INPUTS,
};

class InputMap
{
public:

	// construction
	InputMap();
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

	// input index maps
	SDL_Scancode kmap[InputID::MAX_INPUTS] = {
		SDL_SCANCODE_C,SDL_SCANCODE_Z,SDL_SCANCODE_D,SDL_SCANCODE_V,SDL_SCANCODE_X,SDL_SCANCODE_F,
		SDL_SCANCODE_RSHIFT,SDL_SCANCODE_ESCAPE,SDL_SCANCODE_TAB,SDL_SCANCODE_R,SDL_SCANCODE_UP,
		SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,SDL_SCANCODE_RETURN
	}; bool caxis[InputID::MAX_INPUTS] = {
		true,false,false,false,false,false,false,false,false,true,false,false,false,false,false
	}; uint8_t cmap[InputID::MAX_INPUTS] = {
		SDL_CONTROLLER_AXIS_TRIGGERRIGHT,SDL_CONTROLLER_BUTTON_A,SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,SDL_CONTROLLER_BUTTON_B,SDL_CONTROLLER_BUTTON_Y,
		SDL_CONTROLLER_BUTTON_RIGHTSTICK,SDL_CONTROLLER_BUTTON_START,SDL_CONTROLLER_BUTTON_BACK,
		SDL_CONTROLLER_AXIS_TRIGGERLEFT,SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_CONTROLLER_BUTTON_DPAD_DOWN,SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_CONTROLLER_BUTTON_DPAD_RIGHT,SDL_CONTROLLER_BUTTON_A
	};

	// references
	bool* key_actions[InputID::MAX_INPUTS],*cnt_actions[InputID::MAX_INPUTS];

	// axis references
	int32_t* cnt_udaxis,*cnt_lraxis;

	// data
	const std::string keynames[9] = {
		"RETURN","ESCAPE","BACKSPACE","TAB","SPACE","RIGHT","LEFT","DOWN","UP"
	},cntnames[21] = {
		"A","B","X","Y","OPTIONS","GUIDE","START","LSTICK","RSTICK","RSHOULDER","LSHOULDER",
		"UP","DOWN","LEFT","RIGHT","CAPTURE","PADDLE 1","PADDLE 2","PADDLE 3","PADDLE 4","TRIGGERPAD"
	},axisnames[2] = { "LTRIGGER","RTRIGGER" };

	// replacement addresses
	bool rpl_bool = false;
	int32_t rpl_int = 0;
};
// TODO: make the input map dynamic for button reallocation

static inline InputMap gIMap = InputMap();
// TODO: make part of core somehow... or no core at all?

#endif
