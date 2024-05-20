#ifndef SCR_SYSTEMS_WORLDBUILDER
#define SCR_SYSTEMS_WORLDBUILDER

#include <vector>
#include <thread>

#include "../../ccb/core.h"

#include "../scenes/nepal_mountain_woods.h"
#include "../scenes/casino_spike.h"
#include "../scenes/casino_table.h"
#include "../menu/main_menu.h"
#include "../ui/action_menu.h"
#include "../pcs/jaeger_jet.h"
#include "../boss/dpilot.h"
#include "../components/board.h"
#include "../world.h"


class Worldbuilder
{
public:

	// construction
	Worldbuilder(CCBManager* ccbm,World* world);
	~Worldbuilder() {  }

	// load
	void load();

private:

	// loading progression feedback
	static void show_load_progression(LoadStorage* lds);

private:

	// system
	LoadStorage ldstorage;
	// FIXME: load storage should be obsolete with core system
};

#endif
