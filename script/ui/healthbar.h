#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/mat/camera2d.h"
#include "../../ccb/gfx/shader.h"

#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

// text related constants
constexpr uint8_t TEXT_MV = 25;							// inwards move offset for name and counter
constexpr int8_t TEXT_DV = -7;							// y-axis dive of name and counter display
constexpr uint8_t TEXT_SIZE = 20;						// text size of name and phase counter
constexpr uint8_t MAX_CHAR_NAME = 128;					// maximum characters for boss naming
constexpr glm::vec4 TEXT_COL = glm::vec4(.5f,0,1,1);	// text render colour

// other constants
constexpr float POT = 40.0f;		// animation tick maximum for phase upcounting
constexpr uint8_t PT_REPEAT = 5;	// amount of floats the index pattern takes to repeat

struct HPBarSwap
{
	std::vector<std::vector<float>> dest_pos;	// all containing destination position
	std::vector<std::vector<float>> dest_wdt;	// all containing destination widths
    std::vector<float> upload_target;			// modification targets of upload widths (w,w,...)
	std::vector<float> edge_target;				// raised edge values the upper right vertex reaches
	std::vector<float> upload;					// indexing upload data. pattern: (p,w,d,p,w,d,...)
	std::vector<float> upload_splice;			// upload data for healthbar splicers
    int8_t target_itr = 0;						// iteration of target bar modification
	uint8_t hpbar_itr = 0;						// iteration of current healthbar cluster
	Text phname,phcnt;							// visuals for phase name and counter
	glm::vec2 position;							// position of most left nanobar
	uint16_t max_width;							// total width of all nanobars combined
	uint16_t dmg_threshold = 0;					// counter to precalculate damage to sub later
	uint8_t anim_tick = 0;						// counter for animation ticks
};

class Healthbar
{
public:

	// construction
	Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<int> phases,
			std::vector<int> hp,const char* boss_name);
	~Healthbar();

	// draw
	void render();

	// interaction
	void register_damage(uint16_t dmg);

private:

	// calculators
	static void fill_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void splice_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void count_phases(uint8_t &frdy,HPBarSwap &hpswap);
	static void ready_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void reset_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void signal_clear(uint8_t &frdy,HPBarSwap &hpswap);

private:
	/*
		func(uint8_t&,HPBarSwap&) -> void
			=> func(frdy,hpswap) -> void
		frdy: state counter, represeting the current fill_switch index
		hpswap: struct containing nanobar and upload information
	*/
	std::vector<void(*)(uint8_t&,HPBarSwap&)> fill_switch = {
		fill_hpbar,splice_hpbar,count_phases,ready_hpbar,reset_hpbar,signal_clear
	};

private:

	// object
	Buffer hpbuffer = Buffer(),brdbuffer = Buffer(),splcbuffer = Buffer();
	Shader shp = Shader(),sborder = Shader(),ssplice = Shader();
	HPBarSwap hpswap;

	// status
	uint8_t frdy = 0;
};