#pragma once

#include "../../ccb/gfx/cubemap.h"

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
	Cubemap irradiance_map = Cubemap("./res/irradiance_map.hdr");
	uint16_t index_r3D,index_p3D;
	std::vector<uint8_t> texture_repeat;

	// camera
	glm::vec3 cp_pos = glm::vec3(0,5,-5),cp_dir = glm::vec3(0,-.05f,.05f);
	float pitch = 0,yaw = 45.0f;
};