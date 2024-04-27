#ifndef SCR_SYSTEMS_WORLDBUILDER
#define SCR_SYSTEMS_WORLDBUILDER

#include <iostream>
#include <thread>
#include <queue>

#include "../../ccb/fcn/ccb_manager.h"

#include "../scenes/nepal_mountain_woods.h"
#include "../scenes/casino_spike.h"
#include "../scenes/casino_table.h"

#include "../menu/main_menu.h"
#include "../ui/action_menu.h"
#include "../struct/feature_base.h"
#include "../pcs/jaeger_jet.h"
#include "../boss/dpilot.h"
#include "../world.h"

struct LoadStorage
{
	CascabelBaseFeature* m_ccbf;
	CCBManager* m_ccbm;
	World* world;
	bool ldfb_showing = false;
	float progress = .0;
};

class Worldbuilder
{
public:

	// construction
	Worldbuilder(CascabelBaseFeature* ccbf,CCBManager* ccbm,World* world);
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
