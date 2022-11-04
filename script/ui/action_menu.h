#pragma once

#include <iostream>

#include "../../ccb/frm/frame.h"
#include "../../ccb/frm/framebuffer.h"

#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "../player.h"

// constant
constexpr uint8_t SYS_OPTION_COUNT = 3;
constexpr uint8_t INFO_OPTION_COUNT = 5;
constexpr uint8_t TEXT_SIZE = 30;
constexpr uint8_t TEXT_DISTANCE = 5;

// menu position
constexpr uint16_t TEXT_YPOSITION_SYS = 720/2+(SYS_OPTION_COUNT-SYS_OPTION_COUNT%2)*TEXT_SIZE
		+SYS_OPTION_COUNT%2*(TEXT_SIZE/2);
constexpr uint16_t TEXT_YPOSITION_INFO = 720/2+(INFO_OPTION_COUNT-INFO_OPTION_COUNT%2)*TEXT_SIZE
		+INFO_OPTION_COUNT%2*(TEXT_SIZE/2);
constexpr uint8_t TEXT_DRAW_SPACE = TEXT_SIZE-TEXT_DISTANCE;

class ActionMenu
{
public:

	// construction
	ActionMenu(Frame* frame);
	~ActionMenu() {  }

	// runtime interactions
	void update(Player* player);
	void bind();
	void render();

private:

	// cascabel
	Frame* m_frame;
	FrameBuffer game_fb;
	Text mtext,stext;

	// action menu trigger system
	bool menu_trg = false;	// input trigger for last frame menu requests
	bool menu_sys = false;	// identifies if system menu is opened
	bool menu_inf = false;	// identifies if information menu is opened

	// animation
	float ptrans = 1.0f;
};