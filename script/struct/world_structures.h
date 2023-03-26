#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/framebuffer.h"

// describes any form of large scale user interface the player interacts with mainly, when enabled
class UI
{
public:
	virtual void render(FrameBuffer*,uint32_t&,bool&) {  }
};

// describes any form of enemy, that can be considered significant and/or deserves a health bar
class Boss
{
public:
	virtual void update(glm::vec2) {  }
};