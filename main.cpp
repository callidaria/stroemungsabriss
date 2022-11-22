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

#include "script/menu/menu.h"

#define MVMT_SPEED 4
#define BUILD_DEV_MODE 1

int main(int argc,char** argv)
{
	Init init = Init("config.ini");
	Frame f = Frame("黄泉先生",init.rINT(init.FRAME_DISPLAY_ID),init.rINT(init.FRAME_RESOLUTION_WIDTH),
		init.rINT(init.FRAME_RESOLUTION_HEIGHT),(SDL_WindowFlags)init.rINT(init.FRAME_SET_FULLSCREEN));

	// AUDIO
	Listener listener=Listener();

	// RENDERERS
	Renderer2D r2d = Renderer2D();
	Renderer3D r3d = Renderer3D();
	RendererI ri = RendererI();
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	Camera3D cam3d = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f);

	// FONTS
	std::vector<Font> fonts = {
		Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",20,20)
	};
	for (int i=0;i<fonts.size();i++) fonts[i].texture();

	bool dactive = false;

	CCBManager ccbm = CCBManager(&f,&r2d,&cam2d);
	Menu menu = Menu(&ccbm,&f,&r2d,&r3d,&ri,&cam2d,&cam3d,&fonts);

	// CAMERAS
	r2d.load(&cam2d);
	ri.load(&cam2d);
	uint32_t run=1,pause=false;
	bool reboot = false;
	glm::mat4 model = glm::mat4(1.0f);
	while (run) {
		f.print_fps();
		f.calc_time_delta();
		f.input(run,dactive);

		// INPUT
		if (f.kb.ka[SDL_SCANCODE_ESCAPE]) break;

		menu.render(run,reboot);
#if BUILD_DEV_MODE
		ccbm.dev_console(run,dactive);
#endif
		f.update();
	}

	if (reboot)
#ifdef __WIN32__
		ShellExecute(NULL,NULL,"yomisensei.exe",NULL,NULL,SW_SHOW);
#else
		system("./yomisensei &");
#endif

	f.vanish();
	return 0;
}