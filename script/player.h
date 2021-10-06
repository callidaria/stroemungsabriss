#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"

struct PlayerControls
{
	int* flt_ud,*flt_lr;					// floating directionals
	int* prc_ud,*prc_lr;					// precision shot
	bool* abs_up,*abs_down,*abs_left,*abs_right;		// absolute directionals
	bool* cq_def,*cq_atk,*rng_wide,*rng_focus;	// attack and defense
	bool* bomb,*change,*dash,*target;			// additional tools
	bool* pause,*rdetails,*qrestart;			// non-diegesis related
};

class Player
{
public:
	Player() { sal[0]=0;sal[1]=1; }
	Player(Frame* f,Renderer2D* r2d,RendererI* rI);
	~Player();
	void update(uint32_t &rstate);
private:
	// additional
	void emulate_vectorized();

	// ranged functions
	static void jet_wait(RendererI* rI);
	static void jet_wide(RendererI* rI);
	static void jet_focus(RendererI* rI);
	static void jet_scientific(RendererI* rI);
private:
	// rendering
	Frame* m_frame;
	Renderer2D* m_r2d;
	RendererI* m_rI;
	int ri;
	Shader shp;				// health bar shader
	uint32_t hpvao,hpvbo;			// health bar buffers
	uint8_t sal[2];				// sprite adress library

	// controlling
	glm::vec3 pos = glm::vec3(200,200,0);
	glm::vec2 dhold;
	int ddur=0,drec=0;
	struct PlayerControls cnt;
	int emuflt_ud,emuflt_lr;
	uint32_t dz_epsilon = 0;

	// ranged
	std::vector<void(*)(RendererI*)> rng_flib;	// indexed ranged functions
};
