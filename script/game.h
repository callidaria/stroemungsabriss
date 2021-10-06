#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"
#include "../ccb/fcn/ccb_manager.h"
#include "../ccb/mat/camera2d.h"
#include "bgenv.h"
#include "player.h"

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
	Player m_player = Player(m_frame,m_r2d,m_rI);
};
