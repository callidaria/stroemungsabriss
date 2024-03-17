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

		// loading feedback creation
		if (!ldfb_showing) {
			progress = .0f;
			ldfb_showing = true;
			std::thread load_fdb(show_load_progression,&ldfb_showing,m_ccbf,&progress);
			load_fdb.detach();
		} // FIXME: yes, this causes a memory leak ...too bad. called by member?

		// switch load instructions
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

		// check loading feedback conclusion
		ldfb_showing = !m_ccbf->ld.empty();
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
	//Menu* main_menu = new Menu(m_world,m_ccbm,m_ccbf,progress,.75f);
	MainMenu* main_menu = new MainMenu(m_ccbm,m_ccbf,m_world,progress,.75f);
	m_world->add_ui(main_menu);
	m_world->load(progress,.25f);
}

void Worldbuilder::load_casino()
{
	std::cout << "loading: spike's casino\n";
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap,progress,.25f);
	CasinoSpike* cspike = new CasinoSpike(m_ccbf,m_setRigs,progress,.5f);
	m_world->add_ui(action_menu);
	m_world->add_scene(cspike);
	m_world->active_daui = 1;
	m_world->active_cam3D = 0;
	m_world->load(progress,.21f);
	progress = 1.0f;
	// TODO: dynamification of camera and ui (etc...) picker
}

void Worldbuilder::load_cards()
{
	std::cout << "loading: card games\n";
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap,progress,.25f);
	CasinoTable* ctable = new CasinoTable(m_ccbf,m_setRigs,progress,.5f);
	m_world->add_ui(action_menu);
	m_world->add_scene(ctable);
	m_world->active_daui = 1;
	m_world->active_cam3D = 3;
	m_world->load(progress,.21f);
	progress = 1.0f;
}

void Worldbuilder::load_airfield()
{ std::cout << "loading: airfield scene\n"; }

void Worldbuilder::load_dpilot()
{
	std::cout << "loading: dancing pilot duel\n";
	ActionMenu* action_menu = new ActionMenu(m_ccbf->frame,m_ccbf->iMap,progress,.25f);
	NepalMountainWoods* nmw = new NepalMountainWoods(m_ccbm,m_ccbf);
	progress += .2f;
	JaegerJet* jj = new JaegerJet(m_ccbf);
	progress += .2f;
	DPilot* dpilot = new DPilot(m_ccbf);
	progress += .2f;
	m_world->add_ui(action_menu);
	m_world->add_scene(nmw);
	m_world->add_playable(jj);
	m_world->add_boss(dpilot);
	m_world->active_daui = 1;
	m_world->active_cam3D = 1;
	m_world->load(progress,.2f);
}

/*
	show_load_progression(bool*,CascabelBaseFeature*,float*) -> void
	purpose: display loading screen by a different thread, to visualize load progression
	\param loading: memory address of boolean holding info, if load instructions are being processed
	\param ccbf: addresses to main feature objects of cascabel
	\param progress: address of loading progress variable
*/
void Worldbuilder::show_load_progression(bool* loading,CascabelBaseFeature* ccbf,float* progress)
{
	// visualization setup
	SDL_GLContext context = ccbf->frame->create_new_context();
	Camera2D cam2D = Camera2D(1280.0f,720.0f);

	// loading bar setup
	std::vector<float> ld_canvas
			= Toolbox::create_sprite_canvas_triangled(glm::vec2(0),700,10);
	Buffer ld_buffer = Buffer();
	ld_buffer.bind();
	ld_buffer.upload_vertices(ld_canvas);
	Shader ld_shader = Shader();
	ld_shader.compile2d("./shader/vloadfdb.shader","./shader/floadfdb.shader");
	ld_shader.upload_camera(cam2D);

	// bar borders setup
	float x_lft = 528.0f,x_rgt = 1232.0f,y_dwn = 42.0f,y_up = 57.0f,brd_offset = 4;
	std::vector<float> ld_bar = {
		x_lft,y_up, x_lft+brd_offset,y_up, x_lft,y_up, x_lft,y_up-brd_offset,
		x_rgt,y_up, x_rgt-brd_offset,y_up, x_rgt,y_up, x_rgt,y_up-brd_offset,
		x_lft,y_dwn, x_lft+brd_offset,y_dwn, x_lft,y_dwn, x_lft,y_dwn+brd_offset,
		x_rgt,y_dwn, x_rgt-brd_offset,y_dwn, x_rgt,y_dwn, x_rgt,y_dwn+brd_offset,
	}; Buffer brd_buffer = Buffer();
	brd_buffer.bind();
	brd_buffer.upload_vertices(ld_bar);
	Shader brd_shader = Shader();
	brd_shader.compile("./shader/vloadborder.shader","./shader/floadborder.shader");
	brd_shader.def_attributeF("position",2,0,2);
	brd_shader.upload_camera(cam2D);

	// render loop
	while (*loading) {

		// clear loading screen
		ccbf->frame->clear(.1f,.1f,.1f);
		ccbf->frame->vsync(60);
		ccbf->frame->calc_time_delta();
		// create stylized background animation OR timed background action art iterations

		// prepare bar
		ld_shader.enable();
		ld_buffer.bind();

		// translate bar
		glm::mat4 rmodel = glm::translate(glm::mat4(1.0f),glm::vec3(530,45,0));
		rmodel = glm::scale(rmodel,glm::vec3(*progress,1,1));
		ld_shader.upload_matrix("model",rmodel);

		// render bar
		glDrawArrays(GL_TRIANGLES,0,6);

		// border rendering
		brd_shader.enable();
		brd_buffer.bind();
		glDrawArrays(GL_LINES,0,16);

		// update loading screen
		ccbf->frame->update();
	} SDL_GL_DeleteContext(context);
}
// TODO: what if multiple load instructions are in queue? the loading bar will reset! fix this!
