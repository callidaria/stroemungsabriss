#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/fcn/input.h"
#include "ccb/gfx/renderer.h"


struct SceneData
{
	bool* running;
	RenderBatch* batch0,*batch1;
	bool await = false;
	bool scene_ready = false;
	bool second_request = false;
};

void load_scene(SceneData& data)
{
	if (data.await)
	{
		data.scene_ready = data.batch0->state==RBFR_READY;
		return;
	}
	data.batch0 = g_Renderer.load("./lvload/test_scene1.ccb");
	data.await = true;
}

void maintain_scene(SceneData& data)
{
	// check closing request
	*data.running = !g_Input.kb.ka[SDL_SCANCODE_Q]&&*data.running;

	// request additional batch load
	if (g_Input.kb.ka[SDL_SCANCODE_B]&&!data.second_request)
	{
		data.batch1 = g_Renderer.load("./lvload/test_scene0.ccb");
		data.second_request = true;
	}

	int8_t camDir = g_Input.kb.ka[SDL_SCANCODE_L]-g_Input.kb.ka[SDL_SCANCODE_J];
	g_Camera3D.rotate_around_target(camDir*4);
	g_Camera3D.update();
}

typedef void (*scene_update)(SceneData&);
scene_update update_scene[] = { load_scene,maintain_scene };


int main(int argc,char** argv)
{
	// settings
	Frame::gpu_vsync_on();
	//g_Frame.set_refresh_rate(60);

	// store scene data
	SceneData sdata;

	// MAIN LOOP
	bool run = true, reboot = false;
	sdata.running = &run;
	while (run)
	{
		// frame
		//g_Frame.calc_time_delta();
		//g_Frame.cpu_vsync();
		//g_Frame.print_fps();

		// update
		g_Input.update(run);
		update_scene[sdata.scene_ready](sdata);

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
		uint32_t _ = system("./yomisensei &");
#endif

	// close
	COMM_RST();
	g_Input.close();
	g_Frame.vanish();
	return 0;
}
