#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/fcn/input.h"
#include "ccb/gfx/renderer.h"


struct SceneData
{
	// pointers
	bool* running;
	RenderBatch* batch0,*batch1;

	// load handling
	bool scene_ready = false;
	bool second_request = false;

	// mouse rotation interfacing
	float dt_mouse_position;
	float ape_momentum = .0f;
};

void load_scene(SceneData& data)
{
	data.batch0 = g_Renderer.load("./lvload/test_scene1.ccb");

	g_Renderer.m_Lighting.directional_lights[0] = {
		.position = glm::vec3(50,-25,75),
		.colour = glm::vec3(1.f),
		.intensity = 4.2f
	};
	g_Renderer.m_Lighting.directional_lights[1] = {
		.position = glm::vec3(-50,25,-75),
		.colour = glm::vec3(1.f),
		.intensity = .125f
	};
	g_Renderer.m_Lighting.directional_range+=2;

	data.scene_ready = true;
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

	// calculate camera to target object rotation
	int8_t t_KeyDirection = g_Input.kb.ka[SDL_SCANCODE_L]-g_Input.kb.ka[SDL_SCANCODE_J];
	float t_MouseDirection = (data.dt_mouse_position-g_Input.mouse.mxfr)*g_Input.mouse.mb[0];
	data.ape_momentum += t_KeyDirection+t_MouseDirection*100;
	g_Camera3D.rotate_around_target(data.ape_momentum);
	g_Camera3D.update();
	data.ape_momentum *= .85f;
	data.dt_mouse_position = g_Input.mouse.mxfr;
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
