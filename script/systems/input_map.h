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

// values
constexpr uint8_t IMP_MAX_INPUTS = 14;
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

	// special
	inline bool get_input_triggered(uint8_t id) { return input_val[id]&&!input_trg[id]; }

public:

	// precalculate input values
	bool input_val[IMP_MAX_INPUTS],input_trg[IMP_MAX_INPUTS];
	glm::vec2 move_dir;

private:

	// cascabel
	Frame* m_frame;

	// replacement addresses
	bool rpl_bool = false;
	int32_t rpl_int = 0;

	// references
	bool* key_actions[IMP_MAX_INPUTS],*cnt_actions[IMP_MAX_INPUTS];

	// axis references
	int32_t* cnt_udaxis,*cnt_lraxis;
};
// TODO: make the input map dynamic for button reallocation

#endif