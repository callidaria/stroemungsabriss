#include "boss_dpilot.h"

void BossDPilot::load(Renderer2D* r2d)
{
	m_player = Player(r2d);
}
void BossDPilot::update()
{
	m_player.update();
}
