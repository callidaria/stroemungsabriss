#include "frame.h"

/*
	PARAMETER DEFINITIONS:
	title (callidaria-program): frame header title
	screen (0): screen id of frame origin position
	width (1280): frame pixel width
	height (720): frame pixel height
	fs (0): framestate to define if windowed/fullscreen/borderless
*/

/*
	constructor(const char*,int8_t,SDL_WindowFlags)
	purpose: create output frame with given title on given screen
	ADD FEATURE: !!! ADD OUTPUT INFORMATION ABOUT SYSTEM !!!
		ALSO MAYBE IMPLEMENT WITH VIEWPORT RIGHT FROM THE BEGINNING ?
*/
Frame::Frame(const char* title,int8_t screen,SDL_WindowFlags fs)
{
	// initializing
	init();

	// screen informations
	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	// setup routine
	w_res = dim_screen.w;
	h_res = dim_screen.h;
	setup(title,dim_screen.x,dim_screen.y,dim_screen.w,dim_screen.h,fs);
}

/*
	constructor(const char*,int16_t,int16_t,SDL_WindowFlags)
	purpose: create output frame with given title with given frame dimensions
*/
Frame::Frame(const char* title,int16_t width,int16_t height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	init();
	setup(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,fs);
}

/*
	constructor(const char*,int8_t,int16_t,int16_t,SDL_WindowFlags)
	purpose: create output frame with given title with given frame dimensions on given screen
*/
Frame::Frame(const char* title,int8_t screen,int16_t width,int16_t height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	// initializing
	init();

	// screen informations
	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	// setup routine
	setup(title,dim_screen.x+100,dim_screen.y+100,width,height,fs);
}

/*
	clear(float,float,float) -> void (static)
	cr (0): red value of clear colour
	cg (0): green value of clear colour
	cb (0): blue value of clear colour
	purpose: clear screen and active framebuffers with given colour
		will overwrite all previous clears, alpha value is only significant in texture combinations
*/
void Frame::clear(float cr,float cg,float cb)
{
	glClearColor(cr,cg,cb,0);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
}
// TODO: add method to skip depth buffer clear

/*
	print_fps() -> void
	purpose: prints current fps and uts into cascabel console or terminal
*/
void Frame::print_fps()
{
	fps++;
	if (1000<=SDL_GetTicks()-past_ticks) {
		past_ticks = SDL_GetTicks();
		printf("\rFPS: %i    TIME MOD: %f",fps,time_mod);
		fflush(stdout);
		fps = 0;
	}
}

/*
	vsync(uint8_t) -> void
	max_frames (60): maximum frames per second
	purpose: capping frames per second to given value
	!! DEPRECATED: PLEASE KILL BRANCHING, THIS IS PATHETIC LOOPCODE !!
*/
void Frame::vsync(uint8_t max_frames)
{
	// count continue
	past_ticks = current_ticks;
	current_ticks = SDL_GetTicks();
	temp_fps++;

	// reset after running second is completed
	if (current_ticks-lO>=1000) {
		lO = current_ticks;
		fps = temp_fps;
		temp_fps = 0;
	}

	// delay while counting second
	if (current_ticks-past_ticks<1000/max_frames)
		SDL_Delay(1000/max_frames-SDL_GetTicks()+past_ticks);
}

/*
	calc_time_delta() -> void
	purpose: calculate time delta to disconnect frame from update and make physics timebased
*/
void Frame::calc_time_delta()
{
	// circle ticks
	time_pticks = time_cticks;
#ifdef BUILDISSUE_OLD_SDL_VERSION
	time_cticks = SDL_GetTicks();
#else
	time_cticks = SDL_GetTicks64();
#endif

	// calculate time delta
	time_delta = ((time_cticks-time_pticks)/1000.0)*time_mod;
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
	input(uint32_t&,bool) -> void
	running: value to negate if the window gets closed
	tinput (false): defines if text input mode is enabled
	purpose: processes keyboard, controller and mouse input
*/
void Frame::input(bool &running)
{
	// reset
	mouse.mw = 0;

	// process events and set input activity
	event_active = false;
	while (SDL_PollEvent(&m_fe)) {
		event_active = true;
		bool relevant_motion = false;

		// switch input handling (not my favourite necessarily)
		int32_t motion;
		switch (m_fe.type)
		{

		// keyboard input
		case SDL_KEYDOWN: kb.ka[m_fe.key.keysym.scancode] = true;
			break;
		case SDL_KEYUP: kb.ka[m_fe.key.keysym.scancode] = false;
			break;

		// mouse input
		case SDL_MOUSEMOTION:
			SDL_GetMouseState(&mouse.mx,&mouse.my);
			mouse.mxfr = (float)mouse.mx/w_res;
			mouse.myfr = (float)(h_res-mouse.my)/h_res;
			break;
		case SDL_MOUSEBUTTONDOWN: mouse.mb[m_fe.button.button-1] = true;
			break;
		case SDL_MOUSEBUTTONUP: mouse.mb[m_fe.button.button-1] = false;
			break;
		case SDL_MOUSEWHEEL: mouse.mw = m_fe.wheel.y;
			break;

		// controller input
		case SDL_CONTROLLERAXISMOTION:
			motion = SDL_GameControllerGetAxis(m_gc[0],(SDL_GameControllerAxis)m_fe.caxis.axis);
			xb[0].xba[m_fe.caxis.axis] = motion;
			relevant_motion = (abs(motion)>AXIS_MOTION_RELEVANCE)||relevant_motion;
			break;
		case SDL_CONTROLLERBUTTONDOWN: xb[0].xbb[m_fe.cbutton.button] = true;
			break;
		case SDL_CONTROLLERBUTTONUP: xb[0].xbb[m_fe.cbutton.button] = false;
			break;
		// face buttons have the default xbox layout so for sony it is X=A,O=B,sq=X and delta=Y
		// results in SDL_CONTROLLER_BUTTON_* const for nintendo controllers while a&b is exchanged

		// controller hotswapping
		case SDL_JOYDEVICEADDED:
		case SDL_JOYDEVICEREMOVED:
			kill_controllers();
			load_controllers();
			controller_remap = true;
			break;

		// window close request
		case SDL_QUIT: running = false;
			break;
		}
		// TODO: improve further, this seems really simple minded
		// a quick look into the assembly translation of this will surely motivate due to rage
		// maybe solve this by letting different events jump to different functions by pointer
		// above solution may be frowned upon and extremely unsafe, but it's faster so there you go
		// -> !compare translation though because functions would have to be static
		// FIXME: switch input refuses to be read. conn ok but no prints

		// update preferred peripheral
		cpref_peripheral = (cpref_peripheral||m_fe.type==SDL_CONTROLLERBUTTONDOWN||relevant_motion)
				&& !(m_fe.type==SDL_MOUSEBUTTONDOWN||m_fe.type==SDL_KEYDOWN);
		mpref_peripheral = (mpref_peripheral&&!cpref_peripheral&&m_fe.type!=SDL_KEYDOWN)
				|| m_fe.type==SDL_MOUSEMOTION||m_fe.type==SDL_MOUSEBUTTONDOWN||m_fe.type==SDL_MOUSEWHEEL;

		/*if (m_fe.type==SDL_KEYDOWN&&m_fe.key.keysym.sym==SDLK_BACKSPACE&&tline.length()>0)
			tline.pop_back();
		if (m_fe.type==SDL_TEXTINPUT) tline += m_fe.text.text;*/
		// TODO: make this code happen sensibly within this new input processing implementation
	}
}

/*
	vanish() -> void
	purpose: close window and delete audio and render context
*/
void Frame::vanish()
{
	// breakline on out & close controllers
	printf("\n");
	kill_controllers();

	// closing audio context & device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}

/*
	init() -> void
	purpose: initialize basic background needs and set their parameters
*/
void Frame::init()
{
	// sdl setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
	SDL_StopTextInput();
	//SDL_ShowCursor(SDL_DISABLE);
	// TODO: dynamically hide the cursor, when controller input is mainly used
}

/*
	setup(const char*,GLuint,GLuint,int16_t,int16_t,SDL_WindowFlags) -> void
	x: x-axis position of the frame's upper left corner on the desktop
	y: y-axis position of the frame's upper left corner on the desktop
	purpose: setup routine used by all constructors to help create a frame with all it's details
*/
void Frame::setup(const char* title,GLuint x,GLuint y,int16_t width,int16_t height,
		SDL_WindowFlags fs)
{
	// creating window
	m_frame = SDL_CreateWindow(title,x,y,width,height,SDL_WINDOW_OPENGL);
	SDL_SetWindowFullscreen(m_frame,fs);
	m_context = create_new_context();

	// opengl setup
	glewInit();
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LEQUAL);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	glFrontFace(GL_CCW);
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glViewport(0,0,width,height);

	// openal setup
	m_alcdev = alcOpenDevice(NULL);
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon);
}
// FIXME: opening the window will cause doubled controller load

/*
	get_screen(int8_t,SDL_Rect*) -> void
	dim_screen: read requested screen dimensions and write them to SDL_Rect address
	purpose: get screen details to later use them in the setup routine
*/
void Frame::get_screen(int8_t screen,SDL_Rect* dim_screen)
{
	if (screen<SDL_GetNumVideoDisplays()&&SDL_GetDisplayBounds(screen,dim_screen)==0)
		printf("\033[1;36mmaximum resolution of selected screen is: %ix%i\n",dim_screen->w,dim_screen->h);
	else {
		printf("\033[1;31mscreen could not be set: %s\n",SDL_GetError());
		printf("\033[1;36m\t=> falling back to standard configuration\n");
		dim_screen->x = 0,dim_screen->y = 0,dim_screen->w = 1280,dim_screen->h = 720;
	} printf("\033[0m");
}

/*
	load_controllers() -> void
	purpose: load all currently plugged-in controllers
*/
void Frame::load_controllers()
{
	uint8_t gcc = 0;
	while (SDL_IsGameController(gcc)) {
		m_gc.push_back(SDL_GameControllerOpen(gcc));
		xb.push_back(XBox());
		gcc++;
	} printf("\033[0;34mcontrollers: %i plugged in\033[0m\n",gcc);
}

/*
	kill_controllers() -> void
	purpose: close all controllers & remove their input reference structures
*/
void Frame::kill_controllers()
{
	for (int i=0;i<m_gc.size();i++) SDL_GameControllerClose(m_gc.at(i));
	m_gc.clear(),xb.clear();
}
