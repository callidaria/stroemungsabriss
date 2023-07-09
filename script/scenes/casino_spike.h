#ifndef SCR_SCENES_CASINO_SPIKE
#define SCR_SCENES_CASINO_SPIKE

#include "../../ccb/gfx/material3d.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

class CasinoSpike : public Scene
{
public:

	// construction
	CasinoSpike(CascabelBaseFeature* ccbf,StageSetup* set_rigs);
	~CasinoSpike() {  }

	// draw
	virtual void render();

private:

	// render information
	CascabelBaseFeature* m_ccbf;
	StageSetup* m_setRigs;
	uint16_t index_r3D;
	glm::vec3 mv_pos = glm::vec3(1,0,-2);

	// camera
	glm::vec3 cp_pos = glm::vec3(0,5,-5),cp_dir = glm::vec3(0,-.05f,.05f);
	float pitch = 0,yaw = 45.0f;

	// test
	float avx = 0;
	uint8_t aac;
};

#endif