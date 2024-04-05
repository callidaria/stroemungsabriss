#include "playing_cards.h"

/*
	constructor(CascabelBaseFeature*,StageSetup*)
	purpose: create aunt susann's kopfüber playing cards
	\param ccbf: most basic cascabel features
	\param sorg: light origin position, from where the shadow is projected
*/
PlayingCards::PlayingCards(CascabelBaseFeature* ccbf,glm::vec3 sorg)
	: m_ccbf(ccbf),shadow_dir(-glm::normalize(sorg))
{
	// load card vertices
	std::vector<float> overts;
	Toolbox::load_object("./res/card.obj",overts,glm::vec3(0),1,glm::vec3(0));

	// convert card object vertices into the format compatible with card shader
	// memory allocation
	size_t vertsize = overts.size()/PATTERN_OBJECT_LOAD_REPEAT;
	std::vector<float> cverts = std::vector<float>(vertsize*CARD_VERTEX_REPEAT*2);

	// front side
	for (uint32_t i=0;i<vertsize;i++) {
		uint32_t k0 = i*CARD_VERTEX_REPEAT, k1 = i*PATTERN_OBJECT_LOAD_REPEAT;

		// correlate position
		cverts[k0+CARD_POSITION_X] = overts[k1];
		cverts[k0+CARD_POSITION_Y] = overts[k1+1];
		cverts[k0+CARD_POSITION_Z] = overts[k1+2];

		// correlate texture coordinates
		cverts[k0+CARD_TCOORD_X] = overts[k1+3];
		cverts[k0+CARD_TCOORD_Y] = -overts[k1+4];

		// correlate normals
		cverts[k0+CARD_NORMALS_X] = overts[k1+5];
		cverts[k0+CARD_NORMALS_Y] = overts[k1+6];
		cverts[k0+CARD_NORMALS_Z] = overts[k1+7];

		// add frontside identifier
		cverts[k0+CARD_SIDES] = 0;
	}

	// back side
	for (uint32_t i=0;i<vertsize;i++) {
		uint32_t k0 = (i+vertsize)*CARD_VERTEX_REPEAT, k1 = i*PATTERN_OBJECT_LOAD_REPEAT;

		// correlate position
		cverts[k0+CARD_POSITION_X] = -overts[k1];
		cverts[k0+CARD_POSITION_Y] = overts[k1+1];
		cverts[k0+CARD_POSITION_Z] = overts[k1+2];

		// correlate texture coordinates
		cverts[k0+CARD_TCOORD_X] = overts[k1+3];
		cverts[k0+CARD_TCOORD_Y] = overts[k1+4];

		// correlate normals
		cverts[k0+CARD_NORMALS_X] = overts[k1+5];
		cverts[k0+CARD_NORMALS_Y] = overts[k1+6];
		cverts[k0+CARD_NORMALS_Z] = overts[k1+7];

		// add backside identifier
		cverts[k0+CARD_SIDES] = 1;
	}

	// upload card vertices
	bfr.bind();
	bfr.upload_vertices(cverts);
	sdr.compile("./shader/vertex_cards.shader","./shader/fragment_cards.shader");
	sdr.def_attributeF("position",3,CARD_POSITION_X,CARD_VERTEX_REPEAT);
	sdr.def_attributeF("texCoords",2,CARD_TCOORD_X,CARD_VERTEX_REPEAT);
	sdr.def_attributeF("normals",3,CARD_NORMALS_X,CARD_VERTEX_REPEAT);
	sdr.def_attributeF("texID",1,CARD_SIDES,CARD_VERTEX_REPEAT);
	sdr.upload_camera(Core::gCamera3D);

	// load card game texture
	glGenTextures(1,&tex);
	Toolbox::load_texture(tex,"./res/kopfuber_atlas.png",-1.2f,true);
	sdr.upload_int("tex",0);

	// card instancing: single draw call for all playing cards
	bfr.add_buffer();
	bfr.bind_index();
	sdr.def_indexF("tofs",3,ICARD_POSITION_X,CARD_INSTANCE_REPEAT);
	sdr.def_indexF("i_tex",2,ICARD_TEXTURE_X,CARD_INSTANCE_REPEAT);
	sdr.def_indexF("rotation_sin",3,ICARD_SINROT_X,CARD_INSTANCE_REPEAT);
	sdr.def_indexF("rotation_cos",3,ICARD_COSROT_X,CARD_INSTANCE_REPEAT);
	sdr.def_indexF("deckID",1,ICARD_DECK,CARD_INSTANCE_REPEAT);
}
// TODO: compare performance between card object and cutouts with render queue

/*
	render_shadow() -> void (virtual)
	purpose: handling of geometry shadow by 3D renderer
*/
void PlayingCards::render_shadow()
{
	// draw card shadow
	sdr.enable();
	bfr.bind();
	bfr.upload_indices(rqueue);
	sdr.upload_matrix("view",m_ccbf->r3d->shadow_view);
	sdr.upload_matrix("proj",m_ccbf->r3d->shadow_proj);
	sdr.upload_vec3("shadow_dir",shadow_dir);
	glDrawArraysInstanced(GL_TRIANGLES,0,CARD_INSTANCE_REPEAT,CSYS_CARD_COUNT);
	sdr.upload_vec3("shadow_dir",glm::vec3(.0f));
}

/*
	render() -> void
	purpose: draw instanced playing cards
*/
void PlayingCards::render()
{
	// setup cards
	sdr.enable();
	bfr.bind();
	bfr.upload_indices(rqueue);
	sdr.upload_matrix("light_trans",m_ccbf->r3d->scam_projection);
	sdr.upload_camera(Core::gCamera3D);

	// draw cards
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,12,112);
}
