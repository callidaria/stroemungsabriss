#define GLM_ENABLE_EXPERIMENTAL 1

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/core.h"


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
	uint16_t sid = Core::g_Renderer.add_sprite(0,"./res/bllt_ffdir.png");
	Core::g_Renderer.add_sprite(0,"./res/continue_dialogue.png",2,3,5);
	Core::g_Renderer.register_sprite(0,sid,glm::vec2(150,150),100,100);
	Core::g_Renderer.register_sprite(0,sid+1,glm::vec2(300,200),75,75,true,20);
	Core::g_Renderer.bfr_sprite[0].attribs.state = RBFR_LOAD;
	Core::g_Renderer.bfr_sprite[0].attribs.auto_stateswitch = true;

	// MAIN LOOP
	bool run = true;
	while (run)
	{
		// frame
		//Core::gFrame.calc_time_delta();
		//Core::gFrame.cpu_vsync();
		//Core::gFrame.print_fps();
		Core::g_Input.update(run);
		Frame::clear();

		// update
		//Core::gRenderer.update();
		Core::g_Frame.update();
	}

	// vanish
	Core::g_Renderer.close();
	Core::g_Input.close();
	Core::g_Frame.vanish();
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
