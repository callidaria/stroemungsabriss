#include "worldbuilder.h"

/*
	construction(CascabelBaseFeature*,CCBManager*,World*)
	ccbf: all common cascabel tools & features
	set_rigs: stage setup
	ccbm: cascabel manager to load level files with
	world: world to load objects & logic for
	purpose: create a worldbuilder to process world loading logic
*/
Worldbuilder::Worldbuilder(CascabelBaseFeature* ccbf,StageSetup* set_rigs,CCBManager* ccbm,
		World* world)
	: m_ccbf(ccbf),m_setRigs(set_rigs),m_ccbm(ccbm),m_world(world) {  }

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

		// casino
		case LOAD_CASINO: load_casino();
			break;

		// cardtable testing
		case LOAD_CARDTABLE: load_cards();
			break;

		// nepal
		case LOAD_AIRFIELD: load_airfield();
		case LOAD_DPILOT: load_dpilot();
			break;

		default: printf("load instruction has no logic\n");
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
}
void Worldbuilder::load_casino()
{
	bool loading = true;
	std::thread loading_screen(show_load_progression,&loading,m_ccbf);
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap);
	CasinoSpike* cspike = new CasinoSpike(m_ccbf,m_setRigs);
	m_world->add_ui(action_menu);
	m_world->add_scene(cspike);
	m_world->active_daui = 1;
	m_world->active_cam3D = 0;
	m_world->load_geometry();
	m_world->upload_lighting();
	m_world->upload_lightmap();
	loading = false;
	loading_screen.join();
}
void Worldbuilder::load_cards()
{
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap);
	CasinoTable* ctable = new CasinoTable(m_ccbf,m_setRigs);
	m_world->add_ui(action_menu);
	m_world->add_scene(ctable);
	m_world->active_daui = 1;
	m_world->active_cam3D = 3;
	m_world->load_geometry();
	m_world->upload_lighting();
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
}

// test
void Worldbuilder::background_ticks(bool* loading,uint32_t* tick)
{
	while (*loading) {
		printf("tick progression: %i\n",*tick);
		(*tick)++;
	}
}

/*
	TODO
*/
void Worldbuilder::show_load_progression(bool* loading,CascabelBaseFeature* ccbf)
{
	// setup loading visualization
	SDL_GLContext context = ccbf->frame->get_new_context();
	std::vector<float> ld_canvas = Toolbox::create_sprite_canvas_triangled(glm::vec2(100,100),200,200);
	Buffer ld_buffer = Buffer();
	ld_buffer.bind();
	ld_buffer.upload_vertices(ld_canvas);
	Shader ld_shader = Shader();
	ld_shader.compile2d("shader/vloadfdb.shader","shader/floadfdb.shader");
	while (*loading) {

		// clear loading screen
		ccbf->frame->clear(0,.4f,0);
		ccbf->frame->vsync(60);

		// canvas render
		glDisable(GL_DEPTH_TEST);
		ld_shader.enable();
		ld_buffer.bind();
		glDrawArrays(GL_TRIANGLES,0,6);

		// update loading screen
		ccbf->frame->update();
	} SDL_GL_DeleteContext(context);
}