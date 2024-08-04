#ifndef SCR_INTERFACE_ACTION_MENU
#define SCR_INTERFACE_ACTION_MENU

#include <iostream>

#include "../../ccb/core.h"

#include "../base.h"
#include "../world.h"


// constant
constexpr uint8_t ACT_TEXT_SIZE = 30;
constexpr uint8_t TEXT_DISTANCE = 5;

enum SystemSelection
{
	AMSEL_SYS_CONTINUE,
	AMSEL_SYS_SETTINGS,
	AMSEL_SYS_RESET,
	AMSEL_SYS_RESET_OS,
	AMSEL_SYS_COUNT,
	AMSEL_SYS_MAXCHOICE = AMSEL_SYS_RESET_OS
};

enum InfomationSelection
{
	AMSEL_INFO_TEST0,
	AMSEL_INFO_TEST1,
	AMSEL_INFO_TEST2,
	AMSEL_INFO_TEST3,
	AMSEL_INFO_COUNT,
	AMSEL_INFO_MAXCHOICE = AMSEL_INFO_TEST3
};

// menu position
constexpr uint16_t TEXT_YPOSITION_SYS =((uint16_t)MATH_CARTESIAN_YRANGE>>1)-(ACT_TEXT_SIZE>>1)
		+ (AMSEL_SYS_COUNT>>1)*ACT_TEXT_SIZE+(AMSEL_SYS_COUNT&1)*(ACT_TEXT_SIZE>>1);
constexpr uint16_t TEXT_YPOSITION_INFO = ((uint16_t)MATH_CARTESIAN_YRANGE>>1)-(ACT_TEXT_SIZE>>1)
		+ (AMSEL_INFO_COUNT>>1)*ACT_TEXT_SIZE+(AMSEL_INFO_COUNT&1)*(ACT_TEXT_SIZE>>1);
constexpr uint8_t TEXT_DRAW_SPACE = ACT_TEXT_SIZE-TEXT_DISTANCE;


class ActionMenu : public UI
{
public:

	// construction
	ActionMenu(float& progress,float pseq);
	~ActionMenu() {  }

	// runtime interactions
	void render(FrameBuffer* game_fb,bool& running,bool& reboot);

private:

	// cascabel
	Buffer splash_buffer = Buffer();
	Shader splash_shader = Shader();
	Font tfont = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",
			TEXT_DRAW_SPACE,TEXT_DRAW_SPACE);
	Text mtext = Text(tfont),stext = Text(tfont);
	InputMap* imap;

	// action menu trigger system
	bool menu_trg = false;	// input trigger for last frame menu requests
	bool menu_sys = false;	// identifies if system menu is opened
	bool menu_inf = false;	// identifies if information menu is opened

	// animation
	float ptrans = 1.0f;
	int32_t sEdges[4] = { rand()%30-15,rand()%30-15,rand()%30-15,rand()%30-15 };

	// selection
	uint8_t msel = 0,isel = 0;
	bool trg_smod = false;
};

#endif