#ifndef SCR_SCENES_CASINO_SPIKE
#define SCR_SCENES_CASINO_SPIKE

#include "../../ccb/core.h"
#include "../../ccb/gfx/cubemap.h"

#include "../world.h"

class CasinoSpike : public Scene
{
public:

	// construction
	CasinoSpike(float &progress,float pseq);
	~CasinoSpike() {  }

	// draw
	virtual void render();

private:

	// render information
	Cubemap irradiance_map = Cubemap("./res/irradiance_map.hdr");
	uint16_t index_r3D,index_p3D;
	uint8_t texture_repeat[6] = { 1,1,10,4,1,1 };

	// physics
	float oheights[4] = { 0 },ospeed[4] = { 0 };

	// camera
	glm::vec3 cp_pos = glm::vec3(0,5,-5),cp_dir = glm::vec3(0,-.05f,.05f);
	float pitch = 0,yaw = 45.0f;
};

#endif
