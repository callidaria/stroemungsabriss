#ifndef CCB_FRAME_SELFTITLED
#define CCB_FRAME_SELFTITLED

#include <AL/alc.h>

#include "../mat/toolbox.h"
#include "../fcn/config.h"


// timing
constexpr double FRAME_REFRATE_DEFAULT_DELTA = 1./60.;


class Frame
{
public:

	// construction
	Frame(const char* title);
	~Frame() {  }

	// settings
	static inline void set_clear_colour(float r=0,float g=0,float b=0) { glClearColor(r,g,b,0); }

	// utility
	static inline void clear(float cr=0,float cg=0,float cb=0)
		{ glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT); }
	inline void update() { SDL_GL_SwapWindow(m_frame); }
	// TODO: add method to skip depth buffer clear

	// time
	void calc_time_delta();
	void change_tmod(double goal,double rate);

	// fps
	inline void set_refresh_rate(double refresh_rate=60.0) { rate_delta = 1./refresh_rate; }
	static void gpu_vsync_on();
	static inline void gpu_vsync_off() { SDL_GL_SetSwapInterval(0); }
	void cpu_vsync();
	void print_fps();

	// termination
	void vanish();

private:

	// controller management
	void load_controllers();
	void kill_controllers();

public:

	// properties
	bool event_active = false;
	double time_mod = 1.,time_delta = 0;

	// input
	std::string tline = "";

private:

	// visuals
	SDL_Window* m_frame;
	SDL_GLContext m_context;

	// audio
	ALCdevice* m_alcdev;
	ALCcontext* m_alccon;

	// dimensions
	uint32_t w_res,h_res;

	// time & vsync
	std::chrono::steady_clock::time_point past_ticks,curr_ticks = std::chrono::steady_clock::now(),
			last_out = std::chrono::steady_clock::now();
	uint32_t fps = 0,temp_fps = 0;
	double time_delta_nmod = .0;
	double rate_delta = FRAME_REFRATE_DEFAULT_DELTA;
	double stalled_time = .0;
};

inline Frame g_Frame = Frame("stroemungsabriss");

#endif
