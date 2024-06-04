#define GLM_ENABLE_EXPERIMENTAL 1

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/core.h"

#include "script/base.h"
#include "script/systems/worldbuilder.h"

#define MVMT_SPEED 4
#define BUILD_DEV_MODE 1

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
	//Frame::gpu_vsync_on();
	Core::gFrame.set_refresh_rate(60);

	// register test assets
	Renderer renderer = Renderer();
	uint16_t sid = renderer.add_sprite(0,"./res/bllt_ffdir.png");
	renderer.register_sprite(0,sid,glm::vec2(150,150),100,100);
	renderer.load_buffer_sprites(0);

	// MAIN LOOP
	bool run = true;
	while (run) {

		// frame
		Core::gFrame.calc_time_delta();
		Core::gFrame.cpu_vsync();
		Core::gFrame.print_fps();
		Core::gFrame.input(run);
		Frame::clear();

		// update
		renderer.update();
		Core::gFrame.update();
	}

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
	*/
	Core::gFrame.vanish();
	return 0;
}
