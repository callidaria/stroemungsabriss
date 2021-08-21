#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"
#include "player.h"

class Game
{
public:
	Game(Frame* f,Renderer2D* r2d,RendererI* rI) : m_frame(f),m_r2d(r2d),m_rI(rI) {  }
	~Game() {  }
	void run(uint32_t &rstate);
private:
	Frame* m_frame;
	Renderer2D* m_r2d;
	RendererI* m_rI;
	Player m_player = Player(m_frame,m_r2d,m_rI);
};
