#ifndef BRD_TESTAREA
#define BRD_TESTAREA

#include "../../ccb/core.h"

#include "../world.h"


// casino bonus environment
struct CasinoSpike
{
	// equivalence to board pointer
	Board d;

	// variables
	uint8_t process_id;

	// implementation
	static void load(LoadStorage& lds);
	static void update_showroom(Board* b);
	static void update_cardtable(Board* b);
};

// test area for visualization and structuring tryouts
struct TestArea
{
	// variables
	Board b;
	uint16_t sprite_rID,anim_rID;

	// implementation
	static void load(LoadStorage& lds);
	static void update(Board* b);
};
// TODO: remove LoadStorage struct fully, this is an overdefinition

#endif
