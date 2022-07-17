#include "player.h"

/*
	constructor(Frame*,Renderer2D*,RendererI*,BulletSystem*)
	f: the frame the player is shown in
	r2d: renderer to handle visualization of player character and other controllable entities
	rI: index renderer to handle the particles emitted by the player character
	bSys: bullet system to spawn player projectiles with
	purpose: setting up the player object to be able to control and visualize pc later
*/
Player::Player(Frame* f,Renderer2D* r2d,RendererI* rI,BulletSystem* bsys)
	: m_frame(f),m_r2d(r2d),m_rI(rI),m_bsys(bsys)
{
	// setup player character visualization
	ri = m_r2d->add(glm::vec2(0,0),50,50,"res/flyfighter.png");

	// add pc projectiles to bullet system
	m_bsys->add_cluster(15,15,4096,"./res/hntblt.png");
	treg[2] = 9;

	// adressing indexable interaction methods
	rng_flib.push_back(&jet_wait);
	rng_flib.push_back(&jet_wide);
	rng_flib.push_back(&jet_focus);
	rng_flib.push_back(&jet_scientific);

	// setup controlling
	if (f->m_gc.size()>0) {
		// TODO: dynamic controller choosing
		// TODO: dynamic controller and keyboard config
		dz_epsilon = 5000; // FIXME: set static epsilon value to dynamic from ini
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
		cnt.rng_wide = (bool*)&f->xb.at(0).xba[SDL_CONTROLLER_AXIS_TRIGGERRIGHT];
		cnt.rng_focus = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_A];
		cnt.bomb = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_B];
		cnt.change = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_Y];
		cnt.dash = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_X];
		cnt.target = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_RIGHTSTICK];
		cnt.pause = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_START];
		cnt.rdetails = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_BACK];
		cnt.qrestart = (bool*)&f->xb.at(0).xba[SDL_CONTROLLER_AXIS_TRIGGERLEFT];
	} else {
		// TODO: define in case of keyboard
		emulate_vectorized();
		cnt.flt_ud = &emuflt_ud;
		cnt.flt_lr = &emuflt_lr;
		cnt.abs_up = &f->kb.ka[SDL_SCANCODE_UP];
		cnt.abs_down = &f->kb.ka[SDL_SCANCODE_DOWN];
		cnt.abs_left = &f->kb.ka[SDL_SCANCODE_LEFT];
		cnt.abs_right = &f->kb.ka[SDL_SCANCODE_RIGHT];
		cnt.cq_def = &f->kb.ka[SDL_SCANCODE_Q];
		cnt.cq_atk = &f->kb.ka[SDL_SCANCODE_E];
		cnt.rng_wide = &f->kb.ka[SDL_SCANCODE_C];
		cnt.rng_focus = &f->kb.ka[SDL_SCANCODE_Z];
		cnt.bomb = &f->kb.ka[SDL_SCANCODE_F];
		cnt.change = &f->kb.ka[SDL_SCANCODE_V];
		cnt.dash = &f->kb.ka[SDL_SCANCODE_LSHIFT];
		cnt.target = &f->kb.ka[SDL_SCANCODE_RSHIFT];
		cnt.pause = &f->kb.ka[SDL_SCANCODE_ESCAPE];
		cnt.rdetails = &f->kb.ka[SDL_SCANCODE_TAB];
		cnt.qrestart = &f->kb.ka[SDL_SCANCODE_5];
	}
} Player::~Player() {  }

/*
	update(uint32_t&,int32_t,int32_t) -> void
	rstate: render state to derive player character visualization, animation and handling from
	pDmg: outside player damage to reduce health and healthbar length
	purpose: to update the players state and render position, visualization & health bar
*/
void Player::update(uint32_t &rstate,int32_t pDmg)
{
	// movement processing 8-way to vectorized
	emulate_vectorized();

	// movement speed based on shot and focus mode
	float mvspeed = !ddur*(4+(3*!(*cnt.rng_focus||*cnt.change)));

	// calculate movement in direction of stick or 8-way input
	glm::vec2 fltmv=glm::normalize(glm::vec2(*cnt.flt_lr*(abs(*cnt.flt_lr)>dz_epsilon), // vectorized
			*cnt.flt_ud*(abs(*cnt.flt_ud)>dz_epsilon)*-1));
	bool t_real = fltmv.x==fltmv.x;
	glm::vec2 absmv=glm::normalize(glm::vec2((*cnt.abs_right)-(*cnt.abs_left), // absolute
			(*cnt.abs_up)-(*cnt.abs_down)));
	bool is_real = absmv.x==absmv.x;
	glm::vec2 mvdir = (t_real?fltmv:glm::vec2(0))+glm::vec2(!t_real)*(is_real?absmv:glm::vec2(0));
	// FIXME: find something better than the ternary every frame

	// change position of player based on calculated movement direction
	pos += glm::vec3(mvdir.x*mvspeed,mvdir.y*mvspeed,0);
	dhold = dhold*glm::vec2(ddur!=3)+mvdir*glm::vec2(ddur==3);  // FIXME: remove dash bullshit

	// force player in-bounds
	pos += glm::vec3((pos.x<0)*-pos.x-(pos.x>1230)*(pos.x-1230),	// vertical bounds
			(pos.y<0)*-pos.y-(pos.y>670)*(pos.y-670),pos.z);		// horizontal bounds

	// write player position to register
	treg[0] = pos.x;
	treg[1] = pos.y;

	// update bullet position by current direction
	m_bsys->delta_fDir(0);

	// run requested shot type or idle
	rng_flib.at(0+(*cnt.rng_wide&&!*cnt.rng_focus)+2*(*cnt.rng_focus))(m_bsys,treg);

	// TODO: bombs
	// TODO: shot modes and spawn
	// TODO: close quarters

	// render and move player character
	m_r2d->sl.at(ri).model = glm::translate(glm::mat4(1.0f),pos);
	m_r2d->prepare();
	m_r2d->render_sprite(ri,ri+1);

	// render health bar
	hbar.register_damage(pDmg);
	hbar.render();
}

/*
	get_pPos() -> vec2
	returns: current player position
*/
glm::vec2 Player::get_pPos()
{
	return glm::vec2(pos.x,pos.y);
}

/*
	emulate_vectorized() -> void
	purpose: convert 8-way input to dynamic vector produced by stick input
*/
void Player::emulate_vectorized()
{
	// FIXME: exclude 2 subs if keyboard is not primary input
	emuflt_ud = m_frame->kb.ka[SDL_SCANCODE_S]-m_frame->kb.ka[SDL_SCANCODE_W];
	emuflt_lr = m_frame->kb.ka[SDL_SCANCODE_D]-m_frame->kb.ka[SDL_SCANCODE_A];
}

/*
	jet_wait(BulletSystem*,int32_t*) -> void
	conforming to: rng_flib
	purpose: handle the jet flight movement idle state
*/
void Player::jet_wait(BulletSystem* bsys,int32_t* treg)
{
	// TODO: cool waiting animation and muzzle smoke particles
}

/*
	jet_wide(BulletSystem*,int32_t*) -> void
	conforming to: rng_flib
	purpose: handle wideshot for fighter jet
*/
void Player::jet_wide(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10+50*(treg[2]<5);i<11;i++) {
		glm::vec4 rVec = glm::vec4(0,1,0,0)*glm::rotate(glm::mat4(1.0f),i*.075f,glm::vec3(0,0,1));
		bsys->spwn_blt(0,glm::vec2(treg[0]+20,treg[1]+55),glm::vec2(rVec.x,rVec.y)*glm::vec2(12));
	} treg[2]--;
	treg[2] += (treg[2]<1)*9;
}

/*
	jet_focus(BulletSystem*,int32_t*) -> void
	conforming to: rng_flib
	purpose: handle focus shot for fighter jet
*/
void Player::jet_focus(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10;i<11;i++)
		bsys->spwn_blt(0,glm::vec2(treg[0]+20+i*7,treg[1]+55),glm::vec2(0,1)*glm::vec2(12));
}

/*
	jet_scientific(BulletSystem*,int32_t*) -> void
	conforming to: rng_flib
	purpose: handle chosen secondary shot for fighter jet
*/
void Player::jet_scientific(BulletSystem* bsys,int32_t* treg)
{
	// TODO: second shot type
}
