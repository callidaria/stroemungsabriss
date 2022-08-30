#pragma once

#include <iostream>
#include <glm/glm.hpp>

#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"
#include "../ccb/fcn/buffer.h"

#include "bullet_system.h"
#include "ui/healthbar.h"

struct PlayerControls
{
	int* flt_ud,*flt_lr;							// floating directionals
	int* prc_ud,*prc_lr;							// precision shot
	bool* abs_up,*abs_down,*abs_left,*abs_right;	// absolute directionals
	bool* cq_def,*cq_atk,*rng_wide,*rng_focus;		// attack and defense
	bool* bomb,*change,*dash,*target;				// additional tools
	bool* pause,*rdetails,*qrestart;				// non-diegesis related
};

class Player
{
public:
	Player() { sal[0]=0;sal[1]=1; }
	Player(Frame* f,Renderer2D* r2d,RendererI* rI,BulletSystem* bsys);
	~Player();

	void update(uint32_t &rstate,int32_t pDmg);
	glm::vec2 get_pPos();
private:
	// additional
	void emulate_vectorized();

	// ranged functions
	static void jet_wait(BulletSystem* bsys,int32_t* treg);
	static void jet_wide(BulletSystem* bsys,int32_t* treg);
	static void jet_focus(BulletSystem* bsys,int32_t* treg);
	static void jet_scientific(BulletSystem* bsys,int32_t* treg);
private:
	// rendering
	Frame* m_frame;
	Renderer2D* m_r2d;
	RendererI* m_rI;
	BulletSystem* m_bsys;
	//Healthbar hbar = Healthbar(glm::vec2(10,10),400,25,{ 1 },{ 1 });
	int ri;
	uint8_t sal[2];			// sprite adress library
	int32_t treg[16] = { 0 };

	// controlling
	glm::vec3 pos = glm::vec3(200,200,0);
	glm::vec2 dhold;
	int ddur=0,drec=0;
	struct PlayerControls cnt;
	int emuflt_ud,emuflt_lr;
	uint32_t dz_epsilon = 0;

	// ranged method list
	/*
		func(BulletSystem*,int32_t*) -> void
			=> func(bsys,treg) -> void
		bsys: bullet system used to spawn player projectiles
		treg: register holding information to save beyond static method
	*/
	std::vector<void(*)(BulletSystem*,int32_t*)> rng_flib;	// indexed ranged functions
};
