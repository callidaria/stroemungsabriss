#pragma once

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

#include "../systems/card_system.h"

class CasinoTable : public Scene
{
public:

	// construction
	CasinoTable(CascabelBaseFeature* ccbf);
	~CasinoTable() {  }

	// draw
	virtual void render(Camera3D &cam3D);

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	CardSystem card_system;
};