#pragma once

#include <iostream>
#include "../ccb/gfx/renderer2d.h"
#include "player.h"

class Level
{
public:
	Level() {  }
	virtual void load(Renderer2D* r2d) {  }
	virtual void update() {  }
protected:
	Player m_player;
};
