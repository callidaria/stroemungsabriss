#include "boss_dpilot.h"

/*
	REGISTER DEFINITIONS:
	0: ex var counting frames in present phase
	1: cooldown frames left for flaredrop attack
	2: movement direction multiplier
	3: movement stage counter
	4: cooldown frames left for mine wall attack
	5: loopcount flarespray attack
	6: cooldown frames left for whirlpool spawn
	7: cooldown frames left for directional spawn
	8: bullet spray chunk counter directionals
	9: bullet cluster index id
	10: boss health
*/

void BossDPilot::load(Renderer2D* r2d,uint32_t &rnd_index,BulletSystem* bSys,int32_t* treg)
{
	// VISUALS
	rnd_index = r2d->add(glm::vec2(0,0),50,50,"./res/flyfighter.png");

	// DANMAKU
	treg[9] = bSys->add_cluster(21,21,2048,"./res/bllt_proj.png");
	bSys->add_cluster(30,30,2048,"./res/bllt_norm.png");
	/*bSys->add_cluster(17,17,1024,"./res/fast_bullet.png");
	bSys->add_cluster(12,12,2048,"./res/bllt_ffdir.png");*/
	bSys->add_cluster(17,17,1024,"./res/bllt_ffdir.png");
	bSys->add_cluster(12,12,2048,"./res/fast_bullet.png");

	// REGISTERS
	treg[2]=1;
	treg[5]=rand()%3+4;
	treg[7]=rand()%2+3;
	treg[10] = 5000;
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
	treg[0]+=(!treg[3]*-4*treg[2]+treg[3]*8*treg[2])*!!treg[5];
	treg[5]-=mult_swap;

	// flaredrop pattern
	for (int i=0;i<2048;i++) bSys->delta_bltPos(treg[9],i,glm::vec2(0,((i%6)+1)*-1)); // flaredrop movement
	// FIXME: count of movement loop
	// ??cut spray while state 1 or continuous
	bool no_flares = treg[1]||treg[3]; // flaredrop spawn
	for (int i=0+6*no_flares;i<6;i++) bSys->spwn_blt(treg[9],glm::vec2(ePos.x-10,ePos.y));
	for (int i=0+6*no_flares;i<6;i++) bSys->spwn_blt(treg[9],glm::vec2(ePos.x+40,ePos.y));
	treg[1]=!no_flares*(rand()%12+4)+no_flares*treg[1];
	treg[1]-=!treg[3];

	// mine pattern
	for (int i=0;i<2048;i++)
		bSys->delta_bltPos(treg[9]+1,i,glm::vec2(0,-1+(bSys->get_bltPos(treg[9]+1,i).y<50)*-100));
	for (int i=(treg[4]!=0||!treg[3]);i<1;i++) {
		bSys->spwn_blt(treg[9]+1,glm::vec2(ePos.x+20,ePos.y-15));
		treg[4]=rand()%3+1;
	} treg[4]-=treg[3];

	// directional pattern
	bSys->delta_fDir(treg[9]+2);
	glm::vec2 dPos = ePos+glm::vec2(glm::vec2(16,-17));
	glm::vec2 norm = glm::normalize(glm::vec2(pPos.x+16,pPos.y+17)-dPos);
	for (int i=-2+5*!!treg[7];i<3;i++) {
		glm::vec4 rVec = glm::vec4(norm.x,norm.y,0,0)
				*glm::rotate(glm::mat4(1.0f),i*.175f,glm::vec3(0,0,1));
		bSys->spwn_blt(treg[9]+2,dPos,glm::vec2(7)*glm::vec2(rVec.x,rVec.y));
		treg[8]--;
		treg[7]=rand()%2+3+30*(treg[8]<1);
	} treg[8]+=!treg[8]*30;
	treg[7]--;

	// whirlpool mesh pattern
	for (int i=0;i<2048;i++) {
		float t = 0.1f*bSys->get_ts(treg[9]+3,i);
		bSys->set_bltPos(treg[9]+3,i,bSys->get_bltDir(treg[9]+3,i)+glm::vec2(-pow(E,.35f*t)*glm::cos(t),
				pow(E,.35f*t)*glm::sin(t)));
	} bSys->inc_tick(treg[9]+3);
	for (int i=0;i<1;i++)
		bSys->spwn_blt(treg[9]+3,ePos,ePos);
	treg[10] -= bSys->get_pHit(0,ePos,50,50);

	/*bSys->delta_fDir(3);
	for (int i=0;i<2048;i++) {
		glm::vec2 toDir = bSys->get_bltDir(2,i);float mult = glm::length(toDir);
		glm::vec2 oDir = glm::normalize(toDir);
		float angle = glm::orientedAngle(oDir,glm::vec2(1*treg[2],0));
		glm::vec4 nDir = glm::vec4(oDir.x,oDir.y,0,0)*glm::rotate(glm::mat4(1.0f),
			-angle/60,glm::vec3(0,0,1));
		bSys->set_bltDir(2,i,glm::vec2(nDir.x,nDir.y));
	} for (int i=0+6*!treg[3];i<6;i++) {
		glm::vec2 bPos = glm::vec2(ePos.x-12*treg[2],ePos.y)+glm::vec2(i,5*i);
		bSys->spwn_blt(2,bPos,bPos-ePos);
	}*/

	// VISUALS
	r2d->prepare();
	r2d->sl.at(rnd_index).model = glm::translate(glm::mat4(1.0f),glm::vec3(ePos.x,ePos.y,0));
	r2d->render_sprite(rnd_index,rnd_index+1);
	bSys->render();
}
