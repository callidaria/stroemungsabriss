#pragma once

#include <iostream>

#include "../../ccb/frm/framebuffer.h"

class UI
{
public:
	virtual void render(FrameBuffer*,uint32_t&,bool&) {  }
};