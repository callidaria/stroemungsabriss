#pragma once

#include "level.h"

class BossDPilot : public Level
{
public:
	BossDPilot() {  }
	void load(Renderer2D* r2d);
	void update();
};
