#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <AL/alc.h>

struct Keyboard
{
	bool ka[285] = { false };	// list of keyboard scan results
};

struct Mouse
{
	bool mcl=false,mcr=false;	// mouse button requests
	int32_t mx,my;				// mouse positions x,y
	int32_t mxfr,myfr;			// mouse position x,y scaled to absolute range
	int32_t mw;					// mouse wheel movements
};

struct XBox
{
	int xba[6];					// controller axis tilts
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
	Frame(const char* title="callidaria-program",int8_t screen=0,int16_t width=1280,int16_t height=720,
			SDL_WindowFlags fs=(SDL_WindowFlags)0);
	~Frame() {  }

	// modification
	void clear(float cr=0,float cg=0,float cb=0);
	void update();

	// fps & update ticking
	void print_fps();
	void vsync(uint8_t frames=60);
	void calc_time_delta();

	// controlling
	void input(uint32_t &running,bool tinput=false);

	// termination
	void vanish();

	// text input
	void input_start();
	void input_stop();

	// getter
	float get_time_delta();

private:

	void init();
	void setup(const char* title,GLuint x,GLuint y,int16_t width,int16_t height,
			SDL_WindowFlags fs);
	void get_screen(int8_t screen,SDL_Rect* dim_screen);

public:

	Keyboard kb; Mouse mouse; std::vector<XBox> xb;
	bool event_active = false;
	int w_res, h_res;

	std::vector<SDL_GameController*> m_gc;
	std::string tline = "";

	// input
	SDL_Event m_fe;

private:

	// visuals
	SDL_Window* m_frame;
	SDL_GLContext m_context;

	// audio
	ALCdevice* m_alcdev;
	ALCcontext* m_alccon;

	// time & vsync
	uint16_t past_ticks,current_ticks = 0,fps = 0,temp_fps = 0,lO = 0;
	float time_mod = 1.0f,time_delta = 0;
	uint16_t time_pticks,time_cticks = 0;
};