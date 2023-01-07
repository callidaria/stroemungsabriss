#include "card_system.h"

/*
	TODO
*/
CardSystem::CardSystem()
{
	// card visualization setup
	const float hwdt = 1.125f,hhgt = 1.75f;
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
	Toolbox::load_texture(tex,"./res/kopfuber_atlas.png");
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
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArrays(GL_TRIANGLES,0,6);
}