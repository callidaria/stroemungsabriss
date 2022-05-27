#include "player.h"

Player::Player(Frame* f,Renderer2D* r2d,RendererI* rI)
	: m_frame(f),m_r2d(r2d),m_rI(rI)
{
	// setup graphics
	ri = m_r2d->add(glm::vec2(0,0),50,50,"res/flyfighter.png");

	// health bar
	hpbuffer = Buffer();
	shp = Shader();
	float hpverts[] = {
		10,10,0,0,10,35,0,0,310,35,0,1,310,35,0,1,310,10,0,2,10,10,0,0,
		10,40,1,0,10,50,1,0,10,50,1,3,10,50,1,3,10,40,1,4,10,40,1,0,
	};
	hpbuffer.bind();
	hpbuffer.upload_vertices(hpverts,sizeof(hpverts));
	shp.compile_hp("shader/fbv_healthbar.shader","shader/fbf_healthbar.shader");
	Camera2D tc2d = Camera2D(1280.0f,720.0f);
	shp.upload_matrix("view",tc2d.view2D);shp.upload_matrix("proj",tc2d.proj2D);

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
		cnt.rng_wide = &f->kb.ka[SDL_SCANCODE_SPACE];
		cnt.rng_focus = &f->kb.ka[SDL_SCANCODE_LALT];
		cnt.bomb = &f->kb.ka[SDL_SCANCODE_F];
		cnt.change = &f->kb.ka[SDL_SCANCODE_C];
		cnt.dash = &f->kb.ka[SDL_SCANCODE_LSHIFT];
		cnt.target = &f->kb.ka[SDL_SCANCODE_RSHIFT];
		cnt.pause = &f->kb.ka[SDL_SCANCODE_ESCAPE];
		cnt.rdetails = &f->kb.ka[SDL_SCANCODE_TAB];
		cnt.qrestart = &f->kb.ka[SDL_SCANCODE_5];
	}
}
Player::~Player() {  }
void Player::update(uint32_t &rstate)
{
	// movement processing
	emulate_vectorized();
	ddur = ddur*!(!drec&&*cnt.dash)+3*(!drec&&*cnt.dash);drec = drec*(ddur!=3)+42*(ddur==3); // dash
	float mvspeed = !ddur*(4+(3*!(*cnt.rng_focus||*cnt.change))); // movement speed based on mode
	glm::vec2 fltmv=glm::normalize(glm::vec2(*cnt.flt_lr*(abs(*cnt.flt_lr)>dz_epsilon), // vectorized
			*cnt.flt_ud*(abs(*cnt.flt_ud)>dz_epsilon)*-1));
	bool t_real = fltmv.x==fltmv.x;
	glm::vec2 absmv=glm::normalize(glm::vec2((*cnt.abs_right)-(*cnt.abs_left), // absolute
			(*cnt.abs_up)-(*cnt.abs_down)));
	bool is_real = absmv.x==absmv.x;
	glm::vec2 mvdir = (t_real?fltmv:glm::vec2(0))+glm::vec2(!t_real)*(is_real?absmv:glm::vec2(0)); // cdirs
	pos += glm::vec3(mvdir.x*mvspeed,mvdir.y*mvspeed,0); // adding directional input
	dhold = dhold*glm::vec2(ddur!=3)+mvdir*glm::vec2(ddur==3);
	pos += glm::vec3(ddur>0)*glm::vec3(mvdir.x*37,mvdir.y*37,0); // adding dash
	pos += glm::vec3((pos.x<0)*-pos.x-(pos.x>1230)*(pos.x-1230),
		(pos.y<0)*-pos.y-(pos.y>670)*(pos.y-670),pos.z); // mvmt bounds
	ddur-=ddur>0;drec-=drec>0; // dash reset
	// FIXME: ??does the ternary flush the pipeline and if so how badly !!yes, very badly

	rng_flib.at(0+((*cnt.rng_focus||*cnt.rng_wide)&&!*cnt.change)+2*(*cnt.change))(m_rI); // TODO: reassert

	// TODO: bombs
	// TODO: shot modes and spawn
	// TODO: close quarters

	// rendering pchar
	m_r2d->sl.at(ri).model = glm::translate(glm::mat4(1.0f),pos);
	m_r2d->prepare();m_r2d->render_sprite(ri,ri+1);

	// rendering health bar
	shp.enable();
	hpbuffer.bind();
	float engbar_dist = 200.0f*((42.0f-drec)/42.0f);
	shp.upload_float("edgediv[3]",engbar_dist);shp.upload_float("edgediv[4]",engbar_dist);
	glDrawArrays(GL_TRIANGLES,0,12);
}
glm::vec2 Player::get_pPos() { return glm::vec2(pos.x,pos.y); }
void Player::emulate_vectorized()
{
	// FIXME: exclude 2 subs if keyboard is not primary input
	emuflt_ud = m_frame->kb.ka[SDL_SCANCODE_S]-m_frame->kb.ka[SDL_SCANCODE_W];
	emuflt_lr = m_frame->kb.ka[SDL_SCANCODE_D]-m_frame->kb.ka[SDL_SCANCODE_A];
}
void Player::jet_wait(RendererI* rI)
{
	// TODO: cool waiting animation and muzzle smoke particles
}
void Player::jet_wide(RendererI* rI)
{
	// TODO: wideshot calculations
}
void Player::jet_focus(RendererI* rI)
{
	// TODO: focused projectile shot
}
void Player::jet_scientific(RendererI* rI)
{
	// TODO: scientific shot
}
