#ifndef BRD_TESTAREA
#define BRD_TESTAREA

#include "../../ccb/core.h"

#include "../scenes/nepal_mountain_woods.h"
#include "../scenes/casino_spike.h"
#include "../scenes/casino_table.h"
#include "../pcs/jaeger_jet.h"
#include "../boss/dpilot.h"
#include "../world.h"


// casino bonus environment
struct NevadaCasino
{
	Board b;
	uint8_t process_id;

	// implementation
	static void load(LoadStorage& lds);
	static void update_showroom(Board* b);
	static void update_cardtable(Board* b);
};

// nepal airfield finale (d3)
struct NepalAirfield
{
	Board b;
	uint8_t process_id;
	// FIXME: this id will be invalidated once a baord has been deleted

	// implementation
	static void load(LoadStorage& lds);
	static void update_airfield(Board* b);
	static void update_pilots(Board* b);
};

// test area for visualization and structuring tryouts
struct TestArea
{
	Board b;

	// memory index
	uint16_t sprite_rID,anim_rID;

	// implementation
	static void load(LoadStorage& lds);
	static void update(Board* b);
};
// TODO: remove LoadStorage struct fully, this is an overdefinition

#endif
