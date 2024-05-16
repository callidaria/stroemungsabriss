#include "worldbuilder.h"


/*
	TODO
*/
void load_casino(LoadStorage& lds)
{
	std::cout << "loading: spike's casino\n";
	CasinoSpike* cspike = new CasinoSpike(lds.m_ccbf,lds.progress,.5f);
	lds.world->add_scene(cspike);
}

/*
	TODO
*/
void load_cards(LoadStorage& lds)
{
	std::cout << "loading: card games\n";
	CasinoTable* ctable = new CasinoTable(lds.m_ccbf,lds.progress,.5f);
	lds.world->add_scene(ctable);
}

/*
	TODO
*/
void load_airfield(LoadStorage& lds)
{
	std::cout << "loading: airfield scene\n";
	// TODO
}

/*
	TODO
*/
void load_dpilot(LoadStorage& lds)
{
	std::cout << "loading: dancing pilot duel\n";
	Core::gCamera3D = Camera3D(1280.0f,720.0f);  // TODO: this belongs inside the scene loader
	NepalMountainWoods* nmw = new NepalMountainWoods(lds.m_ccbm,lds.m_ccbf);
	lds.progress += .2f;
	/*JaegerJet* jj = new JaegerJet(m_ccbf);
	progress += .2f;*/
	DPilot* dpilot = new DPilot(lds.m_ccbf);
	lds.progress += .2f;
	lds.world->add_scene(nmw);
	//m_world->add_playable(jj);
	lds.world->add_boss(dpilot);
}


typedef void (*load_routine)(LoadStorage&);
const load_routine load_routines[] = {
	load_casino,
	load_cards,
	load_airfield,
	load_dpilot,
	TestArea::load,
};
// TODO: use a queue of function pointers directly
// TODO: every load a member of the board evrart?!??!?


/*
	construction(CascabelBaseFeature*,CCBManager*,World*)
	ccbf: all common cascabel tools & features
	ccbm: cascabel manager to load level files with
	world: world to load objects & logic for
	purpose: create a worldbuilder to process world loading logic
*/
Worldbuilder::Worldbuilder(CascabelBaseFeature* ccbf,CCBManager* ccbm,World* world)
{
	// setup loader storage
	ldstorage = {
		.m_ccbf = ccbf,
		.m_ccbm = ccbm,
		.world = world
	};

	// preload user interface
	MainMenu* main_menu = new MainMenu(ccbm,ccbf,world,ldstorage.progress,.75f);
	ActionMenu* action_menu = new ActionMenu(ccbf->iMap,ldstorage.progress,.25f);
	world->ui_master = { main_menu,action_menu };
	world->load(ldstorage.progress,.25f);
}

/*
	load() -> void
	purpose: process load instruction queue & load world based on said instructions
*/
void Worldbuilder::load()
{
	// process load instructions
	while (!ldstorage.m_ccbf->ld.empty()) {

		// loading feedback creation
		if (!ldstorage.ldfb_showing) {
			ldstorage.progress = .0f;
			ldstorage.ldfb_showing = true;
			std::thread load_fdb(show_load_progression,&ldstorage);
			load_fdb.detach();
		}
		// FIXME: yes, this causes a memory leak ...too bad. called by member?

		load_routines[ldstorage.m_ccbf->ld.front()](ldstorage);
		ldstorage.m_ccbf->ld.pop();

		// load & switch to action ui
		ldstorage.world->active_daui = 1;
		ldstorage.world->load(ldstorage.progress,.2f);

		// check loading feedback conclusion
		ldstorage.ldfb_showing = !ldstorage.m_ccbf->ld.empty();
	}
}

/*
	show_load_progression(bool*,CascabelBaseFeature*,float*) -> void
	purpose: display loading screen by a different thread, to visualize load progression
	// TODO: adjust params
*/
void Worldbuilder::show_load_progression(LoadStorage* lds)
{
	// visualization setup
	SDL_GLContext context = Core::gFrame.create_new_context();

	// loading bar setup
	size_t t_vsize = 0;
	std::vector<float> ld_canvas = std::vector<float>(PATTERN_SPRITE_TRIANGLE_REPEAT);
	Toolbox::create_sprite_canvas_triangled(ld_canvas,t_vsize,glm::vec2(0),700,10);		// TODO: use constants

	// bar upload
	Buffer ld_buffer = Buffer();
	ld_buffer.bind();
	ld_buffer.upload_vertices(ld_canvas);
	Shader ld_shader = Shader();
	ld_shader.compile2d("./shader/loader/bar.vs","./shader/loader/bar.fs");
	ld_shader.upload_camera();

	// bar borders setup
	float x_lft = 528.0f,x_rgt = 1232.0f,y_dwn = 42.0f,y_up = 57.0f,brd_offset = 4;
	std::vector<float> ld_bar = {
		x_lft,y_up, x_lft+brd_offset,y_up, x_lft,y_up, x_lft,y_up-brd_offset,
		x_rgt,y_up, x_rgt-brd_offset,y_up, x_rgt,y_up, x_rgt,y_up-brd_offset,
		x_lft,y_dwn, x_lft+brd_offset,y_dwn, x_lft,y_dwn, x_lft,y_dwn+brd_offset,
		x_rgt,y_dwn, x_rgt-brd_offset,y_dwn, x_rgt,y_dwn, x_rgt,y_dwn+brd_offset,
	};

	// border upload
	Buffer brd_buffer = Buffer();
	brd_buffer.bind();
	brd_buffer.upload_vertices(ld_bar);
	Shader brd_shader = Shader();
	brd_shader.compile("./shader/standard/direct.vs","./shader/loader/border.fs");
	brd_shader.def_attributeF("position",2,0,2);
	brd_shader.upload_camera();

	// render loop
	while (lds->ldfb_showing) {

		// clear loading screen
		Core::gFrame.clear(.1f,.1f,.1f);
		Core::gFrame.cpu_vsync();
		Core::gFrame.calc_time_delta();
		// create stylized background animation OR timed background action art iterations

		// prepare bar
		ld_shader.enable();
		ld_buffer.bind();

		// translate bar
		glm::mat4 rmodel = glm::translate(glm::mat4(1.0f),glm::vec3(530,45,0));
		rmodel = glm::scale(rmodel,glm::vec3(lds->progress,1,1));
		ld_shader.upload_matrix("model",rmodel);

		// render bar
		glDrawArrays(GL_TRIANGLES,0,6);

		// border rendering
		brd_shader.enable();
		brd_buffer.bind();
		glDrawArrays(GL_LINES,0,16);

		// update loading screen
		Core::gFrame.update();
	}
	SDL_GL_DeleteContext(context);
}
// TODO: what if multiple load instructions are in queue? the loading bar will reset! fix this!
