#ifndef SCR_SCENES_NEPAL_MOUNTAIN_WOODS
#define SCR_SCENES_NEPAL_MOUNTAIN_WOODS

#include <iostream>

#include "../../ccb/fcn/ccb_manager.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

class NepalMountainWoods : public Scene
{
public:

	// construction
	NepalMountainWoods(CCBManager* ccb_manager,CascabelBaseFeature* ccbf);
	~NepalMountainWoods() {  }

	// draw
	virtual void render();

private:

	// render information
	CascabelBaseFeature* m_ccbf;
	uint16_t index_r2D;
};

#endif