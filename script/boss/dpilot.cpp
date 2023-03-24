#include "dpilot.h"

/*
	construction(CascabelBaseFeature*,uint32_t&,int32_t*)
	conforming to: Boss::Boss()
	purpose: load the essentials for the dancing pilot fight and set initial register values
*/
DPilot::DPilot(CascabelBaseFeature* ccbf)
	: m_ccbf(ccbf)
{
	// visuals
	r3d_index = m_ccbf->r3d->add("./res/flyfighter.obj","./res/flyfighter_tex.png","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,0,0),18,glm::vec3(-90,0,0));

	// danmaku
	/*bsys_index = m_ccbf->bSys->add_cluster(15,15,2048,"./res/test_bullet_anim.png",2,3,6,30);
	m_ccbf->bSys->add_cluster(15,15,2048,"./res/bllt_norm.png",1,1,1,30);
	m_ccbf->bSys->add_cluster(17,17,1024,"./res/bllt_ffdir.png",1,1,1,30);*/
	//m_ccbf->bSys->add_cluster(12,12,2048,"./res/fast_bullet.png",1,1,1,30);
}

/*
	update(vec2) -> void
	conforming to: Boss::update()
	purpose: update boss fight circumstances and patterns for the dancing pilot fight
*/
void DPilot::update(glm::vec2 pPos)
{
	if (m_ccbf->frame->get_time_delta()>.1f) {

		// movement
		ePos = glm::vec2(!mv_stage)
			* glm::vec2(615+ph_fcount,650+dir_mult*20000/(ph_fcount/2-100*dir_mult)+50)
			+ glm::vec2(mv_stage)*glm::vec2(615+ph_fcount,650+ph_fcount*ph_fcount/2400-150);
		bool ex_ovfl = ph_fcount<-600||ph_fcount>600;
		bool mult_swap = ph_fcount*dir_mult>1;
		dir_mult *= -1*mult_swap+1*!mult_swap;
		mv_stage += ex_ovfl-mult_swap;
		ph_fcount += (!mv_stage*-4*dir_mult+mv_stage*8*dir_mult)*!!cd_flare;
		cd_flare -= mult_swap;

		// patterns
		flaredrop();
		mines();
		directional_sweep();
		//whirlpool();
	}

	// collision check
	/*health_mod = m_ccbf->bSys->get_pHit(0,ePos,35,0);
	uint8_t n1_hit = m_ccbf->bSys->get_pHit(bsys_index,m_ccbf->player->get_pPos(),0,5);
	uint8_t n2_hit = m_ccbf->bSys->get_pHit(bsys_index+1,m_ccbf->player->get_pPos(),0,5);
	uint8_t n3_hit = m_ccbf->bSys->get_pHit(bsys_index+2,m_ccbf->player->get_pPos(),0,7);*/
	/*bool pHit = (n1_hit||n2_hit||n3_hit)&&iframes>11;
	iframes -= iframes*pHit;*/
	//iframes++;

	// visuals
	m_ccbf->r3d->prepare();
	m_ccbf->r3d->s3d.upload_matrix("model",
			glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0)));
	m_ccbf->r3d->render_mesh(r3d_index,r3d_index+1);

	// healthbar
	hbar.render();
}

/*
	flaredrop() -> void
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
	// TODO
}

/*
	mines() -> void
	purpose: spawns mines right behind the boss, slowly moving down.
		used to build horizontal walls to force the player character to minimize sideways movement.
	FIXME: i don't really know why, but i know i don't like the current usage
*/
void DPilot::mines()
{
	// TODO
}

/*
	directional_sweep() -> void
	purpose: spawns a spread of five bullets, where the middle is aimed at current player position.
		this pretty standard pattern reduces the save range of player movability due to encasing.
	FIXME: needed and an obvious but great idea for this fight, but add a spin axis to spawn	
		-> this will cause a prettier pattern and causes more initial confusion
*/
void DPilot::directional_sweep()
{
	// TODO
}

/*
	whirlpool() -> void
	purpose: this is supposed to spread in the opposite direction of enemy movement,
		then it should circle back and a sideways movement of the player is required to dodge
			1st: the walling created by the mines pattern
			2nd: the circular whirlpool pattern emitted at the same time
		this shifts the prediction direction constantly, demanding reorientation of the player.
	FIXME: this is basically a todo, because this whole pattern is trash and not working!
*/
void DPilot::whirlpool()
{
	// TODO
}