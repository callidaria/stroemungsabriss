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
	const float thck = .017f;
	for (uint8_t i=0;i<40;i++) {
		icpos.push_back(0);icpos.push_back(thck*i);icpos.push_back(0);		// position modification
		icpos.push_back(i%10);icpos.push_back((uint8_t)(i/10));				// texture atlas index
	}

	// spawn number cards into space
	for (uint8_t i=0;i<36;i++) {

		// card for first deck
		icpos.push_back(0);icpos.push_back(thck*(i*2+40));icpos.push_back(0);
		icpos.push_back(i%9);icpos.push_back(4+(uint8_t)(i/9));

		// card for second deck
		icpos.push_back(0);icpos.push_back(thck*(i*2+41));icpos.push_back(0);
		icpos.push_back(i%9);icpos.push_back(4+(uint8_t)(i/9));
	}
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
	sdr.enable();
	bfr.bind();
	bfr.bind_index();
	bfr.upload_indices(icpos);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,112);
}