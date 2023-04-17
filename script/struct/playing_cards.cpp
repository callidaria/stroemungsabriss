#include "playing_cards.h"

/*
	TODO
*/
PlayingCards::PlayingCards(CascabelBaseFeature* ccbf,StageSetup* set_rigs)
	: m_ccbf(ccbf),m_setRigs(set_rigs)
{
	// card visualization setup
	float cverts[] = {

		// card front
		-CARD_HWIDTH,0,-CARD_HHEIGHT,0,0,0,1,0,0, -CARD_HWIDTH,0,CARD_HHEIGHT,0,1,0,1,0,0,
		CARD_HWIDTH,0,CARD_HHEIGHT,1,1,0,1,0,0, CARD_HWIDTH,0,CARD_HHEIGHT,1,1,0,1,0,0,
		CARD_HWIDTH,0,-CARD_HHEIGHT,1,0,0,1,0,0, -CARD_HWIDTH,0,-CARD_HHEIGHT,0,0,0,1,0,0,

		// card back
		CARD_HWIDTH,0,-CARD_HHEIGHT,0,0,0,1,0,1, CARD_HWIDTH,0,CARD_HHEIGHT,0,1,0,1,0,1,
		-CARD_HWIDTH,0,CARD_HHEIGHT,1,1,0,1,0,1, -CARD_HWIDTH,0,CARD_HHEIGHT,1,1,0,1,0,1,
		-CARD_HWIDTH,0,-CARD_HHEIGHT,1,0,0,1,0,1, CARD_HWIDTH,0,-CARD_HHEIGHT,0,0,0,1,0,1,
	};
	bfr.bind();
	bfr.upload_vertices(cverts,sizeof(cverts));
	sdr.compile("./shader/vertex_cards.shader","./shader/fragment_cards.shader");
	sdr.def_attributeF("position",3,0,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.def_attributeF("texCoords",2,3,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.def_attributeF("normals",3,5,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.def_attributeF("texID",1,8,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.upload_camera(set_rigs->cam3D[3]);

	// load card game texture
	glGenTextures(1,&tex);
	Toolbox::load_texture(tex,"./res/kopfuber_atlas.png",-1.2f);
	sdr.upload_int("tex",0);
	sdr.upload_int("shadow_map",3);

	// card instancing: single draw call for all playing cards
	bfr.add_buffer();
	bfr.bind_index();
	sdr.def_indexF(bfr.get_indices(),"tofs",3,0,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"i_tex",2,3,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"rotation_sin",3,5,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"rotation_cos",3,8,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"deckID",1,11,CARDSYSTEM_INDEX_REPEAT);
}

/*
	TODO
*/
void PlayingCards::render_shadow()
{
	// draw card shadow
	sdr.enable();
	bfr.bind();
	bfr.upload_indices(render_queue);
	sdr.upload_matrix("view",m_ccbf->r3d->shadow_view);
	sdr.upload_matrix("proj",m_ccbf->r3d->shadow_proj);
	//glDrawArraysInstanced(GL_TRIANGLES,0,12,112);
}

/*
	TODO
*/
void PlayingCards::render()
{
	// setup cards
	glEnable(GL_BLEND);
	sdr.enable();
	bfr.bind();
	bfr.upload_indices(render_queue);
	std::string base="al[0].";
	sdr.upload_vec3((base+"pos").c_str(),m_setRigs->lighting.sunlights[0].position);
	sdr.upload_vec3((base+"col").c_str(),m_setRigs->lighting.sunlights[0].colour);
	sdr.upload_float((base+"ins").c_str(),m_setRigs->lighting.sunlights[0].intensity);
	sdr.upload_matrix("light_trans",m_ccbf->r3d->scam_projection);
	sdr.upload_camera(m_setRigs->cam3D[3]);

	// draw cards
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,12,112);
	glDisable(GL_BLEND);
}