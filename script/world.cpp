#include "world.h"

/*
	constructor(EngineReference)
	eref: references to all relevant engine objects
	purpose: create a world handling entity for a better loop structure
*/
World::World(EngineReference eref)
	: m_eref(eref)
{
	player = Player(eref.frame,eref.r2d,eref.r3d,eref.rI,eref.bSys,eref.iMap);
	game_fb = FrameBuffer(m_eref.frame->w_res,m_eref.frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);
}

/*
	add_<element>(<element>*) -> void
	argument[0]: element to add to element list it belongs to
	purpose: create an element in the world that will be handled according to the world's logic
*/
void World::add_ui(UI* ui)
{ ui_master.push_back(ui); }
void World::add_boss(Boss* boss)
{ boss_master.push_back(boss); }

/*
	remove_<element>(uint8_t) -> void
	argument[0]: list index of element to remove from world
	purpose: remove an element from the world by it's list id
*/
void World::remove_ui(uint8_t ui_id)
{ ui_master.erase(ui_master.begin()+ui_id); }
void World::remove_boss(uint8_t boss_id)
{ boss_master.erase(boss_master.begin()+boss_id); }

/*
	render(uint32_t&,bool&) -> void
	running: runstate of the program, used to store progression & if program is still running
	reboot: holds if program should reboot after it has been closed
	purpose: render & handle world entites
*/
void World::render(uint32_t &running,bool &reboot)
{
	// bind scene framebuffer
	game_fb.bind();
	m_eref.frame->clear(.1f,.1f,.1f);

	// handle bosses
	for (auto boss : boss_master) boss->update(glm::vec2(100,100));

	// player handling
	player.update(running,0);

	// bullets
	m_eref.bSys->render();

	// render ui
	game_fb.close();
	ui_master[active_menu]->render(&game_fb,running,reboot);
}