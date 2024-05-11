#ifndef BRD_TESTAREA
#define BRD_TESTAREA

#include "../../ccb/core.h"
#include "../struct/world_structures.h"


struct TestArea
{
	// variables
	Board b;
	uint16_t sprite_rID,anim_rID;

	// implementation
	static void load(Board* b);
	static void update(Board* b);
};

#endif
