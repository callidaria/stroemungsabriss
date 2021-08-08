#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "boss_lchild.h"
#include "boss_dpilot.h"

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

	BossLChild lchild = BossLChild();
	BossDPilot dpilot = BossDPilot();

	const uint32_t LID_LCHILD = 0;
	const uint32_t LID_DPILOT = 1;
};
