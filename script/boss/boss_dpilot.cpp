#include "boss_dpilot.h"

/*
	REGISTER DEFINITIONS:
	 0 : ex var counting frames in present phase
	 1 : cooldown frames left for flaredrop attack
	 2 : movement direction multiplier
	 3 : movement stage counter
	 4 : cooldown frames left for mine wall attack
	 5 : loopcount flarespray attack
	 6 : cooldown frames left for whirlpool spawn
	 7 : cooldown frames left for directional spawn
	 8 : bullet spray chunk counter directionals
	 9 : bullet cluster index id
	10 : negative boss health modifier
	11 : negative player health modifier
	12 : player iframes
	13 : player x
	14 : player y
*/

/*
	load(CascabelBaseFeature*,uint32_t&,int32_t*) -> void (static)
	conforming to: stg_ld
	purpose: load the essentials for the dancing pilot fight and set initial register values
*/
void BossDPilot::load(CascabelBaseFeature* ccbf,uint32_t &rnd_index,int32_t* treg)
{
	// visuals
	rnd_index = ccbf->r3d->add("./res/flyfighter.obj","./res/flyfighter_tex.png","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,0,0),18,glm::vec3(-90,0,0));

	// danmaku
	treg[9] = ccbf->bSys->add_cluster(15,15,2048,"./res/test_bullet_anim.png",2,3,6,30);
	ccbf->bSys->add_cluster(50,50,2048,"./res/bllt_norm.png",1,1,1,30);
	ccbf->bSys->add_cluster(30,30,1024,"./res/bllt_ffdir.png",1,1,1,30);
	//ccbf->bSys->add_cluster(12,12,2048,"./res/fast_bullet.png",1,1,1,30);

	// registers
	treg[2]  = 1;			// set initial direction multiplier to positive value
	treg[5]  = rand()%3+4;	// set flarespray stall counter to 4-2 frames
	treg[7]  = rand()%2+3;	// set directional spawn cooldown to 3-4 frames
	treg[12] = 12;			// set 12 iframes for player after hit
}

/*
	update(CascabelBaseFeature*,uint32_t&,glm::vec2,int32_t*) -> void (static)
	conforming to: stg_upd
	purpose: update boss fight circumstances and patterns for the dancing pilot fight
*/
void BossDPilot::update(CascabelBaseFeature* ccbf,uint32_t &rnd_index,glm::vec2 &ePos,int32_t* treg)
{
	if (ccbf->frame->get_time_delta()>.1f) {

		// movement
		ePos = glm::vec2(!treg[3])*glm::vec2(615+treg[0],650+treg[2]*20000/(treg[0]/2-100*treg[2])+50)
			+glm::vec2(treg[3])*glm::vec2(615+treg[0],650+treg[0]*treg[0]/2400-150); // B mv
		bool ex_ovfl = treg[0]<-600||treg[0]>600;	// if B mv reached screen width cap
		bool mult_swap = treg[0]*treg[2]>1;
		treg[2] *= -1*mult_swap+1*!mult_swap;		// invert movement direction multiplier
		treg[3] += ex_ovfl-mult_swap;
		treg[0] += (!treg[3]*-4*treg[2]+treg[3]*8*treg[2])*!!treg[5];
		treg[5] -= mult_swap;

		// patterns
		flaredrop(ccbf->bSys,treg,ePos);
		mines(ccbf->bSys,treg,ePos);
		//directional_sweep(ccbf->bSys,treg,ccbf->player->get_pPos(),ePos);
		// whirlpool(ccbf->bSys,treg,ePos);
	}

	// collision check
	treg[10] = ccbf->bSys->get_pHit(0,ePos,35,0);
	uint8_t n1_hit = ccbf->bSys->get_pHit(treg[9],ccbf->player->get_pPos(),0,5);
	uint8_t n2_hit = ccbf->bSys->get_pHit(treg[9]+1,ccbf->player->get_pPos(),0,5);
	uint8_t n3_hit = ccbf->bSys->get_pHit(treg[9]+2,ccbf->player->get_pPos(),0,7);
	bool pHit = (n1_hit||n2_hit||n3_hit)&&treg[12]>11;
	treg[11] = pHit;
	treg[12] -= treg[12]*pHit;
	treg[12]++;

	// visuals
	/*ccbf->r3d->prepare();
	ccbf->r3d->s3d.upload_matrix("model",glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0)));
	ccbf->r3d->render_mesh(rnd_index,rnd_index+1);*/
}

/*
	PARAMETER DEFINITIONS:
	func(BulletSystem*,int32_t*,glm::vec2,glm::vec2) -> void
	bSys: bullet system used by the attacks to spawn patterns
	treg: extern & changable variable register
	pPos: player position in 1280x720 world surface
	ePos: boss position in 1280x720 world surface
*/

/*
	flaredrop(BulletSystem*,int32_t*,glm::vec2) -> void (static)
	purpose: spawns doubled flares behind the jet, falling down parallel to y axis
		also flares spread by moving down in different speeds to take up more
		movable space and thus create a higher threat and reduce dodgability
			Y|_|Y  <-  enemy
			O   O  <-  mines
			O	O
	FIXME: flares are way to big and require to precise movement for a support pattern
	FIXME: flare movement is kinda boring and doesn't look too good right now
*/
void flaredrop(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos)
{
	// downwards movement depending on index
	for (int i=0;i<2048;i++)
		bSys->delta_bltPos(treg[9]+BMEM_FLARES,i,glm::vec2(0,((i%6)+1)*-1));
	// FIXME: count of movement loop
	// ??cut spray while state 1 or continuous

	// flaredrop spawn if cooldown over
	bool no_flares = treg[1]||treg[3];
	for (int i=0+6*no_flares;i<6;i++)
		bSys->spwn_blt(treg[9]+BMEM_FLARES,glm::vec2(ePos.x-10,ePos.y));  // lft flaredrop
	for (int i=0+6*no_flares;i<6;i++)
		bSys->spwn_blt(treg[9]+BMEM_FLARES,glm::vec2(ePos.x+40,ePos.y));  // rgt flaredrop

	// setup random cooldown time if flares spawned
	treg[1] = !no_flares*(rand()%12+4)+no_flares*treg[1];
	treg[1] -= !treg[3];  // tick cooldown frame counter
}

/*
	mines(BulletSystem*,int32_t*,glm::vec2) -> void (static)
	purpose: spawns mines right behind the boss, slowly moving down.
		used to build horizontal walls to force the player character to minimize sideways movement.
	FIXME: i don't really know why, but i know i don't like the current usage
*/
void mines(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos)
{
	// constant downwards movement
	for (int i=0;i<2048;i++)
		bSys->delta_bltPos(treg[9]+BMEM_MINES,i,
				glm::vec2(0,-1+(bSys->get_bltPos(treg[9]+BMEM_MINES,i).y<50)*-100));

	// spawn after cooldown frames ticked down
	for (int i=(treg[4]||!treg[3]);i<1;i++) {
		bSys->spwn_blt(treg[9]+BMEM_MINES,glm::vec2(ePos.x+20,ePos.y-15));
		treg[4] = rand()%3+1;  // FIXME: kick this outside of loop to reduce the amount of rand()
	} treg[4] -= treg[3];  // tick cooldown frame counter
}

/*
	directional_sweep(BulletSystem*,int32_t*,glm::vec2,glm::vec2) -> void (static)
	purpose: spawns a spread of five bullets, where the middle is aimed at current player position.
		this pretty standard pattern reduces the save range of player movability due to encasing.
	FIXME: needed and an obvious but great idea for this fight, but add a spin axis to spawn	
		-> this will cause a prettier pattern and causes more initial confusion
*/
void directional_sweep(BulletSystem* bSys,int32_t* treg,glm::vec2 pPos,glm::vec2 ePos)
{
	// move bullets towards set directions
	bSys->delta_fDir(treg[9]+BMEM_SPREAD);

	// aim bullets in normalized direction at player
	glm::vec2 dPos = ePos+glm::vec2(16.5f);
	glm::vec2 norm = glm::normalize((pPos-glm::vec2(8.5f))-dPos);

	// spawn bullets and set previously calculated direction aimed at player
	for (int i=-2+5*!!treg[7];i<3;i++) {
		glm::vec4 rVec = glm::vec4(norm.x,norm.y,0,0)
				* glm::rotate(glm::mat4(1.0f),i*.175f,glm::vec3(0,0,1));
		bSys->spwn_blt(treg[9]+BMEM_SPREAD,dPos,glm::vec2(7)*glm::vec2(rVec.x,rVec.y),
				Toolbox::calculate_vecangle(glm::vec2(0,-1),glm::vec2(rVec.x,rVec.y))
				* ((pPos.x<=ePos.x)-(pPos.x>ePos.x)));
		treg[8]--;
		treg[7] = rand()%2+3+30*(treg[8]<1);  // FIXME: same issue with rand() as in function above.
	}

	// counter ticks and reset
	treg[8] += !treg[8]*30;
	treg[7]--;
}

/*
	whirlpool(BulletSystem*,int32_t*,glm::vec2) -> void (static)
	purpose: this is supposed to spread in the opposite direction of enemy movement,
		then it should circle back and a sideways movement of the player is required to dodge
			1st: the walling created by the mines pattern
			2nd: the circular whirlpool pattern emitted at the same time
		this shifts the prediction direction constantly, demanding reorientation of the player.
	FIXME: this is basically a todo, because this whole pattern is trash and not working!
*/
void whirlpool(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos)
{
	for (int i=0;i<2048;i++) {
		float t = 0.1f*bSys->get_ts(treg[9]+BMEM_WHIRL,i);
		bSys->set_bltPos(treg[9]+BMEM_WHIRL,i,
				bSys->get_bltDir(treg[9]+BMEM_WHIRL,i)+glm::vec2(-pow(E,.35f*t)*glm::cos(t),
				pow(E,.35f*t)*glm::sin(t)));
	} bSys->inc_tick(treg[9]+BMEM_WHIRL);
	for (int i=0;i<1;i++)
		bSys->spwn_blt(treg[9]+BMEM_WHIRL,ePos,ePos);
}