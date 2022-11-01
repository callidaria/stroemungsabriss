#include "action_menu.h"

/*
	construction(Frame*)
	frame: pointer to frame the game is drawn onto
	purpose: creates the lightweight in-action menu to pause and use while in-game
*/
ActionMenu::ActionMenu(Frame* frame)
	: m_frame(frame)
{
	game_fb = FrameBuffer(m_frame->w_res,m_frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);
}

/*
	update() -> void
	purpose: animation updates and user request interpretation regarding action menu
*/
void ActionMenu::update()
{
	// action menu open requests
	bool req_sysmenu = m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!menu_trg&&!menu_inf;
	menu_sys = menu_sys*!req_sysmenu+!menu_sys*req_sysmenu;
	bool req_infomenu = m_frame->kb.ka[SDL_SCANCODE_TAB]&&!menu_trg&&!menu_sys;
	menu_inf = menu_inf*!req_infomenu+!menu_inf*req_infomenu;
	menu_trg = m_frame->kb.ka[SDL_SCANCODE_RETURN]||m_frame->kb.ka[SDL_SCANCODE_TAB];

	// dynamic animation variable change
	ptrans += -.1f*((menu_sys||menu_inf)&&ptrans>0)+.1f*((!menu_sys&&!menu_inf)&&ptrans<1);

	// game time modification
	m_frame->set_tmod(ptrans);
}

/*
	bind() -> void
	purpose: bind and clear game framebuffer so that it can serve as menu background
	!! DISCLAIMER: has to be used right before all the game board components !!
*/
void ActionMenu::bind()
{
	game_fb.bind();
	m_frame->clear();
}

/*
	render() -> void
	purpose: render current menu state
		ptrans ~= 1.0f: renders game normally as the game has not been stopped at this moment
		ptrans ~= 0.0f: renders game sepia filtered and the menu is placed on top
	!! DISCLAIMER: has to be run after game board components, as well as all other ui !!
*/
void ActionMenu::render()
{
	FrameBuffer::close();
	m_frame->clear();
	game_fb.render(ptrans);
}