#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"

struct PlayerControls
{
	int* flt_ud,*flt_lr;					// floating directionals
	int* prc_ud,*prc_lr;					// precision shot
	bool* abs_up,*abs_down,*abs_left,*abs_right;		// absolute directionals
	bool* cq_def,*cq_atk;int* rng_wide;bool* rng_focus;	// attack and defense
	bool* bomb,*change,*dash,*target;			// additional tools
	bool* pause,*rdetails;int* qrestart;			// non-diegesis related
};

typedef void (*fnc_vpoint)();

class Player
{
public:
	Player() {  }
	Player(Frame* f,Renderer2D* r2d);
	~Player();
	void update();
private:
	// ranged functions
	static void jet_wait();
	static void jet_projectile();
	static void jet_scientific();
private:
	// rendering
	Renderer2D* m_r2d;
	int ri;
	Shader shp;				// health bar shader
	uint32_t hpvao,hpvbo;			// health bar buffers

	// controlling
	glm::vec3 pos = glm::vec3(200,200,0);
	glm::vec2 dhold;
	int ddur=0,drec=0;
	struct PlayerControls cnt;

	// ranged
	std::vector<fnc_vpoint> rng_flib;	// indexed ranged functions
};
