#define GLM_ENABLE_EXPERIMENTAL 1
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"

#include "ccb/gfx/renderer2d.h"
#include "ccb/gfx/renderer3d.h"
#include "ccb/gfx/rendereri.h"

#include "ccb/mat/camera2d.h"
#include "ccb/mat/camera3d.h"

#include "ccb/ppe/msaa.h"
#include "ccb/ppe/bloom.h"

#include "ccb/fcn/text.h"
#include "ccb/fcn/init.h"

#include "ccb/aud/audio.h"
#include "ccb/aud/listener.h"

#include "ccb/fcn/ccb_manager.h"

#include "ccb/gfx/light3d.h"
#include "ccb/gfx/material3d.h"

#include "script/systems/input_map.h"
#include "script/systems/worldbuilder.h"

#include "script/menu/menu.h"

#include "script/world.h"

#define MVMT_SPEED 4
#define BUILD_DEV_MODE 1

int main(int argc,char** argv)
{
	// INIT
	Init init = Init("config.ini");
	Frame f = Frame("黄泉先生",init.rINT(init.FRAME_DISPLAY_ID),
			init.rINT(init.FRAME_RESOLUTION_WIDTH),init.rINT(init.FRAME_RESOLUTION_HEIGHT),
			(SDL_WindowFlags)init.rINT(init.FRAME_SET_FULLSCREEN));
	InputMap imap = InputMap(&f);

	// AUDIO
	Listener listener = Listener();

	// RENDERERS
	Renderer2D r2d = Renderer2D();
	Renderer3D r3d = Renderer3D();
	RendererI ri = RendererI();
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	BulletSystem bsys = BulletSystem(&f,&ri);

	bool dactive = false;

	// LOADERS
	CascabelBaseFeature eref = { &f,&r2d,&r3d,&ri,&bsys,&imap };
	World world = World(&eref);
	CCBManager ccbm = CCBManager(&f,&r2d,&cam2d);

	// BUILD SET
	world.add_camera(cam2d);
	world.add_camera(Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f));
	world.add_camera(Camera3D(1280.0f,720.0f));
	world.add_camera(Camera3D(glm::vec3(5,5,5),1280.0f,720.0f,45.0f));

	// WORLD LOADING
	Worldbuilder wb = Worldbuilder(&eref,&ccbm,&world);
	eref.ld.push(LOAD_CASINO);

	// MAIN LOOP
	uint32_t run=1;
	bool reboot = false;
	while (run) {

		// process loading requests
		wb.load();

		// timing & raw input
		f.print_fps();
		f.calc_time_delta();
		f.input(run);

		// input mapping
		imap.update();
		imap.precalculate_all();

		// render scene
		world.render(run,reboot);

		// developer tools in debug mode
#if BUILD_DEV_MODE
		ccbm.dev_console(run,dactive);
#endif

		// flip
		f.update();
	}

	// TRIGGER REBOOT
	if (reboot)
#ifdef __WIN32__
		ShellExecute(NULL,NULL,"yomisensei.exe",NULL,NULL,SW_SHOW);
#else
		system("./yomisensei &");
#endif

	// CLOSING
	world.free_memory();
	ccbm.vanish();
	f.vanish();
	return 0;
}