#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/fcn/input.h"
#include "ccb/gfx/renderer.h"

#include "script/ui/ui.h"


struct SceneData
{
	// pointers
	bool* running;
	RenderBatch* batch0,*batch1;

	// load handling
	bool scene_ready = false;
	bool second_request = false;

	// mouse rotation interfacing
	float dt_mouse_position = .0f;
	float ape_momentum = .0f;
	float ape_zoom = 1.f;
	float zoom_momentum = .0f;
	glm::vec3 cam_position = glm::vec3(0,-4,2);
};

void load_scene(SceneData& data)
{
	data.batch0 = g_Renderer.load("./lvload/test_scene1.ccb");
	data.scene_ready = true;
}

void maintain_scene(SceneData& data)
{
	// system
	// check closing request
	*data.running = !g_Input.kb.ka[SDL_SCANCODE_Q]&&*data.running;

	// timing
	// time manipulation
	g_Frame.time_mod += (g_Input.kb.ka[SDL_SCANCODE_T]-g_Input.kb.ka[SDL_SCANCODE_E])*.05f;
	g_Frame.time_mod = (g_Input.kb.ka[SDL_SCANCODE_R]) ? 1.f : g_Frame.time_mod;

	// loading
	// request additional batch load
	if (g_Input.kb.ka[SDL_SCANCODE_B]&&!data.second_request)
	{
		data.batch1 = g_Renderer.load("./lvload/test_scene0.ccb");
		data.second_request = true;
	}

	// camera movement
	// calculate camera to target object rotation
	int8_t __KeyDirection = g_Input.kb.ka[SDL_SCANCODE_L]-g_Input.kb.ka[SDL_SCANCODE_J];
	float __MouseDirection = (data.dt_mouse_position-g_Input.mouse.mxfr)*g_Input.mouse.mb[0];
	data.ape_momentum += __KeyDirection+__MouseDirection*100;
	g_Camera3D.position = data.cam_position;
	g_Camera3D.rotate_around_target(data.ape_momentum);
	data.cam_position = g_Camera3D.position;

	// calculate camera zoom
	int8_t __KeyZoom = g_Input.kb.ka[SDL_SCANCODE_K]-g_Input.kb.ka[SDL_SCANCODE_I];
	data.zoom_momentum += g_Input.mouse.mw*-.1f+__KeyZoom*.01f;
	data.ape_zoom += data.zoom_momentum;
	g_Camera3D.position *= data.ape_zoom;

	// update camera and reset
	g_Camera3D.update();
	data.ape_momentum *= .85f;
	data.zoom_momentum *= .85f;
	data.dt_mouse_position = g_Input.mouse.mxfr;
}

typedef void (*scene_update)(SceneData&);
scene_update update_scene[] = { load_scene,maintain_scene };


int main(int argc,char** argv)
{
	// setup
	UI ui = UI();

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
		g_Frame.calc_time_delta();
		//g_Frame.print_fps();
		//g_Frame.cpu_vsync();

		// update
		g_Input.update(run);
		ui.update();
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
		uint32_t _rbt = system("./yomisensei &");
#endif

	// close
	COMM_RST();
	g_Input.close();
	g_Frame.vanish();
	return 0;
}
