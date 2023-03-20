#include "player.h"

/*
	constructor(Frame*,Renderer3D*,RendererI*,BulletSystem*)
	f: the frame the player is shown in
	r3d: renderer to handle visualization of player character and other controllable entities
	rI: index renderer to handle the particles emitted by the player character
	bSys: bullet system to spawn player projectiles with
	purpose: setting up the player object to be able to control and visualize pc later
*/
Player::Player(Frame* f,Renderer2D* r2d,Renderer3D* r3d,RendererI* rI,BulletSystem* bsys,
		InputMap* input_map)
	: m_frame(f),m_r2d(r2d),m_r3d(r3d),m_rI(rI),m_bsys(bsys),imap(input_map)
{
	// setup player hitbox indicator
	aidx = m_r2d->add(glm::vec2(0,0),10,10,"./res/hitbox_def.png");

	// setup player character visualization
	ridx = m_r3d->add("./res/flyfighter.obj","./res/flyfighter_tex.png","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,0,0),18,glm::vec3(-90,0,0));

	// add pc projectiles to bullet system
	m_bsys->add_cluster(15,15,4096,"./res/hntblt.png",1,1,1,30);
	treg[2] = 9;

	// adressing indexable interaction methods
	rng_flib.push_back(&jet_wait);
	rng_flib.push_back(&jet_wide);
	rng_flib.push_back(&jet_focus);
	rng_flib.push_back(&jet_scientific);
} Player::~Player() {  }

/*
	update(uint32_t&,int32_t,int32_t) -> void
	rstate: render state to derive player character visualization, animation and handling from
	pDmg: outside player damage to reduce health and healthbar length
	purpose: to update the players state and render position, visualization & health bar
*/
void Player::update(uint32_t &rstate,int32_t pDmg)
{
	// movement speed and direction based on shot and focus mode
	glm::vec2 mvdir = imap->move_dir;
	float mvspeed = !ddur*(4+(3*!(imap->input_val[IMP_REQFOCUS]||imap->input_val[IMP_REQCHANGE])));

	// change position of player based on calculated movement direction
	pos += glm::vec3(imap->move_dir.x*mvspeed,imap->move_dir.y*mvspeed,0)
			* glm::vec3(m_frame->get_time_delta());

	// force player in-bounds
	uint16_t x_full_cap = 1280-JET_BORDER_WIDTH;
	uint16_t y_full_cap = 720-JET_BORDER_HEIGHT;
	pos += glm::vec3((pos.x<JET_BORDER_WIDTH)*(JET_BORDER_WIDTH-pos.x)
			- (pos.x>x_full_cap)*(pos.x-x_full_cap),
			(pos.y<JET_BORDER_HEIGHT)*(JET_BORDER_HEIGHT-pos.y)
			- (pos.y>y_full_cap)*(pos.y-y_full_cap),pos.z);

	// write player position to register
	treg[0] = pos.x;treg[1] = pos.y;

	// update bullet position by current direction
	m_bsys->delta_fDir(0);

	// run requested shot type or idle
	uint8_t sidx = ((imap->input_val[IMP_REQWIDE]&&!imap->input_val[IMP_REQFOCUS])+2
			* imap->input_val[IMP_REQFOCUS])*(m_frame->get_time_delta()>.1f);
	rng_flib.at(sidx)(m_bsys,treg);

	// TODO: bombs
	// TODO: shot modes and spawn
	// TODO: close quarters

	// calculate player jet tilt
	bool abs_right = imap->req_right(),abs_left = imap->req_left();
	tilt += (abs_right*5*(tilt<30)-abs_left*5*(tilt>-30))*m_frame->get_time_delta();
	tilt += (((tilt<0)-(tilt>0))*5*(!abs_left&&!abs_right))*m_frame->get_time_delta();
	glm::mat4 mdrot = glm::rotate(glm::mat4(1.0f),glm::radians(tilt),glm::vec3(0,1,0));

	// render and move player character
	m_r3d->prepare();
	m_r3d->s3d.upload_matrix("model",glm::translate(glm::mat4(1.0f),pos)*mdrot);
	m_r3d->render_mesh(ridx,ridx+1);

	// render player hitbox indicator
	m_r2d->prepare();
	m_r2d->sl[aidx].model = glm::translate(glm::mat4(1.0f),pos-glm::vec3(5,5,0));
	m_r2d->render_sprite(aidx,aidx+1);

	// render health bar
	/*hbar.register_damage(pDmg);
	hbar.render();*/
}

/*
	get_pPos() -> vec2
	returns: current player position
*/
glm::vec2 Player::get_pPos()
{ return glm::vec2(pos.x,pos.y); }

/*
	jet_wait(BulletSystem*,int32_t*) -> void (static)
	conforming to: rng_flib
	purpose: handle the jet flight movement idle state
*/
void Player::jet_wait(BulletSystem* bsys,int32_t* treg)
{
	// TODO: cool waiting animation and muzzle smoke particles
}

/*
	jet_wide(BulletSystem*,int32_t*) -> void (static)
	conforming to: rng_flib
	purpose: handle wideshot for fighter jet
*/
void Player::jet_wide(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10+50*(treg[2]<5);i<11;i++) {
		float rot = i*.075f;
		glm::vec4 rVec = glm::vec4(0,1,0,0)*glm::rotate(glm::mat4(1.0f),rot,glm::vec3(0,0,1));
		bsys->spwn_blt(0,glm::vec2(treg[0]-7,treg[1]+10),glm::vec2(rVec.x,rVec.y)*glm::vec2(12),rot);
	} treg[2]--;
	treg[2] += (treg[2]<1)*9;
}

/*
	jet_focus(BulletSystem*,int32_t*) -> void (static)
	conforming to: rng_flib
	purpose: handle focus shot for fighter jet
*/
void Player::jet_focus(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10;i<11;i++)
		bsys->spwn_blt(0,glm::vec2(treg[0]-7+i*7,treg[1]+10),glm::vec2(0,1)*glm::vec2(12),0);
}

/*
	jet_scientific(BulletSystem*,int32_t*) -> void (static)
	conforming to: rng_flib
	purpose: handle chosen secondary shot for fighter jet
*/
void Player::jet_scientific(BulletSystem* bsys,int32_t* treg)
{
	// TODO: second shot type
}