#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/gfx/renderer2d.h"
#include "player.h"

class BossLChild
{
public:
	BossLChild() {  }
	~BossLChild() {  }
	void run(Player* pchar,Frame* f,Renderer2D* r2d);
};
