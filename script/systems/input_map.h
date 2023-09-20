#ifndef SCR_SYSTEMS_INPUT_MAP
#define SCR_SYSTEMS_INPUT_MAP

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/frame.h"

// request id definitions
constexpr uint8_t IMP_REQWIDE = 0;
constexpr uint8_t IMP_REQFOCUS = 1;
constexpr uint8_t IMP_REQCQCDEF = 2;
constexpr uint8_t IMP_REQCQCATK = 3;
constexpr uint8_t IMP_REQBOMB = 4;
constexpr uint8_t IMP_REQCHANGE = 5;
constexpr uint8_t IMP_REQTARGET = 6;
constexpr uint8_t IMP_REQPAUSE = 7;
constexpr uint8_t IMP_REQDETAILS = 8;
constexpr uint8_t IMP_REQRESTART = 9;
constexpr uint8_t IMP_REQUP = 10;
constexpr uint8_t IMP_REQDOWN = 11;
constexpr uint8_t IMP_REQLEFT = 12;
constexpr uint8_t IMP_REQRIGHT = 13;
constexpr uint8_t IMP_REQCONFIRM = 14;

// values
constexpr uint8_t IMP_MAX_INPUTS = 15;
constexpr uint16_t IMP_CONTROLLERCAP = 32767;
constexpr uint16_t IMP_CONTROLLER_DEADZONE = 12000;
// TODO: change deadzone in options

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
	bool input_val[IMP_MAX_INPUTS],input_trg[IMP_MAX_INPUTS];
	glm::vec2 move_dir;

	// information
	std::string key_name[IMP_MAX_INPUTS],cnt_name[IMP_MAX_INPUTS];

private:

	// cascabel
	Frame* m_frame;

	// input index maps
	SDL_Scancode kmap[IMP_MAX_INPUTS] = {
		SDL_SCANCODE_C,SDL_SCANCODE_Z,SDL_SCANCODE_D,SDL_SCANCODE_V,SDL_SCANCODE_X,SDL_SCANCODE_F,
		SDL_SCANCODE_RSHIFT,SDL_SCANCODE_ESCAPE,SDL_SCANCODE_TAB,SDL_SCANCODE_R,SDL_SCANCODE_UP,
		SDL_SCANCODE_DOWN,SDL_SCANCODE_LEFT,SDL_SCANCODE_RIGHT,SDL_SCANCODE_RETURN
	}; bool caxis[IMP_MAX_INPUTS] = {
		true,false,false,false,false,false,false,false,false,true,false,false,false,false,false
	}; uint8_t cmap[IMP_MAX_INPUTS] = {
		SDL_CONTROLLER_AXIS_TRIGGERRIGHT,SDL_CONTROLLER_BUTTON_A,SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
		SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,SDL_CONTROLLER_BUTTON_B,SDL_CONTROLLER_BUTTON_Y,
		SDL_CONTROLLER_BUTTON_RIGHTSTICK,SDL_CONTROLLER_BUTTON_START,SDL_CONTROLLER_BUTTON_BACK,
		SDL_CONTROLLER_AXIS_TRIGGERLEFT,SDL_CONTROLLER_BUTTON_DPAD_UP,
		SDL_CONTROLLER_BUTTON_DPAD_DOWN,SDL_CONTROLLER_BUTTON_DPAD_LEFT,
		SDL_CONTROLLER_BUTTON_DPAD_RIGHT,SDL_CONTROLLER_BUTTON_A
	};

	// references
	bool* key_actions[IMP_MAX_INPUTS],*cnt_actions[IMP_MAX_INPUTS];

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

#endif
