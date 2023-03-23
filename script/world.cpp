#include "world.h"

/*
	TODO
*/
World::World(EngineReference eref)
	: m_eref(eref)
{
	game_fb = FrameBuffer(m_eref.frame->w_res,m_eref.frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);
}

/*
	TODO
*/
void World::add_ui(UI* ui)
{ ui_master.push_back(ui); }

/*
	TODO
*/
void World::remove_ui(uint8_t ui_id)
{ ui_master.erase(ui_master.begin()+ui_id); }

/*
	TODO
*/
void World::render(uint32_t &running,bool &reboot)
{
	// bind scene framebuffer
	game_fb.bind();

	// render ui
	game_fb.close();
	ui_master[active_menu]->render(&game_fb,running,reboot);
}