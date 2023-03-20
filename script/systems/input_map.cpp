#include "input_map.h"

/*
	TODO
*/
InputMap::InputMap(Frame* frame)
	: m_frame(frame)
{
	map_keyboard();
	map_controller();
}

/*
	TODO
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
	TODO
*/
void InputMap::map_controller()
{
	// controller has been plugged in
	if (m_frame->m_gc.size()) {
		cnt_udaxis = &m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_LEFTX];
		cnt_lraxis = &m_frame->xb[0].xba[SDL_CONTROLLER_AXIS_LEFTY];
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
	}

	// no controller input
	else {
		cnt_udaxis = &rpl_int;
		cnt_lraxis = &rpl_int;
		cnt_actions[IMP_REQWIDE] = &rpl_bool;
		cnt_actions[IMP_REQFOCUS] = &rpl_bool;
		cnt_actions[IMP_REQCQCDEF] = &rpl_bool;
		cnt_actions[IMP_REQCQCATK] = &rpl_bool;
		cnt_actions[IMP_REQBOMB] = &rpl_bool;
		cnt_actions[IMP_REQCHANGE] = &rpl_bool;
		cnt_actions[IMP_REQTARGET] = &rpl_bool;
		cnt_actions[IMP_REQPAUSE] = &rpl_bool;
		cnt_actions[IMP_REQDETAILS] = &rpl_bool;
		cnt_actions[IMP_REQRESTART] = &rpl_bool;
	}
}

/*
	TODO
*/
glm::vec2 InputMap::req_vectorized_direction()
{
	// calculate input directions separately
	glm::vec2 dir
		= glm::vec2((float)*cnt_lraxis/IMP_CONTROLLERCAP,(float)*cnt_udaxis/IMP_CONTROLLERCAP);
	glm::vec2 kdir = glm::vec2(key_actions[IMP_REQRIGHT]-key_actions[IMP_REQLEFT],
		key_actions[IMP_REQUP]-key_actions[IMP_REQDOWN]);

	// combine & unify direction speed
	dir = glm::vec2(dir.x*!kdir.x+kdir.x*!!kdir.x,dir.y*!kdir.y+kdir.y*!!kdir.y);
	dir = glm::normalize(dir);
	return dir;
}

/*
	TODO
*/
bool InputMap::request(uint8_t request_id)
{ return *key_actions[request_id]||*cnt_actions[request_id]; }

/*
	TODO
*/
bool InputMap::req_up()
{ return *key_actions[IMP_REQUP]||(*cnt_udaxis>5000); }
bool InputMap::req_down()
{ return *key_actions[IMP_REQDOWN]||(*cnt_udaxis<-5000); }
bool InputMap::req_left()
{ return *key_actions[IMP_REQLEFT]||(*cnt_lraxis<-5000); }
bool InputMap::req_right()
{ return *key_actions[IMP_REQRIGHT]||(*cnt_lraxis>5000); }