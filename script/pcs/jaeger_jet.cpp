#include "jaeger_jet.h"


typedef void (*jet_movement_logic)(BulletSystem*,int32_t*);

/*
	jet_wait(BulletSystem*,int32_t*) -> void (private,static)
	conforming to: rng_flib
	purpose: handle the jet flight movement idle state
*/
void jet_wait(BulletSystem* bsys,int32_t* treg)
{
	// TODO: cool waiting animation and muzzle smoke particles
}

/*
	jet_wide(BulletSystem*,int32_t*) -> void (private,static)
	conforming to: rng_flib
	purpose: handle wideshot for fighter jet
*/
void jet_wide(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10+50*(treg[2]<5);i<11;i++) {
		float rot = i*.075f;
		glm::vec4 rVec = glm::vec4(0,1,0,0)*glm::rotate(glm::mat4(1.0f),rot,glm::vec3(0,0,1));
		bsys->spwn_blt(0,glm::vec2(treg[0]-7,treg[1]+10),glm::vec2(rVec.x,rVec.y)*glm::vec2(12),rot);
	} treg[2]--;
	treg[2] += (treg[2]<1)*9;
}

/*
	jet_focus(BulletSystem*,int32_t*) -> void (private,static)
	conforming to: rng_flib
	purpose: handle focus shot for fighter jet
*/
void jet_focus(BulletSystem* bsys,int32_t* treg)
{
	for (int i=-10;i<11;i++)
		bsys->spwn_blt(0,glm::vec2(treg[0]-7+i*7,treg[1]+10),glm::vec2(0,1)*glm::vec2(12),0);
}

/*
	jet_scientific(BulletSystem*,int32_t*) -> void (private,static)
	conforming to: rng_flib
	purpose: handle chosen secondary shot for fighter jet
*/
void jet_scientific(BulletSystem* bsys,int32_t* treg)
{
	// TODO: second shot type
}

// pointers to movement logic modes
jet_movement_logic rng_flib[] = {
	jet_wait,
	jet_wide,
	jet_focus,
	jet_scientific
};


/*
	constructor()
	purpose: setting up the player object to be able to control and visualize pc later
*/
JaegerJet::JaegerJet()
{
	// setup player hitbox indicator
	/*
	index_r2D = Core::gRenderer.add_sprite(0,"./res/hitbox_def.png");
	Core::gRenderer.register_sprite(0,index_r2D,glm::vec2(0,0),10,10);
	*/

	// setup player character visualization
	index_r3D = Core::gR3D.add("./res/flyfighter.obj","./res/flyfighter_tex.png","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,0,0),18,glm::vec3(-90,0,0));

	// add pc projectiles to bullet system
	Base::gBSys.add_cluster(15,15,4096,"./res/hntblt.png",1,1,1,30);
	treg[2] = 9;
}

/*
	update() -> void (virtual)
	purpose: update the players state and render position, visualization & health bar
*/
void JaegerJet::update()
{
	// movement speed and direction based on shot and focus mode
	glm::vec2 mvdir = Base::gIMap.move_dir;
	float mvspeed = !ddur*(4+(3*!(
			Base::gIMap.input_val[InputID::FOCUS]||Base::gIMap.input_val[InputID::CHANGE])));

	// change position of player based on calculated movement direction
	position += glm::vec3(Base::gIMap.move_dir.x*mvspeed,Base::gIMap.move_dir.y*mvspeed,0)
			* glm::vec3(Core::gFrame.time_delta);

	// force player in-bounds
	uint16_t x_full_cap = 1280-JET_BORDER_WIDTH;
	uint16_t y_full_cap = 720-JET_BORDER_HEIGHT;
	position += glm::vec3((position.x<JET_BORDER_WIDTH)*(JET_BORDER_WIDTH-position.x)
			- (position.x>x_full_cap)*(position.x-x_full_cap),
			(position.y<JET_BORDER_HEIGHT)*(JET_BORDER_HEIGHT-position.y)
			- (position.y>y_full_cap)*(position.y-y_full_cap),position.z);

	// update bullet position by current direction & write position to register
	Base::gBSys.delta_fDir(0);
	treg[0] = position.x, treg[1] = position.y;

	// run requested shot type or idle
	uint8_t sidx = ((Base::gIMap.input_val[InputID::WIDE]&&!Base::gIMap.input_val[InputID::FOCUS])+2
			* Base::gIMap.input_val[InputID::FOCUS])*(Core::gFrame.time_delta>.1f);
	rng_flib[sidx](&Base::gBSys,treg);
	// TODO: remove bullet system pass. it will be global

	// TODO: bombs
	// TODO: shot modes and spawn
	// TODO: close quarters

	// calculate player jet tilt
	bool abs_right = Base::gIMap.input_val[InputID::RIGHT];
	bool abs_left = Base::gIMap.input_val[InputID::LEFT];
	tilt += (abs_right*5*(tilt<30)-abs_left*5*(tilt>-30))*Core::gFrame.time_delta;
	tilt += (((tilt<0)-(tilt>0))*5*(!abs_left&&!abs_right))*Core::gFrame.time_delta;
	glm::mat4 mdrot = glm::rotate(glm::mat4(1.0f),glm::radians(tilt),glm::vec3(0,1,0));

	// render and move player character
	Core::gR3D.prepare();
	Core::gR3D.s3d.upload_matrix("model",glm::translate(glm::mat4(1.0f),position)*mdrot);
	Core::gR3D.render_mesh(index_r3D,index_r3D+1);

	// render player hitbox indicator
	//Core::gRenderer.prepare_sprites();
	/*
	Core::gRenderer.bfr_sprite[0].sprites[index_r2D].transform.model = glm::mat4(1.f);
	Core::gRenderer.bfr_sprite[0].sprites[index_r2D].transform.translate(position-glm::vec3(5,5,0));
	*/
	//Core::gRenderer.render_sprite(index_r2D);

	// render health bar
	/*hbar.register_damage(pDmg);
	hbar.render();*/
}
