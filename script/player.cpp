#include "player.h"

Player::Player(Frame* f,Renderer2D* r2d)
	: m_r2d(r2d)
{
	// setup graphics
	ri = m_r2d->add(glm::vec2(10,10),50,50,"res/flyfighter.png");

	// setup controlling
	if (f->m_gc.size()>0) {
		// TODO: dynamic controller choosing
		// TODO: dynamic controller and keyboard config
		cnt.flt_lr = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_LEFTX];
		cnt.flt_ud = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_LEFTY];
		cnt.prc_lr = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_RIGHTX];
		cnt.prc_ud = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_RIGHTY];
		cnt.abs_up = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_UP];
		cnt.abs_down = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
		cnt.abs_left = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
		cnt.abs_right = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
		cnt.cq_def = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_LEFTSHOULDER];
		cnt.cq_atk = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_RIGHTSHOULDER];
		cnt.rng_wide = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_TRIGGERRIGHT];
		cnt.rng_focus = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_A];
		cnt.bomb = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_B];
		cnt.change = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_Y];
		cnt.dash = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_X];
		cnt.target = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_RIGHTSTICK];
		cnt.pause = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_START];
		cnt.rdetails = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_BACK];
		cnt.qrestart = &f->xb.at(0).xba[SDL_CONTROLLER_AXIS_TRIGGERLEFT];
	} else {
		// TODO: define in case of keyboard
	}
}
Player::~Player() {  }
void Player::update()
{
	// movement processing
	glm::vec2 fltmv=glm::normalize(glm::vec2(*cnt.flt_lr*(abs(*cnt.flt_lr)>7000),
			*cnt.flt_ud*(abs(*cnt.flt_ud)>7000)*-1)); // TODO: epsilon and ini dead zones
	bool t_real = fltmv.x==fltmv.x;
	pos += glm::vec3(t_real?fltmv.x*7:0,t_real?fltmv.y*7:0,0); // vectorized
	glm::vec2 absmv=glm::normalize(glm::vec2((*cnt.abs_right)-(*cnt.abs_left),(*cnt.abs_up)-(*cnt.abs_down)));
	bool is_real = absmv.x==absmv.x;
	pos += glm::vec3(!t_real)*glm::vec3(is_real?absmv.x*7:0,is_real?absmv.y*7:0,0); // absolute
	// ??does the ternary flush the pipeline
	// TODO: macro or constant the movement speed

	// rendering pchar
	m_r2d->sl.at(ri).model = glm::translate(glm::mat4(1.0f),pos);
	m_r2d->prepare();m_r2d->render_sprite(ri,ri+1);
}
