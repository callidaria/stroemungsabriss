#include "player.h"

Player::Player(Frame* f,Renderer2D* r2d)
	: m_r2d(r2d)
{
	// setup graphics
	ri = m_r2d->add(glm::vec2(0,0),50,50,"res/flyfighter.png");

	// health bar
	glGenVertexArrays(1,&hpvao);glGenBuffers(1,&hpvbo);
	shp = Shader();
	float hpverts[] = { 10,10,10,60,210,60,210,60,210,10,10,10 };
	glBindVertexArray(hpvao); // §§??
	glBindBuffer(GL_ARRAY_BUFFER,hpvbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(hpverts),hpverts,GL_STATIC_DRAW);
	shp.compile_hp("shader/fbv_healthbar.shader","shader/fbf_healthbar.shader");
	Camera2D tc2d = Camera2D(1280.0f,720.0f);
	shp.upload_matrix("view",tc2d.view2D);shp.upload_matrix("proj",tc2d.proj2D);

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
	ddur = ddur*!(!drec&&*cnt.dash)+3*(!drec&&*cnt.dash);drec = drec*(ddur!=3)+42*(ddur==3); // dash
	float mvspeed = !ddur*(4+(3*!(*cnt.rng_focus||*cnt.change))); // movement speed based on mode
	glm::vec2 fltmv=glm::normalize(glm::vec2(*cnt.flt_lr*(abs(*cnt.flt_lr)>5000), // vectorized
			*cnt.flt_ud*(abs(*cnt.flt_ud)>5000)*-1)); // TODO: epsilon and ini dead zones
	bool t_real = fltmv.x==fltmv.x;
	glm::vec2 absmv=glm::normalize(glm::vec2((*cnt.abs_right)-(*cnt.abs_left), // absolute
			(*cnt.abs_up)-(*cnt.abs_down)));
	bool is_real = absmv.x==absmv.x;
	glm::vec2 mvdir = (t_real?fltmv:glm::vec2(0))+glm::vec2(!t_real)*(is_real?absmv:glm::vec2(0)); // cdirs
	pos += glm::vec3(mvdir.x*mvspeed,mvdir.y*mvspeed,0); // adding directional input
	dhold = dhold*glm::vec2(ddur!=3)+mvdir*glm::vec2(ddur==3);
	pos += glm::vec3(ddur>0)*glm::vec3(mvdir.x*37,mvdir.y*37,0); // adding dash
	ddur-=ddur>0;drec-=drec>0; // dash reset
	// ??does the ternary flush the pipeline and if so how badly

	// TODO: bombs
	// TODO: shot modes and spawn
	// TODO: close quarters

	// rendering pchar
	m_r2d->sl.at(ri).model = glm::translate(glm::mat4(1.0f),pos);
	m_r2d->prepare();m_r2d->render_sprite(ri,ri+1);

	// rendering health bar
	shp.enable();glBindVertexArray(hpvao);
	glDrawArrays(GL_TRIANGLES,0,6);
}
