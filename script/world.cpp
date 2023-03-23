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
	// render ui
	for (uint8_t i=0;i<ui_master.size();i++) ui_master[i]->render(&game_fb,running,reboot);
}