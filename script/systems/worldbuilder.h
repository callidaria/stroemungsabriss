#ifndef SCR_SYSTEMS_WORLDBUILDER
#define SCR_SYSTEMS_WORLDBUILDER

#include <iostream>
#include <thread>
#include <queue>

#include "../../ccb/fcn/ccb_manager.h"

#include "../scenes/nepal_mountain_woods.h"
#include "../scenes/casino_spike.h"
#include "../scenes/casino_table.h"

#include "../menu/menu.h"
#include "../menu/main_menu.h"
#include "../ui/action_menu.h"
#include "../struct/feature_base.h"
#include "../pcs/jaeger_jet.h"
#include "../boss/dpilot.h"
#include "../world.h"

class Worldbuilder
{
public:

	// construction
	Worldbuilder(CascabelBaseFeature* ccbf,StageSetup* set_rigs,CCBManager* ccbm,World* world);
	~Worldbuilder() {  }

	// load
	void load();

private:

	// load
	void load_titles();
	void load_menu();
	void load_casino();
	void load_cards();
	void load_airfield();
	void load_dpilot();

	// loading progression feedback
	static void show_load_progression(bool* loading,CascabelBaseFeature* ccbf,float* progress);

private:

	// system
	CascabelBaseFeature* m_ccbf;
	StageSetup* m_setRigs;
	CCBManager* m_ccbm;
	World* m_world;
	bool ldfb_showing = false;
	float progress;
};

#endif