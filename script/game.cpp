#include "game.h"

void Game::run(uint32_t &rstate,CCBManager* ccbm)
{
	m_bgenv.load(rstate,ccbm);

	// TODO: reduce
	uint32_t stg_idx2d;
	int32_t fwd_treg[16] = { 0 };
	glm::vec2 ePos = glm::vec2(615,600);
	stg_ld.at(rstate)(m_r3d,stg_idx2d,&m_bSys,fwd_treg);

	// vertex & texture load
	Camera3D cam3d = Camera3D(1280.0f,720.0f);
	m_r2d->load(m_cam2d);
	m_rI->load(m_cam2d);
	m_r3d->load(&cam3d);

	// ortho 3D element lighting
	Light3D l3d_ortho = Light3D(m_r3d,0,glm::vec3(100,0,0),glm::vec3(1,1,1),1);
	l3d_ortho.set_ambient(1.0f);

	// ui
	Healthbar hbar = Healthbar(glm::vec2(140,670),1000,30,{ 3,4 },
			{ 10000,5000,10000,10000,5000,5000,10000 },"The Dancing Pilot");

	// post processing
	fb = FrameBuffer(m_frame->w_res,m_frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);

	bool menu_trg = false,menu_sys = false,menu_inf = false;
	float ptrans = 1;
	uint32_t running = rstate+1;
	while (running) {  // ??maybe kill check if flush with static func ref
		m_frame->print_fps();
		m_frame->input(running,false);
		m_frame->clear(.1f,.1f,.1f);
		if (m_frame->kb.ka[SDL_SCANCODE_ESCAPE]) break;  // FIXME: kill this when light menu exists

		// action menu open requests
		bool req_sysmenu = m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!menu_trg&&!menu_inf;
		menu_sys = menu_sys*!req_sysmenu+!menu_sys*req_sysmenu;
		bool req_infomenu = m_frame->kb.ka[SDL_SCANCODE_TAB]&&!menu_trg&&!menu_sys;
		menu_inf = menu_inf*!req_infomenu+!menu_inf*req_infomenu;
		menu_trg = m_frame->kb.ka[SDL_SCANCODE_RETURN]||m_frame->kb.ka[SDL_SCANCODE_TAB];

		// dynamic animation variable change
		ptrans += -.1f*((menu_sys||menu_inf)&&ptrans>0)+.1f*((!menu_sys&&!menu_inf)&&ptrans<1);

		// menu background framebuffer
		fb.bind();
		m_frame->clear(0,0,0);

		m_bgenv.update(rstate);
		stg_upd.at(rstate)(m_r3d,stg_idx2d,&m_bSys,m_player.get_pPos()+glm::vec2(25),ePos,fwd_treg);
		m_player.update(rstate,fwd_treg[11]);
		m_bSys.render();

		// healthbar
		hbar.register_damage(fwd_treg[10]);
		hbar.render();

		// action menu
		FrameBuffer::close();
		m_frame->clear(0,0,0);
		fb.render(ptrans);

		m_frame->update();
	} rstate = 0;  // TODO: choose between menu &|| desktop skip
}