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
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ??maybe outside option to clear without depth buffer
}

/*
	update() -> void
	purpose: swaps frame information
*/
void Frame::update()
{ SDL_GL_SwapWindow(m_frame); }

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
	time_cticks = SDL_GetTicks();

	// calculate time delta
	time_delta = ((time_cticks-time_pticks)/1000.0f)*time_mod;
}

/*
	set_tmod(float) -> void
	tmod: new value of time modificator to
	purpose: directly set the value of the time modificator to the given value
*/
void Frame::set_tmod(float tmod)
{ time_mod = tmod; }

/*
	change_tmod(float,float) -> void
	goal: target value of time modificator time_mod after change
	rate: rate at which the time modificator time_mod should change towards the target value
	purpose: changing time modification to a set goal at a given rate
*/
void Frame::change_tmod(float goal,float rate)
{ time_mod += rate*(goal>time_mod)-rate*(goal<time_mod); }

/*
	input(uint32_t&,bool) -> void
	running: value to negate if the window gets closed
	tinput (false): defines if text input mode is enabled
	purpose: processes keyboard, controller and mouse input
*/
void Frame::input(uint32_t &running)
{
	// FIXME: this is loopcode. make it work as such !!branches!!
	event_active = false;
	while (SDL_PollEvent(&m_fe)) {
		event_active = true;
		running = m_fe.type!=SDL_QUIT; // exit the program when closing is requested

		// read keyboard input
		if (m_fe.type==SDL_KEYDOWN) kb.ka[m_fe.key.keysym.scancode] = true;
		if (m_fe.type==SDL_KEYUP) kb.ka[m_fe.key.keysym.scancode] = false;
		if (m_fe.type==SDL_KEYDOWN&&m_fe.key.keysym.sym==SDLK_BACKSPACE&&tline.length()>0)
			tline.pop_back();
		if (m_fe.type==SDL_TEXTINPUT) tline+=m_fe.text.text;

		// read mouse input
		SDL_GetMouseState(&mouse.mx,&mouse.my);
		mouse.mxfr = (float)mouse.mx/w_res; // ??make those optional
		mouse.myfr = (float)(h_res-mouse.my)/h_res;
		// !!fix on move cancellation && carry boolean when button released
		if (m_fe.type==SDL_MOUSEBUTTONDOWN) { // ??breakdown to boolean equasion
			mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT;
		} if (m_fe.type==SDL_MOUSEBUTTONUP) { // !!not the practical way
			mouse.mcl = m_fe.button.button!=SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button!=SDL_BUTTON_RIGHT;
		}
		/*mouse.mcl = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_LEFT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_LEFT);
		mouse.mcr = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_RIGHT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_RIGHT);*/
		/*mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT&&m_fe.type==SDL_MOUSEBUTTONDOWN;
		mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT&&m_fe.type==SDL_MOUSEBUTTONDOWN;*/
		mouse.mw = m_fe.wheel.y;

		// check for controller plug-in
		if (m_fe.type==SDL_JOYDEVICEADDED||m_fe.type==SDL_JOYDEVICEREMOVED) {
			kill_controllers();
			load_controllers();
			controller_remap = true;
		}

		// read controller input
		for (int i=0;i<m_gc.size();i++) {
			for (int j=0;j<6;j++)
				xb.at(i).xba[j] = SDL_GameControllerGetAxis(m_gc.at(i),(SDL_GameControllerAxis)j);
			for (int j=0;j<16;j++)
				xb.at(i).xbb[j] = SDL_GameControllerGetButton(m_gc.at(i),(SDL_GameControllerButton)j);
		}
		// face buttons have the default xbox layout so for sony it is X=A,O=B,sq=X and delta=Y
		// results in SDL_CONTROLLER_BUTTON_* const for nintendo controllers having exchanged a&b recognition
		// switch input refuses to be read. conn ok but no prints
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
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
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
	m_gc.clear();
}

/*
	input_start() -> void
	purpose: activate/deactivate text input mode
*/
void Frame::input_start()
{ SDL_StartTextInput(); }
void Frame::input_stop()
{ SDL_StopTextInput(); }

/*
	get_time_delta() -> float
	returns: time delta between render updates to disconnect physics from non-synced fps counts
	DEPRECATED: do not calculate for 60 individual ticks but for per-second-delta
*/
float Frame::get_time_delta()
{ return time_delta*60; }

/*
	create_new_context() -> SDL_GLContext
	\returns new render context for the current thread this was called by
*/
SDL_GLContext Frame::create_new_context()
{ return SDL_GL_CreateContext(m_frame); }