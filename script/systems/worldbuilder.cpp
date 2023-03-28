#include "worldbuilder.h"

/*
	TODO
*/
Worldbuilder::Worldbuilder(CascabelBaseFeature* ccbf,CCBManager* ccbm,World* world)
	: m_ccbf(ccbf),m_ccbm(ccbm),m_world(world)
{  }

/*
	TODO
*/
void Worldbuilder::load()
{
	while (!m_ccbf->ld.empty()) {
		switch (m_ccbf->ld.front()) {

		// start
		case LOAD_START: load_titles();
		case LOAD_MENU: load_menu();
			load_geometry();
			break;

		// nepal
		case LOAD_AIRFIELD: load_airfield();
		case LOAD_DPILOT: load_dpilot();
			load_geometry();
			break;

		default: return;
		} m_ccbf->ld.pop();
	}
}

/*
	TODO
*/
void Worldbuilder::load_titles()
{
	std::cout << "loading: title display\n";
}

/*
	TODO
*/
void Worldbuilder::load_menu()
{
	std::cout << "loading: main menu\n";
	Menu menu = Menu(m_world,m_ccbm,m_ccbf);
	m_world->add_ui(menu);
}

/*
	TODO
*/
void Worldbuilder::load_airfield()
{
	std::cout << "loading: airfield scene\n";
}

/*
	TODO
*/
void Worldbuilder::load_dpilot()
{
	std::cout << "loading: dpilot boss\n";
}

/*
	TODO
*/
void Worldbuilder::load_geometry()
{
	Camera2D cam2D = Camera2D(1280.0f,720.0f);
	Camera3D cam3D = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f);
	m_ccbf->r2d->load(&cam2D);
	m_ccbf->r3d->load(cam3D);
	m_ccbf->rI->load();
}