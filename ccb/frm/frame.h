#ifndef CCB_FRAME_SELFTITLED
#define CCB_FRAME_SELFTITLED

#include <iostream>
#include <thread>
#include <chrono>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <AL/alc.h>

#include "../mat/toolbox.h"

// timing
constexpr double FRAME_REFRATE_DEFAULT_DELTA = 1./60.;

struct Keyboard
{
	bool ka[285] = { false };	// list of keyboard scan results
};

struct Mouse
{
	bool mcl = false,mcr = false;	// mouse button requests
	int32_t mx,my;			// mouse positions x,y
	float mxfr,myfr;		// mouse position x,y scaled to absolute range
	int32_t mw;			// mouse wheel movements
};

struct XBox
{
	int xba[6];			// controller axis tilts
	bool xbb[16] = { false };	// controller button requests
};

class Frame
{
public:

	// construction
	Frame(const char* title="callidaria-program",int8_t screen=0,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title="callidaria-program",int16_t width=1280,int16_t height=720,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title="callidaria-program",int8_t screen=0,int16_t width=1280,
			int16_t height=720,SDL_WindowFlags fs=(SDL_WindowFlags)0);
	~Frame() {  }

	// modification
	static void clear(float cr=0,float cg=0,float cb=0);
	void update();

	// time
	void calc_time_delta();
	void set_tmod(double tmod);
	void change_tmod(double goal,double rate);

	// fps
	inline void set_refresh_rate(double refresh_rate=60.0) { rate_delta = 1./refresh_rate; }
	static void gpu_vsync_on();
	static inline void gpu_vsync_off() { SDL_GL_SetSwapInterval(0); }
	void cpu_vsync();
	void print_fps();

	// controlling
	void input(uint32_t &running);

	// termination
	void vanish();

	// text input
	void input_start();
	void input_stop();

	// getter
	double get_time_delta();
	SDL_GLContext create_new_context();

private:

	// setup helper
	void init();
	void setup(const char* title,GLuint x,GLuint y,int16_t width,int16_t height,
			SDL_WindowFlags fs);
	void get_screen(int8_t screen,SDL_Rect* dim_screen);

	// controller management
	void load_controllers();
	void kill_controllers();

public:

	// properties
	int w_res,h_res;
	bool event_active = false;

	// input
	std::vector<SDL_GameController*> m_gc;
	SDL_Event m_fe;
	Keyboard kb;
	Mouse mouse;
	std::vector<XBox> xb;
	std::string tline = "";
	bool controller_remap = true;

private:

	// visuals
	SDL_Window* m_frame;
	SDL_GLContext m_context;

	// audio
	ALCdevice* m_alcdev;
	ALCcontext* m_alccon;

	// time & vsync
	std::chrono::steady_clock::time_point past_ticks,curr_ticks = std::chrono::steady_clock::now(),
			last_out = std::chrono::steady_clock::now();
	uint32_t fps = 0,temp_fps = 0;
	double time_mod = 1.,time_delta = .0,time_delta_nmod = .0;
	double rate_delta = FRAME_REFRATE_DEFAULT_DELTA;
	double stalled_time = .0;
};

#endif
