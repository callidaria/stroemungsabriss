#ifndef SCR_BOSS_DANCING_PILOT
#define SCR_BOSS_DANCING_PILOT

#include "../../ccb/mat/toolbox.h"

#include "../base.h"
#include "../world.h"
#include "../ui/healthbar.h"


// bullet memory offset
enum DPilotBMEM
{
	DP_FLARES,
	DP_MINES,
	DP_SPREAD,
	DP_WHIRL
};

class DPilot : public Boss
{
public:

	// construction
	DPilot();
	~DPilot() {  }

	// update
	virtual void update(glm::vec2 pPos);

private:

	// 1st phase
	void flaredrop();
	void mines();
	void directional_sweep(glm::vec2 pPos);
	void whirlpool();

	// switching phase attacks
	void bomb();

public:

	// positioning
	glm::vec2 ePos = glm::vec2(615,600);

private:

	// objects
	Healthbar hbar = Healthbar(glm::vec2(440,690),790,15,{ 3,4 },
			{ 10000,5000,10000,10000,5000,5000,10000 },"The Dancing Pilot");

	// render index
	uint16_t r3d_index,bsys_index;

	// system
	uint8_t iframes = 12;
	int16_t health_mod = 0;

	// animation
	int16_t ph_fcount = 0,spray_counter = 0;
	int8_t dir_mult = 1,mv_stage = 0;
	uint8_t cd_flare = rand()%3+4,cd_direction = (rand()&1)+3,cd_flares = 0,cd_mines = 0;
};

#endif
