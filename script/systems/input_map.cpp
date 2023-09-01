#include "input_map.h"

/*
	constructor(Frame*)
	frame: reference to frame, which is enabled when the input happens
	purpose: map keyboard & controller inputs
*/
InputMap::InputMap(Frame* frame)
	: m_frame(frame)
{
	map_keyboard();
	map_controller();
}

/*
	map_keyboard() -> void
	purpose: map all keyboard inputs to their desired functionality
*/
void InputMap::map_keyboard()
{
	key_actions[IMP_REQWIDE] = &m_frame->kb.ka[SDL_SCANCODE_C];
	key_actions[IMP_REQFOCUS] = &m_frame->kb.ka[SDL_SCANCODE_Z];
	key_actions[IMP_REQCQCDEF] = &m_frame->kb.ka[SDL_SCANCODE_D];
	key_actions[IMP_REQCQCATK] = &m_frame->kb.ka[SDL_SCANCODE_V];
	key_actions[IMP_REQBOMB] = &m_frame->kb.ka[SDL_SCANCODE_X];
	key_actions[IMP_REQCHANGE] = &m_frame->kb.ka[SDL_SCANCODE_F];
	key_actions[IMP_REQTARGET] = &m_frame->kb.ka[SDL_SCANCODE_RSHIFT];
	key_actions[IMP_REQPAUSE] = &m_frame->kb.ka[SDL_SCANCODE_ESCAPE];
	key_actions[IMP_REQDETAILS] = &m_frame->kb.ka[SDL_SCANCODE_TAB];
	key_actions[IMP_REQRESTART] = &m_frame->kb.ka[SDL_SCANCODE_R];
	key_actions[IMP_REQUP] = &m_frame->kb.ka[SDL_SCANCODE_UP];
	key_actions[IMP_REQDOWN] = &m_frame->kb.ka[SDL_SCANCODE_DOWN];
	key_actions[IMP_REQLEFT] = &m_frame->kb.ka[SDL_SCANCODE_LEFT];
	key_actions[IMP_REQRIGHT] = &m_frame->kb.ka[SDL_SCANCODE_RIGHT];
}

/*
	map_controller() -> void
	purpose: map all controller inputs to their desired functionality
*/
void InputMap::map_controller()
{
	// controller has been plugged in
	if (m_frame->m_gc.size()) {
		cnt_lraxis = &m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_LEFTX];
		cnt_udaxis = &m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_LEFTY];
		cnt_actions[IMP_REQWIDE] = (bool*)&m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_TRIGGERRIGHT];
		cnt_actions[IMP_REQFOCUS] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_A];
		cnt_actions[IMP_REQCQCDEF] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_LEFTSHOULDER];
		cnt_actions[IMP_REQCQCATK] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER];
		cnt_actions[IMP_REQBOMB] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_B];
		cnt_actions[IMP_REQCHANGE] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_Y];
		cnt_actions[IMP_REQTARGET] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_RIGHTSTICK];
		cnt_actions[IMP_REQPAUSE] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_START];
		cnt_actions[IMP_REQDETAILS] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_BACK];
		cnt_actions[IMP_REQRESTART] = (bool*)&m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_TRIGGERLEFT];
		cnt_actions[IMP_REQUP] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_DPAD_UP];
		cnt_actions[IMP_REQDOWN] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
		cnt_actions[IMP_REQLEFT] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
		cnt_actions[IMP_REQRIGHT] = &m_frame->xb[0].xbb[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
	}

	// no controller input
	else {
		cnt_udaxis = &rpl_int;cnt_lraxis = &rpl_int;
		for (uint8_t i=0;i<IMP_MAX_INPUTS;i++) cnt_actions[i] = &rpl_bool;
	}
}
// TODO: add option to change players controller when multiple are plugged in

/*
	update() -> void
	purpose: check for controller hotplugging and update mapping, should it happen
*/
void InputMap::update()
{
	if (m_frame->controller_remap) {
		map_controller();
		m_frame->controller_remap = false;
	}
}
// FIXME: constant (nested) branching. maybe worth it, but it can always be improved

/*
	update_triggers() -> void !O(1)
	purpose: save raw last frame inputs to update input triggers
*/
void InputMap::update_triggers()
{
	for (uint8_t i=0;i<IMP_MAX_INPUTS;i++)
		input_trg[i] = input_val[i];
}

/*
	req_vectorized_direction() -> vec2
	purpose: calculate directional movement input, joining stick and 8-way directions
	returns: normalized movement direction
*/
glm::vec2 InputMap::req_vectorized_direction()
{
	// calculate input directions separately
	bool dz_caxis = (glm::abs(*cnt_lraxis)+glm::abs(*cnt_udaxis))>IMP_CONTROLLER_DEADZONE;
	glm::vec2 dir = glm::vec2(*cnt_lraxis,*cnt_udaxis)*glm::vec2(dz_caxis);
	dir = glm::vec2(dir.x/IMP_CONTROLLERCAP,-dir.y/IMP_CONTROLLERCAP);
	glm::vec2 kdir = glm::vec2(*key_actions[IMP_REQRIGHT]-*key_actions[IMP_REQLEFT],
		*key_actions[IMP_REQUP]-*key_actions[IMP_REQDOWN]);

	// combine & unify direction speed
	dir = glm::vec2(dir.x*!kdir.x+kdir.x*!!kdir.x,dir.y*!kdir.y+kdir.y*!!kdir.y);
	dir = (dir.x||dir.y) ? glm::normalize(dir) : glm::vec2(0);
	return dir;
}

/*
	PARAMETER DEFINITIONS:
	request_id: holding id by constexpr to identify requested input
*/

/*
	request(uint8_t) -> bool
	purpose: calculate if requested input has been made with any possible peripheral
	returns: if requested input has been made
*/
bool InputMap::request(uint8_t request_id)
{ return *key_actions[request_id]||*cnt_actions[request_id]; }

/*
	precalculate(uint8_t) -> void
	purpose: precalculate if requested input has been made and store result for repeated usage
*/
void InputMap::precalculate(uint8_t request_id)
{ input_val[request_id] = request(request_id); }

/*
	precalculate_vector() -> void
	purpose: precalculate movement direction and store result for repeated usage
*/
void InputMap::precalculate_vector()
{ move_dir = req_vectorized_direction(); }

/*
	precalculate_dpad() -> void
	purpose: precalculate all directional inputs that relate to dpad functionality
*/
void InputMap::precalculate_dpad()
{
	for (uint8_t i=10;i<IMP_MAX_INPUTS;i++)
		precalculate(i);
}

/*
	precalculate_all() -> void
	purpose: precalculate inputs for all actions ans store results for repeated usage
*/
void InputMap::precalculate_all()
{
	precalculate_vector();
	for (uint8_t i=0;i<IMP_MAX_INPUTS;i++) precalculate(i);
}