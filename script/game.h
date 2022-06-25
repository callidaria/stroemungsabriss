#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"

#include "../ccb/fcn/ccb_manager.h"

#include "../ccb/mat/camera2d.h"

#include "bgenv.h"
#include "player.h"
#include "bullet_system.h"

#include "boss_lchild.h"
#include "boss_dpilot.h"

class Game
{
public:
	Game(Frame* f,Renderer2D* r2d,RendererI* rI,Camera2D* cam2d)
		: m_frame(f),m_r2d(r2d),m_rI(rI),m_cam2d(cam2d) {  }
	~Game() {  }
	void run(uint32_t &rstate,CCBManager* ccbm);
private:
	Frame* m_frame;
	Renderer2D* m_r2d;
	RendererI* m_rI;
	Camera2D* m_cam2d;

	BGEnv m_bgenv = BGEnv(m_r2d);
	BulletSystem m_bSys = BulletSystem(m_rI);

	Player m_player = Player(m_frame,m_r2d,m_rI,&m_bSys);

	/*
		func(Renderer2D*,uint32_t&,BulletSystem*,int32_t*) -> void
			=> func(r2d,rnd_index,bSys,treg) -> void
		r2d: pointer to renderer, used to render 2D elements
		rnd_index: starting memory index of renderer, to be modified by first write
		bSys: bullet system, the enemies use to spawn their bullet patterns
		treg: variable register for externally saved and modified values
	*/
	std::vector<void(*)(Renderer2D*,uint32_t&,BulletSystem*,int32_t*)> stg_ld = {
		BossLChild::load,BossDPilot::load
	};

	/*
		func(Renderer2D*,uint32_t&,BulletSystem*,glm::vec2,glm::vec2,int32_t*) -> void
			=> func(r2d,rnd_index,bSys,pPos,ePos,treg)
		r2d,rnd_index,bSys,treg: see definitions for stg_ld
		pPos: updated pc position
		ePos: updated enemy position
	*/
	std::vector<void(*)(Renderer2D*,uint32_t&,BulletSystem*,glm::vec2,glm::vec2,int32_t*)> stg_upd = {
		BossLChild::update,BossDPilot::update
	};

	// FIXME: reduce all possible register values into register to keep parameters clean
};
