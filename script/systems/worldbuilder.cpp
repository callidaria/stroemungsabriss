#include "worldbuilder.h"


typedef void (*load_routine)(LoadStorage&);
const load_routine load_routines[] = {
	NevadaCasino::load,
	NepalAirfield::load,
	TestArea::load,
};
// TODO: use a queue of function pointers directly


/*
	construction(CCBManager*,World*)
	ccbf: all common cascabel tools & features
	ccbm: cascabel manager to load level files with
	world: world to load objects & logic for
	purpose: create a worldbuilder to process world loading logic
*/
Worldbuilder::Worldbuilder(CCBManager* ccbm,World* world)
{
	// setup loader storage
	ldstorage = {
		.m_ccbm = ccbm,
		.world = world
	};

	// preload user interface
	MainMenu* main_menu = new MainMenu(ccbm,world,ldstorage.progress,.75f);
	ActionMenu* action_menu = new ActionMenu(ldstorage.progress,.25f);
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
	while (!Core::gLI.empty()) {

		// loading feedback creation
		if (!ldstorage.ldfb_showing) {
			ldstorage.progress = .0f;
			ldstorage.ldfb_showing = true;
			std::thread load_fdb(show_load_progression,&ldstorage);
			load_fdb.detach();
		}
		// FIXME: yes, this causes a memory leak ...too bad. called by member?

		load_routines[Core::gLI.front()](ldstorage);
		Core::gLI.pop();

		// load & switch to action ui
		ldstorage.world->active_daui = 1;
		ldstorage.world->load(ldstorage.progress,.2f);

		// check loading feedback conclusion
		ldstorage.ldfb_showing = !Core::gLI.empty();
	}
}

/*
	show_load_progression(LoadStorage*) -> void
	purpose: display loading screen by a different thread, to visualize load progression
	TODO: adjust params
*/
void Worldbuilder::show_load_progression(LoadStorage* lds)
{
	// visualization setup
	SDL_GLContext context = Core::gFrame.produce_window_context();

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
	float x_lft = 528.0f, x_rgt = 1232.0f, y_dwn = 42.0f, y_up = 57.0f, brd_offset = 4;
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
