#define GLM_ENABLE_EXPERIMENTAL 1

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/fcn/input.h"
#include "ccb/gfx/renderer.h"


int main(int argc,char** argv)
{
	// INIT
	/*Init("config.ini");
	Core::gFrame = Frame(
			"黄泉先生",
			Init::iConfig[InitVariable::FRAME_DISPLAY_ID],
			Init::iConfig[InitVariable::FRAME_RESOLUTION_WIDTH],
			Init::iConfig[InitVariable::FRAME_RESOLUTION_HEIGHT],
			(SDL_WindowFlags)Init::iConfig[InitVariable::FRAME_SET_FULLSCREEN]
		);*/
	Frame::gpu_vsync_on();
	//Core::gFrame.set_refresh_rate(60);

	// register test assets
	g_Renderer.compile("./lvload/test_scene.ccb");
	g_Renderer.register_duplicate(0,0,glm::vec2(0,0),25,25);
	g_Renderer.batches[0].inst_sprites[0].upload[0].offset = glm::vec2(1200,700);
	g_Renderer.batches[0].inst_sprites[0].upload[1].offset = glm::vec2(370,420);
	g_Renderer.batches[0].inst_sprites[0].upload[2].offset = glm::vec2(640,360);
	g_Renderer.batches[0].inst_sprites[0].active_range = 3;

	// MAIN LOOP
	bool run = true;
	while (run)
	{
		// frame
		//Core::gFrame.calc_time_delta();
		//Core::gFrame.cpu_vsync();
		//Core::gFrame.print_fps();
		g_Input.update(run);
		Frame::clear();

		// update
		g_Renderer.update();
		g_Frame.update();
	}

	// vanish
	COMM_RST();
	g_Input.close();
	g_Frame.vanish();
	return 0;

	// LOADERS
	/*
	CCBManager ccbm = CCBManager();

	// BUILD SET
	World world = World();

	// WORLD LOADING
	Worldbuilder wb = Worldbuilder(&ccbm,&world);

#if BUILD_DEV_MODE
	bool dactive = false;
#endif

	// MAIN LOOP
	bool run = true,reboot = false;
	while (run) {

		// process loading requests
		wb.load();

		// timing & raw input
		Core::gFrame.calc_time_delta();
		Core::gFrame.cpu_vsync();
		Core::gFrame.print_fps();
		Core::gFrame.input(run);
		Frame::clear();

		// input mapping
		Base::gIMap.update();
		Base::gIMap.precalculate_all();
		Base::gIMap.stick_to_dpad();

		// render scene
		world.render(run,reboot);

		// debrief
		Base::gIMap.update_triggers();

		// developer tools in debug mode
#if BUILD_DEV_MODE
		ccbm.dev_console(run,dactive);
#endif

		// flip
		Core::gFrame.update();
	}

	// TRIGGER REBOOT
	if (reboot)
#ifdef __WIN32__
		ShellExecute(NULL,NULL,"yomisensei.exe",NULL,NULL,SW_SHOW);
#else
		system("./yomisensei &");
#endif

	// CLOSING
	Core::gR3D.clear_memory();
	Core::gFrame.vanish();
	return 0;
	*/
}
