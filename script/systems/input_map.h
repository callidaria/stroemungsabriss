#pragma once

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
constexpr uint16_t IMP_CONTROLLERCAP = 32767;

class InputMap
{
public:

	// construction
	InputMap(Frame* frame);
	~InputMap() {  }

	// mapping
	void map_keyboard();
	void map_controller();

	// action translation
	glm::vec2 req_vectorized_direction();
	bool request(uint8_t request_id);

	// directional request
	bool req_up();
	bool req_down();
	bool req_left();
	bool req_right();

	// precalculate input
	void precalculate(uint8_t calc_id);
	void precalculate_vector();
	void precalculate_up();
	void precalculate_down();
	void precalculate_left();
	void precalculate_right();
	void precalculate_dpad();
	void precalculate_all();

public:

	// precalculate input values
	bool input_val[14];
	glm::vec2 move_dir;

private:

	// cascabel
	Frame* m_frame;

	// replacement addresses
	bool rpl_bool = false;
	int32_t rpl_int = 0;

	// references
	bool* key_actions[14];
	bool* cnt_actions[10];

	// axis references
	int32_t* cnt_udaxis,*cnt_lraxis;
};
// TODO: make the input map dynamic for button reallocation
// TODO: precalculate and request values for certain inputs