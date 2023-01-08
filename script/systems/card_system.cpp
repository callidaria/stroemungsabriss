#include "card_system.h"

/*
	TODO
*/
CardSystem::CardSystem()
{
	// card visualization setup
	const float hwdt = CARDSYSTEM_CARD_WIDTH/2,hhgt = CARDSYSTEM_CARD_HEIGHT/2;
	float cverts[] = {
		-hwdt,0,-hhgt,0,0, -hwdt,0,hhgt,0,1, hwdt,0,hhgt,1,1,
		hwdt,0,hhgt,1,1, hwdt,0,-hhgt,1,0, -hwdt,0,-hhgt,0,0,
	};
	bfr.bind();
	bfr.upload_vertices(cverts,sizeof(cverts));
	sdr.compile("./shader/vertex_cards.shader","./shader/fragment_cards.shader");
	sdr.def_attributeF("position",3,0,5);
	sdr.def_attributeF("texCoords",2,3,5);
	sdr.upload_camera(cam3D);

	// load card game texture
	glGenTextures(1,&tex);
	Toolbox::load_texture(tex,"./res/kopfuber_atlas.png",-1.5f);

	// card instancing: single draw call for all playing cards
	bfr.add_buffer();
	bfr.bind_index();
	sdr.def_indexF(bfr.get_indices(),"tofs",3,0,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"i_tex",2,3,CARDSYSTEM_INDEX_REPEAT);

	// spawn picture cards into space
	for (uint8_t i=0;i<40;i++) {
		icpos.push_back(0);icpos.push_back(0);icpos.push_back(0);		// position modification
		icpos.push_back(i%10);icpos.push_back((uint8_t)(i/10));				// texture atlas index
	}

	// spawn number cards into space
	for (uint8_t i=0;i<36;i++) {

		// card for first deck
		icpos.push_back(0);icpos.push_back(0);icpos.push_back(0);
		icpos.push_back(i%9);icpos.push_back(4+(uint8_t)(i/9));

		// card for second deck
		icpos.push_back(0);icpos.push_back(0);icpos.push_back(0);
		icpos.push_back(i%9);icpos.push_back(4+(uint8_t)(i/9));
	}

	// shuffle deck & place
	shuffle_all(glm::vec2(0,0));
}

/*
	TODO
*/
void CardSystem::shuffle_all(glm::vec2 pos)
{
	// setup allocation list
	std::vector<uint8_t> loose_list;
	for (uint8_t i=0;i<112;i++) loose_list.push_back(i);

	// process all cards
	DeckPile dpile;
	while (loose_list.size()) {

		// pick a random, uninitialized card & put it into pile
		uint8_t rcard = rand()%loose_list.size();
		dpile.cards.push_back(loose_list[rcard]);

		// put physically atop of pile
		set_position(loose_list[rcard],glm::vec3(pos.x,dpile.cards.size()*.017f,pos.y));

		// un-loosen card status
		loose_list.erase(loose_list.begin()+rcard,loose_list.begin()+rcard+1);
	}

	// reset all cards towards single deck pile
	dpiles.clear();
	dpiles.push_back(dpile);
}

/*
	TODO
*/
void CardSystem::place_card(uint8_t id,glm::vec3 pos)
{ icpos.push_back(pos.x);icpos.push_back(pos.y);icpos.push_back(pos.z); }

/*
	TODO
*/
void CardSystem::deal_card()
{
	// TODO
}

/*
	TODO
*/
void CardSystem::render()
{
	glEnable(GL_DEPTH_TEST);
	sdr.enable();
	bfr.bind();
	bfr.bind_index();
	bfr.upload_indices(icpos);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,112);
	glDisable(GL_DEPTH_TEST);
}

/*
	TODO
*/
void CardSystem::set_position(uint8_t id,glm::vec3 pos)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	icpos[rid] = pos.x;icpos[rid+1] = pos.y;icpos[rid+2] = pos.z;
}