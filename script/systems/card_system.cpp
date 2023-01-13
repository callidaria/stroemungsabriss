#include "card_system.h"

/*
	TODO
*/
CardSystem::CardSystem()
{
	// card visualization setup
	const float hwdt = CARDSYSTEM_CARD_WIDTH/2,hhgt = CARDSYSTEM_CARD_HEIGHT/2;
	float cverts[] = {

		// card front
		-hwdt,0,-hhgt,0,0,0, -hwdt,0,hhgt,0,1,0, hwdt,0,hhgt,1,1,0,
		hwdt,0,hhgt,1,1,0, hwdt,0,-hhgt,1,0,0, -hwdt,0,-hhgt,0,0,0,

		// card back
		hwdt,0,-hhgt,0,0,1, hwdt,0,hhgt,0,1,1, -hwdt,0,hhgt,1,1,1,
		-hwdt,0,hhgt,1,1,1, -hwdt,0,-hhgt,1,0,1, hwdt,0,-hhgt,0,0,1,
	};
	bfr.bind();
	bfr.upload_vertices(cverts,sizeof(cverts));
	sdr.compile("./shader/vertex_cards.shader","./shader/fragment_cards.shader");
	sdr.def_attributeF("position",3,0,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.def_attributeF("texCoords",2,3,CARDSYSTEM_UPLOAD_REPEAT);
	sdr.def_attributeF("texID",1,5,CARDSYSTEM_UPLOAD_REPEAT);
	cam3D.view3D = glm::rotate(cam3D.view3D,glm::radians(45.0f),glm::vec3(1,0,0));
	sdr.upload_camera(cam3D);

	// load card game texture
	glGenTextures(1,&tex);
	Toolbox::load_texture(tex,"./res/kopfuber_atlas.png");

	// card instancing: single draw call for all playing cards
	bfr.add_buffer();
	bfr.bind_index();
	sdr.def_indexF(bfr.get_indices(),"tofs",3,0,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"i_tex",2,3,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"rotation_sin",3,5,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"rotation_cos",3,8,CARDSYSTEM_INDEX_REPEAT);
	sdr.def_indexF(bfr.get_indices(),"deckID",1,11,CARDSYSTEM_INDEX_REPEAT);

	// spawn cards into space
	for (uint8_t i=0;i<40;i++) create_card(glm::vec2(i%10,(uint8_t)(i/10)),i>19);
	for (uint8_t i=0;i<36;i++) {
		glm::vec2 tex_id = glm::vec2(i%9,4+(uint8_t)(i/9));
		create_card(tex_id,0);
		create_card(tex_id,1);
	}

	// shuffle deck & place
	create_pile(glm::vec2(2.5f,0));
	shuffle_all();
}

/*
	TODO
*/
void CardSystem::shuffle_all()
{
	// setup allocation list
	std::vector<uint8_t> loose_list;
	for (uint8_t i=0;i<112;i++) loose_list.push_back(i);

	// process all cards
	DeckPile dpile;
	while (loose_list.size()) {

		// pick a random, uninitialized card & put it into pile
		uint8_t rcard = rand()%loose_list.size();
		dpiles[0].cards.push_back(loose_list[rcard]);
		set_position(loose_list[rcard],
				glm::vec3(dpiles[0].pos.x,dpiles[0].cards.size()*.017f,dpiles[0].pos.y));

		// face down card
		set_rotation(loose_list[rcard],glm::vec3(0,0,glm::radians(180.0f)));

		// un-loosen card status
		loose_list.erase(loose_list.begin()+rcard,loose_list.begin()+rcard+1);
	}
}

/*
	TODO
*/
void CardSystem::deal_card(uint8_t pid)
{
	// move card to hand
	uint8_t tmp = dpiles[pid].cards.back();
	create_animation(tmp,glm::vec3(4,7+0.001f*hand.size()+deal.size(),11),
			glm::vec3(glm::radians(-45.0f),0,0),20);
	deal.push_back(tmp);
	dpiles[pid].cards.erase(dpiles[pid].cards.end()-1,dpiles[pid].cards.end());
}

/*
	TODO
*/
void CardSystem::deal_card(uint8_t pid,uint8_t oid)
{
	uint8_t tmp = dpiles[pid].cards.back();
	glm::vec2 pos = ops[oid].position;
	create_animation(tmp,glm::vec3(pos.x,0,pos.y),glm::vec3(0,0,glm::radians(180.0f)),20);
	ops[oid].deal.push_back(tmp);
	dpiles[pid].cards.erase(dpiles[pid].cards.end()-1,dpiles[pid].cards.end());
} // FIXME: pattern replications

/*
	TODO
*/
void CardSystem::hand_to_pile(uint8_t pid,uint8_t idx)
{
	dpiles[pid].cards.push_back(hand[idx]);
	create_animation(hand[idx],
			glm::vec3(dpiles[pid].pos.x,dpiles[pid].cards.size()*.017f,dpiles[pid].pos.y),
			glm::vec3(0),20);
	hand.erase(hand.begin()+idx,hand.begin()+idx+1);
	update_hand_position();
}

/*
	TODO
*/
void CardSystem::create_player(glm::vec2 pos,uint16_t capital)
{ ops.push_back({ pos,{},{},capital }); }

/*
	TODO
*/
void CardSystem::create_pile(glm::vec2 pos)
{ dpiles.push_back({ {},pos }); }

/*
	TODO
*/
void CardSystem::render()
{
	// process deal arrivals
	bool arrival = false;
	uint8_t i = 0;
	while (i<deal.size()) {
		int16_t idx = get_animation_id(deal[i]);
		if (!(idx+1)) {
			hand.push_back(deal[i]);
			deal.erase(deal.begin()+i,deal.begin()+i+1);
			arrival = true;
		} else i++;
	} if (arrival) update_hand_position();

	// process opponent deal arrivals
	uint8_t j = 0;
	while (j<ops.size()) {
		bool upd_opponent = false;
		i = 0;
		while (i<ops[j].deal.size()) {
			int16_t idx = get_animation_id(ops[j].deal[i]);
			if (!(idx+1)) {
				ops[j].cards.push_back(ops[j].deal[i]);
				ops[j].deal.erase(ops[j].deal.begin()+i,ops[j].deal.begin()+i+1);
				upd_opponent = true;
			} else i++;
		} if (upd_opponent) {
			// TODO: update opponent card placement
		} j++;
	}

	// animate
	i = 0;
	while (i<c_anims.size()) {
		CardAnimation anim = c_anims[i];

		// check if animation has been completed
		if (anim.ctime>anim.etime) c_anims.erase(c_anims.begin()+i,c_anims.begin()+i+1);
		else {

			// interpolate position & rotation
			float time_factor = (float)anim.ctime/anim.etime;
			set_position(anim.card_id,anim.s_pos+(anim.t_pos-anim.s_pos)*time_factor);
			set_rotation(anim.card_id,anim.s_rot+(anim.t_rot-anim.s_rot)*time_factor);

			// increment ticks & animation index
			c_anims[i].ctime++;
			i++;
		}
	}

	// building the render queue for correct transparency
	render_queue.clear();
	for (auto deck:dpiles) {
		for (auto card:deck.cards)
			card_to_queue(card);
	} for (auto opp:ops) {
		for (auto card:opp.deal) card_to_queue(card);
		for (auto card:opp.cards) card_to_queue(card);
	} for (auto card:deal) card_to_queue(card); // TODO: FILO instead of FIFO will fix transparency
	for (auto card:hand) card_to_queue(card);

	// gl enable features
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// setup
	sdr.enable();
	bfr.bind();
	bfr.bind_index();
	bfr.upload_indices(render_queue);

	// draw
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawArraysInstanced(GL_TRIANGLES,0,12,112);

	// gl disable features
	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
}
// TODO: OPTIMIZE!

/*
	TODO
*/
void CardSystem::set_position(uint8_t id,glm::vec3 pos)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	icpos[rid] = pos.x;icpos[rid+1] = pos.y;icpos[rid+2] = pos.z;
}

/*
	TODO
*/
void CardSystem::set_rotation(uint8_t id,glm::vec3 rot)
{
	// rasterize id jumps
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	icpos[rid+5] = glm::sin(rot.x);icpos[rid+6] = glm::sin(rot.y);icpos[rid+7] = glm::sin(rot.z);
	icpos[rid+8] = glm::cos(rot.x);icpos[rid+9] = glm::cos(rot.y);icpos[rid+10] = glm::cos(rot.z);
}

/*
	TODO
*/
void CardSystem::reset_rotation(uint8_t id)
{
	// rasterize id jumps
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	icpos[rid+5] = 0;icpos[rid+6] = 0;icpos[rid+7] = 0;
	icpos[rid+8] = 1;icpos[rid+9] = 1;icpos[rid+10] = 1;
}

/*
	TODO
*/
glm::vec3 CardSystem::get_position(uint8_t id)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	return glm::vec3(icpos[rid],icpos[rid+1],icpos[rid+2]);
}

/*
	TODO
*/
glm::vec3 CardSystem::get_rotation(uint8_t id)
{
	// get sine & cosine rotations
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	glm::vec3 srot =
			glm::vec3(glm::asin(icpos[rid+5]),glm::asin(icpos[rid+6]),glm::asin(icpos[rid+7]));
	glm::vec3 crot =
			glm::vec3(glm::acos(icpos[rid+8]),glm::acos(icpos[rid+9]),glm::acos(icpos[rid+10]));

	// join rotation vectors to eliminate negative null rotations masquerading as actual null
	return glm::vec3(srot.x+(crot.x-srot.x)*(glm::abs(srot.x)<0.0001f),
			srot.y+(crot.y-srot.y)*(glm::abs(srot.y)<0.0001f),
			srot.z+(crot.z-srot.z)*(glm::abs(srot.z)<0.0001f));
	// TODO: a little hacky don't you think? maybe try to do this more *elegantly*?!?
	// TODO: hamilton instead of euler?!?!?
}

/*
	TODO
*/
void CardSystem::create_card(glm::vec2 tex_id,bool deck_id)
{
	icpos.push_back(0);icpos.push_back(0);icpos.push_back(0);	// position modification
	icpos.push_back(tex_id.x);icpos.push_back(tex_id.y);		// texture atlas index
	icpos.insert(icpos.end(),{ 0,0,0,1,1,1 });					// rotation sine & cosine
	icpos.push_back(deck_id);									// deck identification
}

/*
	TODO
*/
void CardSystem::update_hand_position()
{
	for (uint8_t i=0;i<hand.size();i++)
		create_animation(hand[i],
				glm::vec3(-5.5f-1.5f*(hand.size()/112.0f)+((float)i/hand.size()*5),7+0.001f*i,11),
				glm::vec3(glm::radians(-45.0f),0,0),20);
}

/*
	TODO
*/
void CardSystem::card_to_queue(uint8_t id)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	for (uint8_t i=0;i<CARDSYSTEM_INDEX_REPEAT;i++) render_queue.push_back(icpos[rid+i]);
}

/*
	TODO
*/
void CardSystem::create_animation(uint8_t id,glm::vec3 pos,glm::vec3 rot,uint16_t etime)
{
	int16_t idx = get_animation_id(id);
	CardAnimation tmp = { id,get_position(id),get_rotation(id),pos,rot,0,etime };
	if (!(idx+1)) c_anims.push_back(tmp);
	else c_anims[idx] = tmp;
}

/*
	TODO
*/
void CardSystem::remove_animation(uint8_t id)
{
	int16_t idx = get_animation_id(id);
	if (idx+1) c_anims.erase(c_anims.begin()+idx,c_anims.begin()+idx+1);
}

/*
	TODO
*/
int16_t CardSystem::get_animation_id(uint8_t id)
{
	int16_t out = -1;
	for (uint8_t i=0;i<c_anims.size();i++) out += (1+i)*(c_anims[i].card_id==id);
	return out;
}