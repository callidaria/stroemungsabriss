#ifndef SCR_SCENES_CASINO_TABLE
#define SCR_SCENES_CASINO_TABLE

#include "../struct/feature_base.h"
#include "../systems/card_system.h"
#include "../world.h"


class CasinoTable : public Scene
{
public:

	// construction
	CasinoTable(CascabelBaseFeature* ccbf,float &progress,float pseq);
	~CasinoTable() {  }

	// draw
	virtual void render();

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	CardSystem card_system;
};

#endif
