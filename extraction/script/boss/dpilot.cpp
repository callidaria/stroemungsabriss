#include "dpilot.h"

/*
	construction()
	conforming to: Boss::Boss()
	purpose: load the essentials for the dancing pilot fight and set initial register values
*/
DPilot::DPilot()
{
	// visuals
	r3d_index = Core::gR3D.add("./res/flyfighter.obj","./res/flyfighter_tex.png","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,0,0),18,glm::vec3(-90,0,0));

	// danmaku
	/*bsys_index = gBSys.add_cluster(15,15,2048,"./res/test_bullet_anim.png",2,3,6,30);
	gBSys.add_cluster(15,15,2048,"./res/bllt_norm.png",1,1,1,30);
	gBSys.add_cluster(17,17,1024,"./res/bllt_ffdir.png",1,1,1,30);*/
	//gBSys.add_cluster(12,12,2048,"./res/fast_bullet.png",1,1,1,30);
}

/*
	update(vec2) -> void (virtual)
	conforming to: Boss::update()
	purpose: update boss fight circumstances and patterns for the dancing pilot fight
*/
void DPilot::update(glm::vec2 pPos)
{
	//if (m_ccbf->frame->time_delta>.1) {

		// movement
		/*ePos = glm::vec2(!mv_stage)
			* glm::vec2(615+ph_fcount,650+dir_mult*20000/(ph_fcount>>1-100*dir_mult)+50)
			+ glm::vec2(mv_stage)*glm::vec2(615+ph_fcount,650+ph_fcount*ph_fcount/2400-150);
		bool ex_ovfl = ph_fcount<-600||ph_fcount>600;
		bool mult_swap = ph_fcount*dir_mult>1;
		dir_mult *= -1*mult_swap+1*!mult_swap;
		mv_stage += ex_ovfl-mult_swap;
		ph_fcount += (!mv_stage*-4*dir_mult+mv_stage*8*dir_mult)*!!cd_flare;
		cd_flare -= mult_swap;*/
	

		// patterns
/*
		flaredrop();
		mines();
		directional_sweep(pPos);
		//whirlpool();
*/
		//}

	// collision check
	health_mod = 0;
	//health_mod = gBSys.get_pHit(0,ePos,35,0);
	/*uint8_t n1_hit = gBSys.get_pHit(bsys_index,m_ccbf->player->get_pPos(),0,5);
	uint8_t n2_hit = gBSys.get_pHit(bsys_index+1,m_ccbf->player->get_pPos(),0,5);
	uint8_t n3_hit = gBSys.get_pHit(bsys_index+2,m_ccbf->player->get_pPos(),0,7);*/
	/*bool pHit = (n1_hit||n2_hit||n3_hit)&&iframes>11;
	iframes -= iframes*pHit;*/
	//iframes++;

	// visuals
	Core::gR3D.prepare();
	Core::gR3D.s3d.upload_matrix("model",
			glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0)));
	Core::gR3D.render_mesh(r3d_index,r3d_index+1);

	// healthbar
	hbar.register_damage(health_mod);
	hbar.render();
}

/*
	flaredrop() -> void (private)
	purpose: spawns doubled flares behind the jet, falling down parallel to y axis
		also flares spread by moving down in different speeds to take up more
		movable space and thus create a higher threat and reduce dodgability
			Y|_|Y  <-  enemy
			O   O  <-  mines
			O	O
	FIXME: flares are way to big and require to precise movement for a support pattern
	FIXME: flare movement is kinda boring and doesn't look too good right now
*/
void DPilot::flaredrop()
{
	// downwards movement depending on index
	for (int i=0;i<2048;i++)
		Base::gBSys.delta_bltPos(bsys_index+DP_FLARES,i,glm::vec2(0,((i%6)+1)*-1));
	// FIXME: count of movement loop
	// ??cut spray while state 1 or continuous

	// flaredrop spawn if cooldown over
	bool no_flares = cd_flares||mv_stage;
	for (int i=0+6*no_flares;i<6;i++)
		Base::gBSys.spwn_blt(bsys_index+DP_FLARES,glm::vec2(ePos.x-10,ePos.y));  // lft flaredrop
	for (int i=0+6*no_flares;i<6;i++)
		Base::gBSys.spwn_blt(bsys_index+DP_FLARES,glm::vec2(ePos.x+40,ePos.y));  // rgt flaredrop

	// setup random cooldown time if flares spawned
	cd_flares = !no_flares*(rand()%12+4)+no_flares*cd_flares;
	cd_flares -= !mv_stage;  // tick cooldown frame counter
}

/*
	mines() -> void (private)
	purpose: spawns mines right behind the boss, slowly moving down.
		used to build horizontal walls to force the player character to minimize sideways movement.
	FIXME: i don't really know why, but i know i don't like the current usage
*/
void DPilot::mines()
{
	// constant downwards movement
	for (int i=0;i<2048;i++)
		Base::gBSys.delta_bltPos(bsys_index+DP_MINES,i,
				glm::vec2(0,-1+(Base::gBSys.get_bltPos(bsys_index+DP_MINES,i).y<50)*-100));

	// spawn after cooldown frames ticked down
	for (int i=(cd_mines||!mv_stage);i<1;i++) {
		Base::gBSys.spwn_blt(bsys_index+DP_MINES,glm::vec2(ePos.x+20,ePos.y-15));
		cd_mines = rand()%3+1;  // FIXME: kick this outside of loop to reduce the amount of rand()
	} cd_mines -= mv_stage;  // tick cooldown frame counter
}

/*
	directional_sweep() -> void (private)
	pPos: current player position
	purpose: spawns a spread of five bullets, where the middle is aimed at current player position.
		this pretty standard pattern reduces the save range of player movability due to encasing.
	FIXME: needed and an obvious but great idea for this fight, but add a spin axis to spawn	
		-> this will cause a prettier pattern and causes more initial confusion
*/
void DPilot::directional_sweep(glm::vec2 pPos)
{
	// move bullets towards set directions
	Base::gBSys.delta_fDir(bsys_index+DP_SPREAD);

	// aim bullets in normalized direction at player
	glm::vec2 dPos = ePos+glm::vec2(16.5f);
	glm::vec2 norm = glm::normalize((pPos-glm::vec2(8.5f))-dPos);

	// spawn bullets and set previously calculated direction aimed at player
	for (int i=-2+5*!!cd_direction;i<3;i++) {
		glm::vec4 rVec = glm::vec4(norm.x,norm.y,0,0)
				* glm::rotate(glm::mat4(1.0f),i*.175f,glm::vec3(0,0,1));
		Base::gBSys.spwn_blt(bsys_index+DP_SPREAD,dPos,glm::vec2(7)*glm::vec2(rVec.x,rVec.y),
				Toolbox::calculate_vecangle(glm::vec2(0,-1),glm::vec2(rVec.x,rVec.y))
				* ((pPos.x<=ePos.x)-(pPos.x>ePos.x)));
		spray_counter--;
		cd_direction = (rand()&1)+30*(spray_counter<1)+3;
	}
	// FIXME: same issue with rand() as in function above.

	// counter ticks and reset
	spray_counter += !spray_counter*30;
	cd_direction--;
}

/*
	whirlpool() -> void (private)
	purpose: this is supposed to spread in the opposite direction of enemy movement,
		then it should circle back and a sideways movement of the player is required to dodge
			1st: the walling created by the mines pattern
			2nd: the circular whirlpool pattern emitted at the same time
		this shifts the prediction direction constantly, demanding reorientation of the player.
	FIXME: this is basically a todo, because this whole pattern is trash and not working!
*/
void DPilot::whirlpool()
{
	for (int i=0;i<2048;i++) {
		float t = 0.1f*Base::gBSys.get_ts(bsys_index+DP_WHIRL,i);
		Base::gBSys.set_bltPos(bsys_index+DP_WHIRL,i,
				Base::gBSys.get_bltDir(bsys_index+DP_WHIRL,i)
				+ glm::vec2(-pow(MATH_E,.35f*t)*glm::cos(t),pow(MATH_E,.35f*t)*glm::sin(t)));
	} Base::gBSys.inc_tick(bsys_index+DP_WHIRL);
	for (int i=0;i<1;i++) Base::gBSys.spwn_blt(bsys_index+DP_WHIRL,ePos,ePos);
}
