#ifndef SCR_SCENES_CASINO_TABLE
#define SCR_SCENES_CASINO_TABLE

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

#include "../systems/card_system.h"

class CasinoTable : public Scene
{
public:

	// construction
	CasinoTable(CascabelBaseFeature* ccbf,StageSetup* set_rigs);
	~CasinoTable() {  }

	// draw
	virtual void render();

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	StageSetup* m_setRigs;
	CardSystem card_system;
};

#endif