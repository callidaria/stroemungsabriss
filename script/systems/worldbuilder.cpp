#include "worldbuilder.h"

/*
	construction(CascabelBaseFeature*,CCBManager*,World*)
	ccbf: all common cascabel tools & features
	ccbm: cascabel manager to load level files with
	world: world to load objects & logic for
	purpose: create a worldbuilder to process world loading logic
*/
Worldbuilder::Worldbuilder(CascabelBaseFeature* ccbf,CCBManager* ccbm,World* world)
	: m_ccbf(ccbf),m_ccbm(ccbm),m_world(world) {  }

/*
	load() -> void
	purpose: process load instruction queue & load world based on said instructions
*/
void Worldbuilder::load()
{
	while (!m_ccbf->ld.empty()) {
		switch (m_ccbf->ld.front()) {

		// start
		case LOAD_START: load_titles();
		case LOAD_MENU: load_menu();
			break;

		// nepal
		case LOAD_AIRFIELD: load_airfield();
		case LOAD_DPILOT: load_dpilot();
			break;

		default: return;
		} m_ccbf->ld.pop();
	}
}

/*
	load_<scene>() -> void (private)
	purpose: load logic of different scenes throughout the game
*/
void Worldbuilder::load_titles()
{ std::cout << "loading: title display\n"; }
void Worldbuilder::load_menu()
{
	Menu* menu = new Menu(m_world,m_ccbm,m_ccbf);
	m_world->add_ui(menu);
	m_world->load_geometry();
	Light3D l3d = Light3D(m_ccbf->r3d,0,glm::vec3(-500,750,100),glm::vec3(1,1,1),1);
	l3d.upload();
	l3d.set_amnt(1);
}
void Worldbuilder::load_airfield()
{ std::cout << "loading: airfield scene\n"; }
void Worldbuilder::load_dpilot()
{
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap);
	NepalMountainWoods* nmw = new NepalMountainWoods(m_ccbm,m_ccbf);
	JaegerJet* jj = new JaegerJet(m_ccbf);
	DPilot* dpilot = new DPilot(m_ccbf);
	m_world->add_ui(action_menu);
	m_world->add_scene(nmw);
	m_world->add_playable(jj);
	m_world->add_boss(dpilot);
	m_world->active_daui = 1;
	m_world->active_cam3D = 1;
	m_world->load_geometry();
	Light3D l3d_ortho = Light3D(m_ccbf->r3d,0,glm::vec3(640,360,10000),glm::vec3(1,1,1),1);
	l3d_ortho.set_amnt(1);
	l3d_ortho.upload();
}