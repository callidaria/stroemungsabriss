#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/framebuffer.h"

class UI
{
public:
	virtual void render(FrameBuffer*,uint32_t&,bool&) {  }
};

class PlayerVisuals
{
public:
	virtual void render() {  }
};

class Boss
{
public:
	virtual void update(glm::vec2) {  }
};