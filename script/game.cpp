#include "game.h"

void Game::run(uint32_t &rstate)
{
	while (true) { // ??maybe kill check if flush with static func ref
		m_frame->print_fps();m_frame->input(rstate,false);m_frame->clear(.1f,.1f,.1f);
		if (m_frame->kb.ka[SDL_SCANCODE_ESCAPE]) break; // !!kill this when light menu exists

		m_player.update();

		m_frame->update();
	}
}
