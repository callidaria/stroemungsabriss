#include "boss_dpilot.h"

/*
	REGISTER DEFINITIONS:
	0: ex var counting frames in present phase
	1: cooldown frames left for discspawning in raindrop mines attack
	2: movement direction multiplier
	3: movement stage counter
*/

void BossDPilot::load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg)
{
	// VISUALS
	rnd_index = r2d->add(glm::vec2(0,0),50,50,"./res/flyfighter.png");

	// DANMAKU
	bSys->add_cluster(21,21,2048,"./res/bllt_norm.png");
	treg[2] = 1;
}
void BossDPilot::update(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,glm::vec2 pPos,glm::vec2 ePos,
				int32_t* treg)
{
	// MOVEMENT
	ePos = glm::vec2(!treg[3])*glm::vec2(615+treg[0],650+treg[2]*20000/(treg[0]/2-100*treg[2])+50)
		+glm::vec2(treg[3])*glm::vec2(615+treg[0],650+treg[0]*treg[0]/2400-150); // B mv
	bool ex_ovfl = treg[0]<-600||treg[0]>600; // if B mv reached screen width cap
	bool mult_swap = treg[0]*treg[2]>1;
	treg[2]*=-1*mult_swap+1*!mult_swap; // invert movement direction multiplier
	treg[3]+=ex_ovfl-mult_swap;
	treg[0]+=!treg[3]*-4*treg[2]+treg[3]*8*treg[2];

	// VISUALS
	r2d->prepare();
	r2d->sl.at(rnd_index).model = glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0));
	r2d->render_sprite(rnd_index,rnd_index+1);

	// DANMAKU
	for (int i=0;i<bSys->get_bCount(0);i++) bSys->delta_bltPos(0,i,glm::vec2(0,((i%6)+1)*-1));
	// FIXME: count of movement loop
	for (int i=0+6*(treg[1]!=0);i<6;i++) bSys->spwn_blt(0,glm::vec2(ePos.x-10,ePos.y));
	for (int i=0+6*(treg[1]!=0);i<6;i++) {
		bSys->spwn_blt(0,glm::vec2(ePos.x+40,ePos.y));
		treg[1]=rand()%15+1;
	} treg[1]--;
	// FIXME: randspamming
	bSys->render();
}
