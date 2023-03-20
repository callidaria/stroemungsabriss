#pragma once

#include <iostream>

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/renderer3d.h"
#include "../ccb/gfx/rendereri.h"
#include "../ccb/gfx/light3d.h"

#include "../ccb/frm/frame.h"

#include "../ccb/fcn/ccb_manager.h"
#include "../ccb/mat/camera2d.h"

#include "systems/bgenv.h"
#include "systems/player.h"
#include "systems/bullet_system.h"

#include "ui/healthbar.h"
#include "ui/action_menu.h"

#include "struct/feature_base.h"

#include "boss/boss_lchild.h"
#include "boss/boss_dpilot.h"

class Game
{
public:

	// construction
	Game(Frame* f,Renderer2D* r2d,Renderer3D* r3d,RendererI* rI,Camera2D* cam2d,InputMap* input_map);
	~Game() {  }

	// loop
	void run(uint32_t &rstate,CCBManager* ccbm);

private:

	// cascabel
	Frame* m_frame;
	Renderer2D* m_r2d;
	Renderer3D* m_r3d;
	RendererI* m_rI;
	Camera2D* m_cam2d;
	InputMap* imap;

	// script
	BGEnv m_bgenv = BGEnv(m_r2d);
	BulletSystem m_bSys = BulletSystem(m_frame,m_rI);
	Player m_player = Player(m_frame,m_r2d,m_r3d,m_rI,&m_bSys,imap);
	CascabelBaseFeature ccbf;

	/*
		func(CascabelBaseFeature*,uint32_t&,int32_t*) -> void
			=> func(r3d,rnd_index,bSys,treg) -> void
		ccbf: struct holding pointers to all cascabel and common script main features
		rnd_index: starting memory index of renderer, to be modified by first write
		treg: variable register for externally saved and modified values
	*/
	std::vector<void(*)(CascabelBaseFeature*,uint32_t&,int32_t*)> stg_ld
			= { BossLChild::load,BossDPilot::load };

	/*
		func(CascabelBaseFeature*,uint32_t&,glm::vec2&,int32_t*) -> void
			=> func(r3d,rnd_index,bSys,pPos,ePos,treg)
		ccbf,rnd_index,treg: see definitions for stg_ld
		ePos: updated enemy position
	*/
	std::vector<void(*)(CascabelBaseFeature*,uint32_t&,glm::vec2&,int32_t*)> stg_upd
			= { BossLChild::update,BossDPilot::update };

	// FIXME: reduce all possible register values into register to keep parameters clean
};