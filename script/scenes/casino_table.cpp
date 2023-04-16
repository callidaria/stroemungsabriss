#include "casino_table.h"

/*
	TODO
*/
CasinoTable::CasinoTable(CascabelBaseFeature* ccbf,StageSetup* set_rigs)
	: m_ccbf(ccbf),m_setRigs(set_rigs)
{
	// setup card system
	std::vector<Currency> curr_path = {
		{ "./res/coin.obj","./res/coin10_tex.png","./res/none.png","./res/dnormal.png",
			"./res/none.png",10 },
		{ "./res/coin.obj","./res/coin_tex.png","./res/none.png","./res/dnormal.png",
			"./res/none.png",5 }
	};
	card_system = CardSystem(ccbf,m_setRigs,curr_path);

	// create card game players
	card_system.create_player(glm::vec2(0,-10),180,100);
	card_system.create_player(glm::vec2(-10,0),-90,100);
	card_system.create_player(glm::vec2(10,0),90,100);

	// create playing field
	card_system.create_pile(glm::vec2(0));
	card_system.create_currency_stack(glm::vec2(4,3),0);
	card_system.create_currency_stack(glm::vec2(1,-3),180);
	card_system.create_currency_stack(glm::vec2(-3,0),90);

	// create starting game logic
	card_system.register_auto_deal(0,0,7);
	card_system.register_auto_deal(0,1,7);
	card_system.register_auto_deal(0,2,7);
	card_system.register_auto_deal(0,3,7);
}

/*
	TODO
*/
void CasinoTable::render(Camera3D &cam3D)
{
	card_system.process_input();
	card_system.update();
	card_system.render();
}