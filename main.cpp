#define GLM_ENABLE_EXPERIMENTAL 1

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/fcn/input.h"
#include "ccb/gfx/renderer.h"


int main(int argc,char** argv)
{
	// settings
	Frame::gpu_vsync_on();
	//g_Frame.set_refresh_rate(60);

	// register test assets
	g_Renderer.compile("./lvload/test_scene.ccb");

	// MAIN LOOP
	bool run = true, reboot = false;
	while (run)
	{
		// frame
		//g_Frame.calc_time_delta();
		//g_Frame.cpu_vsync();
		//g_Frame.print_fps();

		// update
		g_Input.update(run);

		// render
		Frame::clear();
		g_Renderer.update();
		g_Frame.update();
	}

	// reboot on request
	if (reboot)
#ifdef __WIN32__
		ShellExecute(NULL,NULL,"yomisensei.exe",NULL,NULL,SW_SHOW);
#else
		system("./yomisensei &");
#endif

	// close
	COMM_RST();
	g_Input.close();
	g_Frame.vanish();
	return 0;
}
