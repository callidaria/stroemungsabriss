#include "game.h"

void Game::run(uint32_t &rstate,CCBManager* ccbm)
{
	m_bgenv.load(rstate,ccbm);

	// TODO: reduce
	uint32_t stg_idx2d;
	int32_t fwd_treg[16] = { 0 };
	glm::vec2 ePos = glm::vec2(615,600);
	stg_ld.at(rstate)(m_r2d,stg_idx2d,&m_bSys,fwd_treg);

	m_r2d->load_wcam(m_cam2d);
	m_rI->load_wcam(m_cam2d);

	// ui
	Healthbar hbar = Healthbar(glm::vec2(140,670),1000,30,{ 3,4 },
			{ 10000,5000,10000,10000,5000,5000,10000 },"The Dancing Pilot");

	uint32_t running=rstate+1;
	while (running) { // ??maybe kill check if flush with static func ref
		m_frame->print_fps();
		m_frame->input(running,false);
		m_frame->clear(.1f,.1f,.1f);
		if (m_frame->kb.ka[SDL_SCANCODE_ESCAPE]) break; // FIXME: kill this when light menu exists

		m_bgenv.update(rstate);	
		stg_upd.at(rstate)(m_r2d,stg_idx2d,&m_bSys,m_player.get_pPos()+glm::vec2(25),ePos,fwd_treg);
		m_player.update(rstate,fwd_treg[11]);
		m_bSys.render();

		// healthbar
		hbar.register_damage(fwd_treg[10]);
		hbar.render();

		m_frame->update();
	} rstate=0; // TODO: choose between menu &|| desktop skip
}
