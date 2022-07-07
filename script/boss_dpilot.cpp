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
	10 : boss health
	11 : player health
	12 : player iframes
	13 : player x
	14 : player y
*/

/*
	load(Renderer2D*,uint32_t&,BulletSystem*,int32_t*) -> void
	conforming to: stg_ld
	purpose: load the essentials for the dancing pilot fight and set initial register values
*/
void BossDPilot::load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg)
{
	// visuals
	rnd_index = r2d->add(glm::vec2(0,0),50,50,"./res/flyfighter.png");

	// danmaku
	treg[9] = bSys->add_cluster(15,15,2048,"./res/bllt_proj.png");
	bSys->add_cluster(15,15,2048,"./res/bllt_norm.png");
	bSys->add_cluster(17,17,1024,"./res/bllt_ffdir.png");
	bSys->add_cluster(12,12,2048,"./res/fast_bullet.png");

	// registers
	treg[2]  = 1;			// set initial direction multiplier to positive value
	treg[5]  = rand()%3+4;	// set flarespray stall counter to 4-2 frames
	treg[7]  = rand()%2+3;	// set directional spawn cooldown to 3-4 frames
	treg[10] = 5000;		// standard phase health
	treg[12] = 12;			// set 12 iframes for player after hit
}

/*
	update(Renderer2D*,uint32_t&,BulletSystem*,glm::vec2,glm::vec2,int32_t*) -> void
	conforming to: stg_upd
*/
void BossDPilot::update(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,glm::vec2 pPos,glm::vec2 ePos,
		int32_t* treg)
{
	// movement
	ePos = glm::vec2(!treg[3])*glm::vec2(615+treg[0],650+treg[2]*20000/(treg[0]/2-100*treg[2])+50)
		+ glm::vec2(treg[3])*glm::vec2(615+treg[0],650+treg[0]*treg[0]/2400-150); // B mv
	bool ex_ovfl = treg[0]<-600||treg[0]>600; // if B mv reached screen width cap
	bool mult_swap = treg[0]*treg[2]>1;
	treg[2] *= -1*mult_swap+1*!mult_swap; // invert movement direction multiplier
	treg[3] += ex_ovfl-mult_swap;
	treg[0] += (!treg[3]*-4*treg[2]+treg[3]*8*treg[2])*!!treg[5];
	treg[5] -= mult_swap;

	flaredrop(bSys,treg,ePos);
	mines(bSys,treg,ePos);
	directional_sweep(bSys,treg,pPos,ePos);
	// whirlpool(bSys,treg,ePos);

	// collision check
	treg[10] -= bSys->get_pHit(0,ePos,50,50);
	uint8_t n1_hit = bSys->get_pHit(treg[9],pPos,20,20);
	uint8_t n2_hit = bSys->get_pHit(treg[9]+1,pPos,20,20);
	uint8_t n3_hit = bSys->get_pHit(treg[9]+2,pPos,12,12);
	bool pHit = (n1_hit||n2_hit||n3_hit)&&treg[12]>11;
	treg[11] -= pHit;
	treg[12] -= treg[12]*pHit;
	treg[12]++;

	// VISUALS
	r2d->prepare();
	r2d->sl.at(rnd_index).model = glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0));
	r2d->render_sprite(rnd_index,rnd_index+1);
	bSys->render();
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
	flaredrop(BulletSystem*,int32_t*,glm::vec2) -> void
	purpose: spawns doubled flares behind the jet, falling down parallel to y axis
		also flares spread by moving down in different speeds to take up more
		movable space and thus create a higher threat and reduce dodgability
			^|_|^  <-  enemy
			O   O  <-  mines
			O	O
	FIXME: flares are way to big and require to precise movement for a support pattern
	FIXME: flare movement is kinda boring and doesn't look too good right now
*/
void flaredrop(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos)
{
	// downwards movement depending on index
	for (int i=0;i<2048;i++)
		bSys->delta_bltPos(treg[9],i,glm::vec2(0,((i%6)+1)*-1));
	// FIXME: count of movement loop
	// ??cut spray while state 1 or continuous

	// flaredrop spawn if cooldown over
	bool no_flares = treg[1]||treg[3];
	for (int i=0+6*no_flares;i<6;i++)
		bSys->spwn_blt(treg[9],glm::vec2(ePos.x-10,ePos.y));  // lft flaredrop
	for (int i=0+6*no_flares;i<6;i++)
		bSys->spwn_blt(treg[9],glm::vec2(ePos.x+40,ePos.y));  // rgt flaredrop

	// setup random cooldown time if flares spawned
	treg[1] = !no_flares*(rand()%12+4)+no_flares*treg[1];
	treg[1] -= !treg[3];  // tick cooldown frame counter
}

/*
	mines(BulletSystem*,int32_t*,glm::vec2) -> void
	purpose: spawns mines right behind the boss, slowly moving down.
		used to build horizontal walls to force the player character to minimize sideways movement.
	FIXME: i don't really know why, but i know i don't like the current usage
*/
void mines(BulletSystem* bSys,int32_t* treg,glm::vec2 ePos)
{
	// constant downwards movement
	for (int i=0;i<2048;i++)
		bSys->delta_bltPos(treg[9]+1,i,glm::vec2(0,-1+(bSys->get_bltPos(treg[9]+1,i).y<50)*-100));

	// spawn after cooldown frames ticked down
	for (int i=(treg[4]||!treg[3]);i<1;i++) {
		bSys->spwn_blt(treg[9]+1,glm::vec2(ePos.x+20,ePos.y-15));
		treg[4] = rand()%3+1;  // FIXME: kick this outside of loop to reduce the amount of rand()
	} treg[4] -= treg[3];  // tick cooldown frame counter
}

/*
	directional_sweep(BulletSystem*,int32_t*,glm::vec2,glm::vec2) -> void
	purpose: spawns a spread of five bullets, where the middle is aimed at current player position.
		this pretty standard pattern reduces the save range of player movability due to encasing.
	FIXME: needed and an obvious but great idea for this fight, but add a spin axis to spawn	
		-> this will cause a prettier pattern and causes more initial confusion
*/
void directional_sweep(BulletSystem* bSys,int32_t* treg,glm::vec2 pPos,glm::vec2 ePos)
{
	// move bullets towards set directions
	bSys->delta_fDir(treg[9]+2);

	// aim bullets in normalized direction at player
	glm::vec2 dPos = ePos+glm::vec2(glm::vec2(16,-17));
	glm::vec2 norm = glm::normalize(glm::vec2(pPos.x+16,pPos.y+17)-dPos);

	// spawn bullets and set previously calculated direction aimed at player
	for (int i=-2+5*!!treg[7];i<3;i++) {
		glm::vec4 rVec = glm::vec4(norm.x,norm.y,0,0)
				*glm::rotate(glm::mat4(1.0f),i*.175f,glm::vec3(0,0,1));
		bSys->spwn_blt(treg[9]+2,dPos,glm::vec2(7)*glm::vec2(rVec.x,rVec.y));
		treg[8]--;
		treg[7] = rand()%2+3+30*(treg[8]<1);  // FIXME: same issue with rand() as in function above.
	}

	// counter ticks and reset
	treg[8] += !treg[8]*30;
	treg[7]--;
}

/*
	whirlpool(BulletSystem*,int32_t*,glm::vec2) -> void
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
		float t = 0.1f*bSys->get_ts(treg[9]+3,i);
		bSys->set_bltPos(treg[9]+3,i,bSys->get_bltDir(treg[9]+3,i)+glm::vec2(-pow(E,.35f*t)*glm::cos(t),
				pow(E,.35f*t)*glm::sin(t)));
	} bSys->inc_tick(treg[9]+3);
	for (int i=0;i<1;i++)
		bSys->spwn_blt(treg[9]+3,ePos,ePos);
}
