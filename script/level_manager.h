#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "player.h"
#include "boss_lchild.h"
#include "boss_dpilot.h"

enum LNR_ID
{
	LCHILD,
	DPILOT
};

class LevelManager
{
public:
	LevelManager(Frame* frame,Renderer2D* r2d)
		: m_frame(frame),m_r2d(r2d) {  }
	~LevelManager() {  }
	void run(uint32_t lnr);
private:
	Frame* m_frame;
	Renderer2D* m_r2d;
	Player m_pchar = Player(m_frame,m_r2d);

	BossLChild lchild = BossLChild();
	BossDPilot dpilot = BossDPilot();
};
