#pragma once

#include <iostream>

#include "../../ccb/frm/frame.h"
#include "../../ccb/frm/framebuffer.h"

class ActionMenu
{
public:

	// construction
	ActionMenu(Frame* frame);
	~ActionMenu() {  }

	// runtime interactions
	void update();
	void bind();
	void render();

private:

	// cascabel
	Frame* m_frame;
	FrameBuffer game_fb;

	// action menu trigger system
	bool menu_trg = false;	// input trigger for last frame menu requests
	bool menu_sys = false;	// identifies if system menu is opened
	bool menu_inf = false;	// identifies if information menu is opened

	// animation
	float ptrans = 1.0f;
};