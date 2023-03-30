#pragma once

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

class CasinoSpike : public Scene
{
public:

	// construction
	CasinoSpike(CascabelBaseFeature* ccbf);
	~CasinoSpike() {  }

	// draw
	virtual void render();

private:

	// render information
	CascabelBaseFeature* m_ccbf;
	uint16_t index_r3D;
};