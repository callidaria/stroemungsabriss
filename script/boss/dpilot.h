#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

#include "../ui/healthbar.h"

#define E 2.7182818284

// bullet memory offset
constexpr uint8_t BMEM_FLARES = 0;
constexpr uint8_t BMEM_MINES = 1;
constexpr uint8_t BMEM_SPREAD = 2;
constexpr uint8_t BMEM_WHIRL = 3;

class DPilot : public Boss
{
public:

	// construction
	DPilot() {  }
	DPilot(CascabelBaseFeature* ccbf);
	~DPilot() {  }

	// update
	virtual void update(glm::vec2 pPos);

private:

	// 1st phase
	void flaredrop();
	void mines();
	void directional_sweep(glm::vec2 pPos);
	void whirlpool();

public:

	// positioning
	glm::vec2 ePos = glm::vec2(615,600);

private:

	// objects
	Healthbar hbar = Healthbar(glm::vec2(440,690),790,15,{ 3,4 },
			{ 10000,5000,10000,10000,5000,5000,10000 },"The Dancing Pilot");

	// render index
	CascabelBaseFeature* m_ccbf;
	uint16_t r3d_index,bsys_index;

	// system
	uint8_t iframes = 12;
	int16_t health_mod = 0;

	// animation
	int16_t ph_fcount = 0,spray_counter = 0;
	int8_t dir_mult = 1,mv_stage = 0;
	uint8_t cd_flare = rand()%3+4,cd_direction = rand()%2+3,cd_flares = 0,cd_mines = 0;
};