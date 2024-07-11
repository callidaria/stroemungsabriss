#include "casino_table.h"

/*
	constructor(StageSetup*)
	purpose: load & setup table scene
*/
CasinoTable::CasinoTable(float &progress,float pseq)
{
	// lighting
	float sseq = pseq/5.0f;
	Core::gR3D.create_shadow(glm::vec3(100,150,150),glm::vec3(0),100,100,10,4096);
	Core::gLighting.add_sunlight({ glm::vec3(100,150,150),glm::vec3(1),.7f });
	progress += sseq;

	// setup card system
	std::vector<Currency> curr_path = {
		{ "./res/coin.obj","./res/coin10_tex.png","./res/none.png","./res/dnormal.png",
			"./res/none.png",10 },
		{ "./res/coin.obj","./res/coin_tex.png","./res/none.png","./res/dnormal.png",
			"./res/none.png",5 }
	};
	card_system = CardSystem(curr_path);
	progress += sseq;

	// create card game players
	card_system.create_player(glm::vec2(0,-10),180,100);
	card_system.create_player(glm::vec2(-10,0),-90,100);
	card_system.create_player(glm::vec2(10,0),90,100);
	progress += sseq;

	// create playing field
	card_system.create_currency_stack(glm::vec2(4,3),0);
	card_system.create_currency_stack(glm::vec2(1,-3),180);
	card_system.create_currency_stack(glm::vec2(-3,0),90);
	progress += sseq;

	// create starting game logic
	card_system.register_auto_deal(0,0,7);
	card_system.register_auto_deal(0,1,7);
	card_system.register_auto_deal(0,2,7);
	card_system.register_auto_deal(0,3,7);
	progress += sseq;
}

/*
	render() -> void (virtual)
	purpose: draw casino table test scene
*/
void CasinoTable::render()
{
	card_system.process_input();
	card_system.update();
	card_system.render();
}