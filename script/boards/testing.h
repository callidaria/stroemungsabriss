#ifndef BRD_TESTAREA
#define BRD_TESTAREA

#include "../../ccb/core.h"

#include "../world.h"


struct TestArea
{
	// variables
	Board b;
	uint16_t sprite_rID,anim_rID;

	// implementation
	static void load(LoadStorage& lds);
	static void update(Board* b);
};

#endif
