#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "../../ccb/mat/camera2d.h"
#include "../../ccb/gfx/shader.h"

// text related constants
constexpr uint8_t TEXT_MV = 25;							// inwards move offset for name and counter
constexpr int8_t TEXT_DV = -7;							// y-axis dive of name and counter display
constexpr uint8_t TEXT_SIZE = 15;						// text size of name and phase counter
constexpr uint8_t MAX_CHAR_NAME = 128;					// maximum characters for boss naming
constexpr glm::vec4 TEXT_COL = glm::vec4(.5f,0,1,1);	// text render colour

// pixel distances
constexpr uint8_t BORDER_CLEARING = 2;		// pixel distance between border and splicing
constexpr uint8_t SPLICE_ELONGATION = 12;	// elongation distance of splicing after upper edge

// timing
constexpr float POT = 40.0f;			// animation tick maximum for phase upcounting
constexpr uint8_t SPLICE_TICKS = 30;	// animation tick length for splicing animation

// index repeater
constexpr uint8_t PT_REPEAT = 10;	// amount of floats the index pattern takes to repeat
constexpr uint8_t BRD_REPEAT = 10;	// amount of floats the border pattern takes to repeat
constexpr uint8_t SL_REPEAT = 5;	// amount of floats the slice pattern takes to repeat

// physics
constexpr float NBMOMENTUM_RESISTANCE = .25f;	// mellow the nanobar momentum after application

// states of healthbar
enum HBState
{
	FILLING,		// in the process of filling
	SPLICING,		// splice animation between nano bars
	PHASE_COUNT,	// counting up phases
	READY,			// healthbar ready and damagable
	RESET,			// reset healthbar parameters
	CLEAR			// attached boss clear
};

// components for nanobar placement, filling and other information
struct HPBarSwap
{
	std::vector<std::vector<float>> dest_pos;	// all destination positions per combined bar
	std::vector<std::vector<float>> dest_wdt;	// all destination widths per combined bar
	std::vector<float> upload;					// indexing upload data. pattern: (p,w,d,p,w,d,...)
	std::vector<float> upload_splice;			// upload data for healthbar splicers
    int8_t target_itr = 0;						// iteration of target bar modification
	uint8_t hpbar_itr = 0;						// iteration of current healthbar cluster
	Text phname,phcnt;							// visuals for phase name and counter
	glm::vec2 position;							// position of most left nanobar
	uint16_t max_height,max_width;				// dimensions of all frankensteind' nanobars
	uint16_t dmg_threshold = 0;					// counter to precalculate damage to sub later
	uint8_t anim_tick = 0;						// counter for animation ticks
	std::vector<glm::vec2> mntm;				// momentum for all nanobars
};

/*
	NOTE: to myself in about 2-3 years revisiting this code and having the *ultimate* knowledge,
	i'm proud of you, u looking at this and just knowing all the things i did wrong here and
	being naive about. keep going u are doing wonderfully. i wish i could see and ask u right now <3
*/

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

	// animations
	void floating_nanobars();

	// calculators
	static void fill_hpbar(HBState &frdy,HPBarSwap &hpswap);
	static void splice_hpbar(HBState &frdy,HPBarSwap &hpswap);
	static void count_phases(HBState &frdy,HPBarSwap &hpswap);
	static void ready_hpbar(HBState &frdy,HPBarSwap &hpswap);
	static void reset_hpbar(HBState &frdy,HPBarSwap &hpswap);
	static void signal_clear(HBState &frdy,HPBarSwap &hpswap);

private:

	/*
		func(uint8_t&,HPBarSwap&) -> void
			=> func(frdy,hpswap) -> void
		frdy: state counter, represeting the current fill_switch index
		hpswap: struct containing nanobar and upload information
	*/
	std::vector<void(*)(HBState&,HPBarSwap&)> fill_switch = {
		fill_hpbar,splice_hpbar,count_phases,ready_hpbar,reset_hpbar,signal_clear
	};

private:

	// object
	Buffer hpbuffer = Buffer(),brdbuffer = Buffer(),splcbuffer = Buffer();
	Shader shp = Shader(),sborder = Shader(),ssplice = Shader();
	HPBarSwap hpswap;

	// status
	HBState frdy = HBState::FILLING;
};