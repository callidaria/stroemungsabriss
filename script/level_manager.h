#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "boss_dpilot.h"

class LevelManager
{
public:
	LevelManager() {  }
	LevelManager(Frame* frame,Renderer2D* r2d);
	~LevelManager();
	void run(uint8_t &mm);
private:
	Frame* m_frame;
	Renderer2D* m_r2d;
	//BossDPilot dpilot;
	BossDPilot lmains[1];
	Player player;
	int ri;
};
