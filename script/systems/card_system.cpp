#include "card_system.h"

/*
	TODO
*/
CardSystem::CardSystem()
{
	// card visualization setup
	const float hwdt = CARDSYSTEM_CARD_WIDTH/2,hhgt = CARDSYSTEM_CARD_HEIGHT/2;
	float cverts[] = {
		-hwdt,0,-hhgt,0,0,-hwdt,0,hhgt,0,1,hwdt,0,hhgt,1,1,
		hwdt,0,hhgt,1,1,hwdt,0,-hhgt,1,0,-hwdt,0,-hhgt,0,0,
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
	sdr.def_indexF(bfr.get_indices(),"tofs",3,0,3);
	for (int8_t i=-10;i<10;i++) {
		icpos.push_back(CARDSYSTEM_CARD_WIDTH*i);
		icpos.push_back(0);
		icpos.push_back(0);
	}
}

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
	glDrawArraysInstanced(GL_TRIANGLES,0,6,20);
}