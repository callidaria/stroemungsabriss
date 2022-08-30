#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/mat/camera2d.h"
#include "../../ccb/gfx/shader.h"

struct HPBarSwap
{
	std::vector<std::vector<float>> dest_pos;	// all containing destination position
	std::vector<std::vector<float>> dest_wdt;	// all containing destination widths
    std::vector<float> upload_target;			// modification targets of upload widths (w,w,...)
	std::vector<float> upload;					// indexing upload data. pattern: (p,w,d,p,w,d,...)
    uint8_t target_itr = 0;						// iteration of target bar modification
	uint8_t hpbar_itr = 0;
};

class Healthbar
{
public:

	// construction
	Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<int> phases,
			std::vector<int> hp);
	~Healthbar();

	// draw
	void render();

	// interaction
	void register_damage(uint16_t dmg);

private:

	// calculators
	static void fill_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void ready_hpbar(uint8_t &frdy,HPBarSwap &hpswap);
	static void reset_hpbar(uint8_t &frdy,HPBarSwap &hpswap);

private:

	Buffer hpbuffer = Buffer(),brdbuffer = Buffer(),splcbuffer = Buffer();
	Shader shp = Shader(),sborder = Shader(),ssplice = Shader();
	HPBarSwap hpswap;

	// statii
	uint8_t frdy = 0;
	std::vector<void(*)(uint8_t&,HPBarSwap&)> fill_switch = { fill_hpbar,ready_hpbar,reset_hpbar };

	std::vector<float> ofs;
	uint16_t max_width;
};