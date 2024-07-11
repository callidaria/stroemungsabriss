#ifndef SCR_SCENES_CASINO_TABLE
#define SCR_SCENES_CASINO_TABLE

#include "../systems/card_system.h"
#include "../world.h"


class CasinoTable : public Scene
{
public:

	// construction
	CasinoTable(float &progress,float pseq);
	~CasinoTable() {  }

	// draw
	virtual void render();

private:

	// cascabel
	CardSystem card_system;
};

#endif
