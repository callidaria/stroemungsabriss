#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "player.h"

class BossDPilot
{
public:
	BossDPilot() {  }
	~BossDPilot() {  }
	void run(Player* pchar,Frame* f,Renderer2D* r2d);
};
