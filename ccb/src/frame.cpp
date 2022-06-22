#include "../frm/frame.h"

/*
 *	!!! ADD OUTPUT INFORMATION ABOUT SYSTEM !!!
 *	ALSO MAYBE IMPLEMENT WITH VIEWPORT RIGHT FROM THE BEGINNING ?
 * */
Frame::Frame(const char* title,int screen,SDL_WindowFlags fs)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	w_res = dim_screen.w;
	h_res = dim_screen.h;
	setup(title,dim_screen.x,dim_screen.y,dim_screen.w,dim_screen.h,fs);
}
Frame::Frame(const char* title,int width,int height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	init();
	setup(title,SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,width,height,fs);
}
Frame::Frame(const char* title,int screen,int width,int height,SDL_WindowFlags fs)
	: w_res(width),h_res(height)
{
	init();

	SDL_Rect dim_screen;
	get_screen(screen,&dim_screen);

	setup(title,dim_screen.x+100,dim_screen.y+100,width,height,fs);
}
void Frame::clear(float cx,float cy,float cz)
{
	glClearColor(cx,cy,cz,1);
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); // ??maybe outside option to clear without depth buffer
}
void Frame::update() { SDL_GL_SwapWindow(m_frame); }
void Frame::print_fps()
{
        m_fps++;
	// !!reduce branch for pipeline
        if (1000<=SDL_GetTicks()-m_pT) { // !!incredibly high numbers will be saved. lets not do this
                m_pT = SDL_GetTicks();
                printf("\rFPS: %i",m_fps);fflush(stdout);
                m_fps = 0;
        }
}
/*
 *	THIS MIGHT BE EFFICTIVE FOR A VSYNC FUNCTION BUT THIS IS !!!NOT!!! APPROPRIATE FOR DELTA TIME RELIANCE !
 *	MAKE VSYNC OPTIONAL AND BASE TIME RELATED UPDATES AND PHYSICS TO A LEGITIMATE DELTA TIME .
 *	THIS ISN'T THE 90s ANYMORE GODDAMMIT . THE LATER THIS HAPPENS THE MORE CODE HAS TO BE CHANGED WHEN IT DOES
 * */
void Frame::vsync(unsigned int mf)
{
	m_pT = m_cT;
	m_cT = SDL_GetTicks();
	m_tempFPS++;
	if (m_cT-m_lO>=1000) {
		m_lO = m_cT;
		m_fps = m_tempFPS;
		m_tempFPS = 0;
	} if (m_cT-m_pT<1000/mf) SDL_Delay(1000/mf-SDL_GetTicks()+m_pT);
}
void Frame::input(uint32_t &running,bool tinput)
{
	// FIXME: this is loopcode. make it work as such !!branches!!
	event_active = false;
	while (SDL_PollEvent(&m_fe)) {
		event_active = true;
		running = m_fe.type!=SDL_QUIT; // exit the program when closing is requested

		// read keyboard input
		// ??are scancodes slower that sdlks
		if (m_fe.type==SDL_KEYDOWN) kb.ka[m_fe.key.keysym.scancode] = true;
		if (m_fe.type==SDL_KEYUP) kb.ka[m_fe.key.keysym.scancode] = false;
		if (m_fe.type==SDL_KEYDOWN&&m_fe.key.keysym.sym==SDLK_BACKSPACE&&tline.length()>0)
			tline.pop_back();
		if (m_fe.type==SDL_TEXTINPUT) tline+=m_fe.text.text;

		// read mouse input
		SDL_GetMouseState(&mouse.mx,&mouse.my);
		mouse.mxfr = ((float)mouse.mx/w_res)*1920.0f; // ??make those optional
		mouse.myfr = ((float)(h_res-mouse.my)/h_res)*1080.0f;
		// !!fix on move cancellation && carry boolean when button released
		/*if (m_fe.type==SDL_MOUSEBUTTONDOWN) { // ??breakdown to boolean equasion
			mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT;
		} if (m_fe.type==SDL_MOUSEBUTTONUP) { // !!not the practical way
			mouse.mcl = m_fe.button.button!=SDL_BUTTON_LEFT;
			mouse.mcr = m_fe.button.button!=SDL_BUTTON_RIGHT;
		}*/
		mouse.mcl = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_LEFT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_LEFT);
		mouse.mcr = (m_fe.type==SDL_MOUSEBUTTONDOWN&&m_fe.button.button==SDL_BUTTON_RIGHT)
				||(m_fe.type==SDL_MOUSEBUTTONUP&&m_fe.button.button!=SDL_BUTTON_RIGHT);
		/*mouse.mcl = m_fe.button.button==SDL_BUTTON_LEFT&&m_fe.type==SDL_MOUSEBUTTONDOWN;
		mouse.mcr = m_fe.button.button==SDL_BUTTON_RIGHT&&m_fe.type==SDL_MOUSEBUTTONDOWN;*/
		mouse.mw = m_fe.wheel.y;

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
void Frame::vanish()
{
	printf("\n");
	// ??doing this with an array reference even cleaner when converted & test when valgrind isn't mad anymore
	for (int i=0;i<m_gc.size();i++) SDL_GameControllerClose(m_gc.at(i)); // closing controller reference

	// closing audio context & device
	alcMakeContextCurrent(NULL);
	alcDestroyContext(m_alccon);
	alcCloseDevice(m_alcdev);

	// closing render context & program
	SDL_GL_DeleteContext(m_context);
	SDL_Quit();
}
void Frame::init()
{
	// sdl setup
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK,SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION,3);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE,8);
	SDL_StopTextInput();
}
void Frame::setup(const char* title,int x,int y,int width,int height,SDL_WindowFlags fs)
{
	// creating window
	m_frame = SDL_CreateWindow(title,x,y,width,height,SDL_WINDOW_OPENGL);
	SDL_SetWindowFullscreen(m_frame,fs);
	m_context = SDL_GL_CreateContext(m_frame);

	// opengl setup
	glewInit();
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	glViewport(0,0,width,height);

	// openal setup
	m_alcdev = alcOpenDevice(NULL);
	m_alccon = alcCreateContext(m_alcdev,NULL);
	alcMakeContextCurrent(m_alccon);

	// controller setup
	int gcc = 0;
	while (SDL_IsGameController(gcc)) {
		m_gc.push_back(SDL_GameControllerOpen(gcc));
		xb.push_back(XBox()); // !!negative points for style ...maybe stack usage instead???
		gcc++;
	} printf("\033[0;34mcontrollers: %i plugged in\n",gcc);

	m_cT = 0; m_fps = 0; m_tempFPS = 0; m_lO = 0; // ??all necessary & syntax
}
void Frame::get_screen(int screen,SDL_Rect* dim_screen)
{
	if (screen<SDL_GetNumVideoDisplays()&&SDL_GetDisplayBounds(screen,dim_screen)==0)
		printf("\033[1;36mmaximum resolution of selected screen is: %ix%i\n",dim_screen->w,dim_screen->h);
	else {
		printf("\033[1;31mscreen could not be set: %s\n",SDL_GetError());
		printf("\033[1;36m\t=> falling back to standard configuration\n");
		dim_screen->x = 0; dim_screen->y = 0; dim_screen->w = 1280; dim_screen->h = 720;
	}
}
void Frame::input_start() { SDL_StartTextInput(); }
void Frame::input_stop() { SDL_StopTextInput(); }
