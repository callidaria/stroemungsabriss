#include "game.h"

void Game::run(uint32_t &rstate,CCBManager* ccbm)
{
	m_bgenv.load(rstate,ccbm);
	m_r2d->load_wcam(m_cam2d);
	uint32_t running=rstate+1;while (running) { // ??maybe kill check if flush with static func ref
		m_frame->print_fps();m_frame->input(running,false);m_frame->clear(.1f,.1f,.1f);
		if (m_frame->kb.ka[SDL_SCANCODE_ESCAPE]) break; // !!kill this when light menu exists

		m_bgenv.update(rstate);
		m_player.update(rstate);

		m_frame->update();
	} rstate=0; // TODO: choose between menu &|| desktop skip
}
