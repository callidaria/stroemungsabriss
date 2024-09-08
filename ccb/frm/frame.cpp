#include "frame.h"

/*
	!O(1)bm /load -> (public)
	purpose: create window and setup api for rendering
	\param title: window title
*/
Frame::Frame(const char* title)
{
	// api initialization
	// sdl setup
	COMM_MSG(LOG_HEADINGS,"frame setup...");
	COMM_LOG("setup sdl version 3.3");
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
	//SDL_ShowCursor(SDL_DISABLE);
	// TODO: dynamically hide the cursor, when controller input is mainly used
	// FIXME: conflicted reporting regarding SDL_GL_SHARE_WITH_CURRENT_CONTEXT attribute

	// screen information
	COMM_LOG("get display hardware information");
	int8_t screen_id = g_Config.iFrameTargetMonitor;
	SDL_Rect dim_screen = { .x = 0,.y = 0,.w = 1280,.h = 720 };
	bool conf_hardware = screen_id<SDL_GetNumVideoDisplays()&&!SDL_GetDisplayBounds(screen_id,&dim_screen);
	COMM_MSG_COND(
		conf_hardware,
		LOG_YELLOW,
		"maximum resolution of selected screen is: %ix%i",
		dim_screen.w,dim_screen.h
	) COMM_ERR_FALLBACK(
		"screen could not be set: %s\n%s",SDL_GetError(),
		"falling back to standard configuration"
	);

	// environmental setup
	// creating window
	COMM_LOG("window creation");
	w_res = g_Config.vFrameResolutionWidth, h_res = g_Config.vFrameResolutionHeight;
	m_frame = SDL_CreateWindow(title,dim_screen.x+100,dim_screen.y+100,w_res,h_res,SDL_WINDOW_OPENGL);
	SDL_SetWindowFullscreen(m_frame,(SDL_WindowFlags)g_Config.bFrameFullscreen);
	m_context = SDL_GL_CreateContext(m_frame);

	// opengl setup
	COMM_LOG("opengl setup");
	glewInit();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glViewport(0,0,w_res,h_res);

	// openal setup
	COMM_LOG("audio setup");
	m_alcdev = alcOpenDevice(NULL);
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon);

	// default settings
	set_clear_colour();

	COMM_SCC("frame initialization done");
}

/*
	calc_time_delta() -> void
	purpose: calculate time delta to disconnect frame from update and make physics timebased
*/
void Frame::calc_time_delta()
{
	past_ticks = curr_ticks;
	curr_ticks = std::chrono::steady_clock::now();
	time_delta_nmod = (curr_ticks-past_ticks).count()*CONVERSION_MULT_SECONDS;
	time_delta = time_delta_nmod*time_mod;
}

/*
	change_tmod(float,float) -> void
	goal: target value of time modificator time_mod after change
	rate: rate at which the time modificator time_mod should change towards the target value
	purpose: changing time modification to a set goal at a given rate
*/
void Frame::change_tmod(double goal,double rate)
{ time_mod += rate*(goal>time_mod)-rate*(goal<time_mod); }

/*
	!O(1)b /load -> (public)
	purpose: enable GPU vsync (adaptive if possible)
*/
void Frame::gpu_vsync_on()
{
	COMM_AWT("setting gpu vsync");

	// trying to set adaptive vsync
	if (SDL_GL_SetSwapInterval(-1)==-1)
	{
		COMM_ERR("adaptive vsync is not supported");
		SDL_GL_SetSwapInterval(1);
	}

	COMM_CNF();
}

/*
	!O(1)b /update -> (public)
	purpose: capping frames per second to given value
	NOTE: calc_time_delta() has to be run before, but within the same frame (updates clock progress)
*/
void Frame::cpu_vsync()
{
	if (time_delta_nmod<rate_delta)
	{
		double lft_delta = rate_delta-time_delta_nmod;
		std::this_thread::sleep_for(
				std::chrono::milliseconds((uint32_t)(lft_delta*CONVERSION_THRES_MILLISECONDS)));
		stalled_time += lft_delta;
	}
}
// FIXME: stall precision looses about 2 frames
// FIXME: still a problem with doubled refresh rate for some unknown reason

/*
	!O(1)b /update -> (public)
	purpose: prints current fps and uts into cascabel console or terminal
*/
void Frame::print_fps()
{
	fps++;
	if (CONVERSION_THRES_NANOSECONDS<=(std::chrono::steady_clock::now()-last_out).count())
	{
		last_out = std::chrono::steady_clock::now();
		printf("\r%ifps -> processing: %fs,  tmod: %f",fps,1.-stalled_time,time_mod);
		fflush(stdout);
		fps = 0,stalled_time = 0;
	}
}
// TODO: add features to display runtime information
// FIXME: NEGATIVE?!?!?!? PROCESSING TIME?!

/*
	vanish() -> void
	purpose: close window and delete audio and render context
*/
void Frame::vanish()
{
	COMM_MSG(LOG_CYAN,"destroying frame...");

	// closing audio context & device
	COMM_LOG("closing audio device");
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
	COMM_LOG("closing graphical device and delete context");
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();

	COMM_SCC("done");
}
