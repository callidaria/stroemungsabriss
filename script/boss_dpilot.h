#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"

class BossDPilot
{
public:
	BossDPilot() {  }
	~BossDPilot() {  }
	void run(Frame* f,Renderer2D* r2d);
};
