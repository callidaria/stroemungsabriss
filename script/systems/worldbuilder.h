#pragma once

#include <iostream>
#include <queue>

#include "../../ccb/fcn/ccb_manager.h"

#include "../menu/menu.h"
#include "../ui/action_menu.h"
#include "../struct/feature_base.h"
#include "../scenes/nepal_mountain_woods.h"
#include "../pcs/jaeger_jet.h"
#include "../boss/dpilot.h"
#include "../world.h"

class Worldbuilder
{
public:

	// construction
	Worldbuilder(CascabelBaseFeature* ccbf,CCBManager* ccbm,World* world);
	~Worldbuilder() {  }

	// load
	void load();

private:

	// load
	void load_titles();
	void load_menu();
	void load_airfield();
	void load_dpilot();

private:

	// system
	CascabelBaseFeature* m_ccbf;
	CCBManager* m_ccbm;
	World* m_world;
};