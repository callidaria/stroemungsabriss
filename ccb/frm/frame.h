#ifndef CCB_FRAME_SELFTITLED
#define CCB_FRAME_SELFTITLED

#include <AL/alc.h>

#include "../mat/toolbox.h"
#include "../fcn/configuration.h"


// input
constexpr uint16_t KEYBOARD_INPUT_RANGE = 285;
constexpr uint8_t MOUSE_BUTTON_INPUT_RANGE = 5;
constexpr uint8_t CONTROLLER_AXIS_INPUT_RANGE = 6;
constexpr uint8_t CONTROLLER_BUTTON_INPUT_RANGE = 22;

// timing
constexpr double FRAME_REFRATE_DEFAULT_DELTA = 1./60.;


struct Keyboard
{
	bool ka[KEYBOARD_INPUT_RANGE] = { false };
};

struct Mouse
{
	bool mb[MOUSE_BUTTON_INPUT_RANGE] = { false };
	int32_t mx,my;
	float mxfr,myfr;
	int32_t mw;
};

struct XBox
{
	int32_t xba[CONTROLLER_AXIS_INPUT_RANGE];
	bool xbb[CONTROLLER_BUTTON_INPUT_RANGE] = { false };
};

class Frame
{
public:

	// construction
	Frame(const char* title);
	/*Frame(const char* title="callidaria-program",int8_t screen=0,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title="callidaria-program",int16_t width=1280,int16_t height=720,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	Frame(const char* title="callidaria-program",int8_t screen=0,int16_t width=1280,
			int16_t height=720,SDL_WindowFlags fs=(SDL_WindowFlags)0);*/
	~Frame() {  }

	// modification
	static void clear(float cr=0,float cg=0,float cb=0);
	inline void update() { SDL_GL_SwapWindow(m_frame); }

	// time
	void calc_time_delta();
	void change_tmod(double goal,double rate);

	// fps
	inline void set_refresh_rate(double refresh_rate=60.0) { rate_delta = 1./refresh_rate; }
	static void gpu_vsync_on();
	static inline void gpu_vsync_off() { SDL_GL_SetSwapInterval(0); }
	void cpu_vsync();
	void print_fps();

	// controlling
	void input(bool &running);

	// termination
	void vanish();

	// text input
	inline void input_start() { SDL_StartTextInput(); }
	inline void input_stop() { SDL_StopTextInput(); }

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
	bool event_active = false;
	double time_mod = 1.,time_delta = 0;
	bool cpref_peripheral = false,mpref_peripheral = false;

	// input
	std::vector<SDL_GameController*> m_gc;
	SDL_Event m_fe;
	Keyboard kb;
	Mouse mouse;
	std::vector<XBox> xb;
	std::string tline = "";
	bool controller_remap = false;

private:

	// visuals
	SDL_Window* m_frame;
	SDL_GLContext m_context;

	// audio
	ALCdevice* m_alcdev;
	ALCcontext* m_alccon;

	// dimensions
	uint32_t w_res,h_res;

	// input
	const uint32_t& axis_deadzone = Configuration::correlate_variable(VKEY_PERIPHERAL_AXIS_DEADZONE);

	// time & vsync
	std::chrono::steady_clock::time_point past_ticks,curr_ticks = std::chrono::steady_clock::now(),
			last_out = std::chrono::steady_clock::now();
	uint32_t fps = 0,temp_fps = 0;
	double time_delta_nmod = .0;
	double rate_delta = FRAME_REFRATE_DEFAULT_DELTA;
	double stalled_time = .0;
};

#endif
