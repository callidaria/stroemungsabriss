#include "card_system.h"

/*
	constructor(CascabelBaseFeature*,StageSetup*,std::vector<Currency>)
	ccbf: most basic cascabel features
	set_rigs: scene setup
	curr_path: references a list of all necessary object and texture paths for all currency stages
	purpose: creates background objects, indexes playing cards & precalculates positioning
*/
CardSystem::CardSystem(CascabelBaseFeature* ccbf,StageSetup* set_rigs,
		std::vector<Currency> curr_path)
	: m_ccbf(ccbf),m_setRigs(set_rigs)
{
	// background objects
	r3d_index = m_ccbf->r3d->add("./res/table.obj","./res/table.jpg","./res/none.png",
			"./res/dnormal.png","./res/none.png",glm::vec3(0,-0.001f,0),7,glm::vec3(0,0,0));

	set_rigs->cam3D[3].view3D
		= glm::rotate(set_rigs->cam3D[3].view3D,glm::radians(45.0f),glm::vec3(1,0,0));
	pcards = new PlayingCards(ccbf,set_rigs,set_rigs->lighting.sunlights[0].position);
	ccbf->r3d->register_geometry(pcards);

	// spawn cards into space
	for (uint8_t i=0;i<40;i++) create_card(glm::vec2(i%10,(uint8_t)(i/10)),i>19);
	for (uint8_t i=0;i<36;i++) {
		glm::vec2 tex_id = glm::vec2(i%9,4+(uint8_t)(i/9));
		create_card(tex_id,0);
		create_card(tex_id,1);
	}

	// shuffle deck & place
	create_pile(glm::vec2(0));
	shuffle_all();

	// create payment visualization
	ir3d_index = m_ccbf->r3d->iml.size();
	for (auto cstage:curr_path) {
		m_ccbf->r3d->add(cstage.object,cstage.texture,cstage.specular,cstage.normals,cstage.emission,
				glm::vec3(0),1,glm::vec3(0),CSYS_CURRENCY_CAP,true);
		currency_value.push_back(cstage.value);
		currency_spawn.push_back(0);
		cstack.stacks.push_back({});
	}

	// precalculations
	phead_mat = glm::rotate(glm::mat4(1),glm::radians(90.0f),glm::vec3(0,0,1));
}
// TODO: sort currency by value

/*
	shuffle_all() -> void
	purpose: places all cards, turned over into shuffled deck pile
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
	PARAMETER DEFINITIONS:
	pid: deck pile id, to place onto, take from or interact with in any other way
	oid: opponent id, to reference held cards, positions & facing directions
	cid: currency value id, holding index of value and currency stack lists
	sid: currency stack id, to reference field stack to add money to
*/

/*
	deal_card(uint8_t) -> void
	purpose: deal single card to player from given deck pile
*/
void CardSystem::deal_card(uint8_t pid)
{
	uint8_t tmp = dpiles[pid].cards.back();
	create_animation(tmp,glm::vec3(4,7+0.001f*hand.size()+deal.size(),11),
			glm::vec3(glm::radians(-45.0f),0,0),20);
	deal.push_back(tmp);
	dpiles[pid].cards.pop_back();
}

/*
	deal_card(uint8_t,uint8_t) -> void
	overloads: deal_card . uint8_t -> (uint8_t,uint8_t)
	purpose: deal single card to given opponent from given deck pile
*/
void CardSystem::deal_card(uint8_t pid,uint8_t oid)
{
	// calculate positions for dealt playing cards
	uint8_t tmp = dpiles[pid].cards.back();
	glm::vec3 pos = glm::vec3(ops[oid].position.x,0,ops[oid].position.y);
	glm::vec2 norm_pos = -glm::normalize(glm::vec2(pos.x,pos.z));
	glm::vec4 add_pos = (phead_mat*glm::vec4(norm_pos.x,norm_pos.y,0,1))*glm::vec4(4);

	// animation
	create_animation(tmp,glm::vec3(add_pos.x,.5f,add_pos.y)+pos,
			glm::vec3(0,glm::radians(ops[oid].rotation),glm::radians(180.0f)),20);

	// transaction
	ops[oid].deal.push_back(tmp);
	dpiles[pid].cards.pop_back();
}
// FIXME: pattern replications

/*
	register_auto_deal(uint8_t,uint8_t,uint8_t) -> void
	target: defines which player (npc or pc) will be dealt the given amount of cards:
		either it references opponent id through oid = target-1
		or target = 0 which references the player character
	amount: amount of cards to deal to given player
	purpose: register an auto deal task of a given amount of cards to the given player
*/
void CardSystem::register_auto_deal(uint8_t pid,uint8_t target,uint8_t amount)
{ auto_deals.push_back({ pid,target,amount }); }

/*
	hand_to_pile(uint8_t) -> void
	purpose: move the currently selected card from player's hand to given pile
*/
void CardSystem::hand_to_pile(uint8_t pid)
{
	if (choice<hand.size()) {
		dpiles[pid].cards.push_back(hand[choice]);
		create_animation(hand[choice],
				glm::vec3(dpiles[pid].pos.x,dpiles[pid].cards.size()*.017f,dpiles[pid].pos.y),
				glm::vec3(0),20);
		hand.erase(hand.begin()+choice,hand.begin()+choice+1);
		update_hand_position();
	}
}

/*
	opponent_to_pile(uint8_t,uint8_t,uint8_t) -> void
	idx: index of card held by given opponent
	purpose: move the indexed card from opponent's hand to given pile
*/
void CardSystem::opponent_to_pile(uint8_t oid,uint8_t pid,uint8_t idx)
{
	// animation
	reset_rotation(ops[oid].cards[idx]);
	create_animation(ops[oid].cards[idx],
			glm::vec3(dpiles[pid].pos.x,dpiles[pid].cards.size()*.017f,dpiles[pid].pos.y),
			glm::vec3(0),20);

	// transaction
	dpiles[pid].cards.push_back(ops[oid].cards[idx]);
	ops[oid].cards.erase(ops[oid].cards.begin()+idx,ops[oid].cards.begin()+idx+1);
	update_opponent(oid);
}
// FIXME: holds duplicate code chunk

/*
	PARAMETER DEFINITIONS:
	count: amount of currency with specified value to add to the stack
*/

/*
	add_currency(uint8_t,uint16_t) -> void
	purpose: add currency to the player's money pile
*/
void CardSystem::add_currency(uint8_t cid,uint16_t count)
{
	// move currency representation towards players side
	for (uint16_t i=0;i<count;i++) {
		m_ccbf->r3d->inst_position(ir3d_index+cid,currency_spawn[cid],
				glm::vec3(cstack.position.x+2*cid,.2f*cstack.stacks[cid].size(),cstack.position.y));
		m_ccbf->r3d->inst_rotation(ir3d_index+cid,currency_spawn[cid],
				glm::vec3(0,glm::radians((float)(rand()%360)),0));

		// increment currency spawn & stack counts
		cstack.stacks[cid].push_back(currency_spawn[cid]);
		currency_spawn[cid]++;
	}
}

/*
	add_currency(uint8_t,uint8_t,uint16_t) -> void
	purpose: add currency to the specified opponent's money pile
*/
void CardSystem::add_currency(uint8_t cid,uint8_t oid,uint16_t count)
{
	// move currency representation towards opponents side
	for (uint16_t i=0;i<count;i++) {
		m_ccbf->r3d->inst_position(ir3d_index+cid,currency_spawn[cid],
				glm::vec3(ops[oid].capital.position.x,ops[oid].capital.stacks[cid].size()*.2f,
					ops[oid].capital.position.y)+ops[oid].capital.direction*glm::vec3(cid*2));
		m_ccbf->r3d->inst_rotation(ir3d_index+cid,currency_spawn[cid],
				glm::vec3(0,glm::radians((float)(rand()%360)),0));

		// increment currency spawn & respective opponent stack count
		ops[oid].capital.stacks[cid].push_back(currency_spawn[cid]);
		currency_spawn[cid]++;
	}
}
// FIXME: remove code duplicates

/*
	set_currency(uint8_t,uint8_t,uint16_t) -> void
	purpose: set amount of player's currency to specified pile (not value-based)
*/
void CardSystem::set_currency(uint8_t cid,uint8_t sid,uint16_t count)
{
	// move players currency representation to the selected field stack
	for (uint16_t i=0;i<count;i++) {
		m_ccbf->r3d->inst_position(ir3d_index+cid,cstack.stacks[cid].back(),
				glm::vec3(field_stacks[sid].position.x,field_stacks[sid].stacks[cid].size()*.2f,
					field_stacks[sid].position.y)+field_stacks[sid].direction*glm::vec3(cid*2));

		// update physical coin status beyond visualization
		field_stacks[sid].stacks[cid].push_back(cstack.stacks[cid].back());
		cstack.stacks[cid].pop_back();
	}
}

/*
	set_currency(uint8_t,uint8_t,uint8_t,uint16_t) -> void
	purpose: set amount of specified opponent's currency to specified pile (not value-based)
*/
void CardSystem::set_currency(uint8_t cid,uint8_t oid,uint8_t sid,uint16_t count)
{
	// move opponents currency representation to the selected field stack
	for (uint16_t i=0;i<count;i++) {
		m_ccbf->r3d->inst_position(ir3d_index+cid,ops[oid].capital.stacks[cid].back(),
				glm::vec3(field_stacks[sid].position.x,field_stacks[sid].stacks[cid].size()*.2f,
					field_stacks[sid].position.y)+field_stacks[sid].direction*glm::vec3(cid*2));

		// update physical coin status beyond visualization
		field_stacks[sid].stacks[cid].push_back(ops[oid].capital.stacks[cid].back());
		ops[oid].capital.stacks[cid].pop_back();
	}
}

/*
	create_player(vec2,float,uint16_t) -> void
	pos: position of new player
	rot: direction the new player should face towards as indicated by rotation around y-axis
	capital: lifeline stacks the new player brings to the table
	purpose: create a new non player character involved in the game
*/
void CardSystem::create_player(glm::vec2 pos,float rot,uint16_t capital)
{
	// precalculate stack direction & position
	glm::vec3 dir = glm::rotate(glm::mat4(1),glm::radians(rot),glm::vec3(0,1,0))*glm::vec4(1,0,0,0);
	glm::vec3 cspos = glm::vec3(pos.x,0,pos.y)+dir*glm::vec3(7);

	// add player to opponent list
	ops.push_back({ pos,rot,{},{},{
		glm::vec2(cspos.x,cspos.z),dir,std::vector<std::vector<uint16_t>>(currency_value.size())
	} });

	// auto create piles of capital (i wish)
	for (uint8_t i=0;i<currency_value.size();i++) {

		// calculate how much of the current value has to be added
		uint16_t amount = capital/currency_value[i];
		amount -= !!amount&&(i<currency_value.size()-1);

		// add currency and subtract from goal capital
		add_currency(i,ops.size()-1,amount);
		capital -= currency_value[i]*amount;
	}
}

/*
	create_currency_stack(vec2,float) -> void
	pos: position of new currency stack
	rot: rotation of direction the stacks are lined up towards
	purpose: create a new empty in-field currency stack
*/
void CardSystem::create_currency_stack(glm::vec2 pos,float rot)
{
	glm::vec3 dir = glm::rotate(glm::mat4(1),glm::radians(rot),glm::vec3(0,1,0))*glm::vec4(1,0,0,0);
	field_stacks.push_back({ pos,dir,std::vector<std::vector<uint16_t>>(currency_value.size()) });
}

/*
	process_input() -> void
	purpose: update meta player selections based on input
*/
void CardSystem::process_input()
{
	// keyboard input
	choice += (m_ccbf->frame->kb.ka[SDL_SCANCODE_RIGHT]*(choice<hand.size()-1)
			- m_ccbf->frame->kb.ka[SDL_SCANCODE_LEFT]*(choice>0))*!lfI;
	lfI = m_ccbf->frame->kb.ka[SDL_SCANCODE_RIGHT]||m_ccbf->frame->kb.ka[SDL_SCANCODE_LEFT];
	kinput = lfI;

	// transform from clip space & compare screen spaces with cursor position for choice
	bool mouse_input
			= (m_ccbf->frame->mouse.mxfr!=tmx)&&(m_ccbf->frame->mouse.myfr<.35f)
				&&(m_ccbf->frame->mouse.mxfr<.5f);
	if (hand.size()&&mouse_input) {
		float start = (get_card_screen_space(hand[0]).x+1)/2;
		float end = (get_card_screen_space(hand.back()).x+1)/2;
		float stapled_pos = m_ccbf->frame->mouse.mxfr-start;
		float single_card = (end-start)/(hand.size()-1);
		choice = stapled_pos/single_card;
		bool overselected = choice>=hand.size();
		choice = overselected*(hand.size()-1)+!overselected*choice;
	} tmx = m_ccbf->frame->mouse.mxfr;
}

/*
	update() -> void
	purpose: updates everything visually related to the playing card system:
		it starts with the processing of still dealing cards,
		then the automatic dealing tasks get processed,
		followed by playing card animation interpolations.
		finally, the scene gets rendered for the purpose of shadow map creation
*/
void CardSystem::update()
{
	// process deal arrivals
	bool arrival = false;
	uint8_t i = 0;
	while (i<deal.size()) {
		int16_t idx = get_animation_id(deal[i]);
		if (!(idx+1)) {
			hand.push_back(deal[i]);
			glm::vec3 deal_pos = get_position(deal[i]);
			hand_mod.push_back(deal_pos.y),hand_mod.push_back(deal_pos.z);
			deal.erase(deal.begin()+i,deal.begin()+i+1);
			arrival = true;
		} else i++;
	} if (arrival) update_hand_position();

	// process opponent deal arrivals
	uint8_t j = 0;
	while (j<ops.size()) {
		bool upd_opponent = false;
		i = 0;

		// process status of cards that are being dealt
		while (i<ops[j].deal.size()) {
			int16_t idx = get_animation_id(ops[j].deal[i]);
			if (!(idx+1)) {
				ops[j].cards.push_back(ops[j].deal[i]);
				ops[j].deal.erase(ops[j].deal.begin()+i,ops[j].deal.begin()+i+1);
				upd_opponent = true;
			} else i++;
		} if (upd_opponent) update_opponent(j);
		j++;
	}

	// process automatic deals
	bool stall = (crr_dtime<CARDSYSTEM_DEAL_WAIT||!auto_deals.size());
	crr_dtime *= stall;
	crr_deal *= crr_deal<auto_deals.size();

	// process currently active deal
	if (auto_deals[crr_deal].amount&&!stall) {
		if (auto_deals[crr_deal].target)
			deal_card(auto_deals[crr_deal].source,auto_deals[crr_deal].target-1);
		else deal_card(auto_deals[crr_deal].source);

		// move to next deal and decrease left amount of cards to deal
		auto_deals[crr_deal].amount--;
		crr_deal++;
	}

	// remove if deal is completed & update deal stall time
	else if (!auto_deals[crr_deal].amount&&!stall)
		auto_deals.erase(auto_deals.begin()+crr_deal,auto_deals.begin()+crr_deal+1);
	crr_dtime += m_ccbf->frame->time_delta;

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

	// reset & modify selected cards
	for (uint8_t i=0;i<hand.size();i++) {

		// prepare indices
		uint16_t idx = hand[i]*CARDSYSTEM_INDEX_REPEAT+1;
		uint16_t hidx = i*2;

		// calculate card modification
		hand_mod[hidx] += .5f*(i==choice),hand_mod[hidx+1] -= .5f*(i==choice);

		// realize and reset card modifications
		pcards->rqueue[idx] = hand_mod[hidx],pcards->rqueue[idx+1] = hand_mod[hidx+1];
		hand_mod[hidx] -= .5f*(i==choice),hand_mod[hidx+1] += .5f*(i==choice);
	}

	// building the render queue for correct transparency
	/*pcards->render_queue.clear();
	for (auto deck:dpiles) {								// queue pile cards
		for (auto card:deck.cards) card_to_queue(card);
	} for (auto opp:ops) {									// queue cards held by opponent
		for (auto card:opp.deal) card_to_queue(card);		// first queue cards still arriving
		for (auto card:opp.cards) card_to_queue(card);		// then queue cards held by opponent
	} for (auto card:deal) card_to_queue(card);*/				// TODO: FILO instead of FIFO

	// queue cards held by player
	/*for (uint8_t i=0;i<hand.size();i++) {

		// modify selected card to stand out compared to the others
		uint16_t idx = hand[i]*CARDSYSTEM_INDEX_REPEAT+1;
		float selected_mod = (i==choice)*.5f;
		pcards->rqueue[idx] += selected_mod,pcards->rqueue[idx+1] -= selected_mod;

		// revert modification
		//card_to_queue(hand[i]);
		pcards->rqueue[idx] -= selected_mod,pcards->rqueue[idx+1] += selected_mod;
	}*/
}

/*
	render() -> void
	purpose: card system visuals are drawn
*/
void CardSystem::render()
{
	// render background
	m_ccbf->r3d->prepare(m_setRigs->cam3D[3]);
	m_ccbf->r3d->s3d.upload_float("tex_repeat",10);
	m_ccbf->r3d->render_mesh(r3d_index,r3d_index+1);

	// render currency
	m_ccbf->r3d->prepare_inst(m_setRigs->cam3D[3]);
	for (uint8_t i=0;i<currency_spawn.size();i++) {
		m_ccbf->r3d->iml[ir3d_index+i].inst_count = currency_spawn[i];
		m_ccbf->r3d->render_inst(ir3d_index+i);
	}

	// draw cards
	pcards->render();

	// render cursor
	//cursor.render();
}
// TODO: OPTIMIZE!

/*
	PARAMETER DEFINITION:
	id: value id of card as natural result by creation process
*/

/*
	set_position(uint8_t,vec3) -> void
	pos: instantly new position of indexed playing card
	purpose: instantly set new position of given playing card
*/
void CardSystem::set_position(uint8_t id,glm::vec3 pos)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	pcards->rqueue[rid] = pos.x,pcards->rqueue[rid+1] = pos.y,pcards->rqueue[rid+2] = pos.z;
}

/*
	set_rotation(uint8_t,vec3) -> void
	rot: rotations around x, y & z axis stored in their respective vector values (radians)
	purpose: precalculate sines & consines for euler rotations in vertex shader
*/
void CardSystem::set_rotation(uint8_t id,glm::vec3 rot)
{
	// rasterize id jumps
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	pcards->rqueue[rid+5] = glm::sin(rot.x),pcards->rqueue[rid+6] = glm::sin(rot.y),
	pcards->rqueue[rid+7] = glm::sin(rot.z),
	pcards->rqueue[rid+8] = glm::cos(rot.x),pcards->rqueue[rid+9] = glm::cos(rot.y),
	pcards->rqueue[rid+10] = glm::cos(rot.z);
}
// FIXME: duplicate of mesh instance setting in renderer3d

/*
	reset_rotation(uint8_t) -> void
	purpose: instantly reset rotation of given playing card to default orientation.
		much less expensive to use than set_rotation(id,vec3(0,0,0))
*/
void CardSystem::reset_rotation(uint8_t id)
{
	// rasterize id jumps
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	pcards->rqueue[rid+5] = 0,pcards->rqueue[rid+6] = 0,pcards->rqueue[rid+7] = 0,
	pcards->rqueue[rid+8] = 1,pcards->rqueue[rid+9] = 1,pcards->rqueue[rid+10] = 1;
}

/*
	get_position(uint8_t) -> vec3
	returns: current position of requested playing card
*/
glm::vec3 CardSystem::get_position(uint8_t id)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	return glm::vec3(pcards->rqueue[rid],pcards->rqueue[rid+1],pcards->rqueue[rid+2]);
}

/*
	get_rotation(uint8_t) -> vec3
	purpose: reverse precalculation of sines & cosines for euler rotation in vertex shader
	returns: individual rotations around x, y & z axis (radians)
*/
glm::vec3 CardSystem::get_rotation(uint8_t id)
{
	// get sine & cosine rotations
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	glm::vec3 srot = glm::vec3(glm::asin(pcards->rqueue[rid+5]),glm::asin(pcards->rqueue[rid+6]),
			glm::asin(pcards->rqueue[rid+7]));
	glm::vec3 crot = glm::vec3(glm::acos(pcards->rqueue[rid+8]),glm::acos(pcards->rqueue[rid+9]),
			glm::acos(pcards->rqueue[rid+10]));

	// join rotation vectors to eliminate negative null rotations masquerading as actual null
	return glm::vec3(srot.x+(crot.x-srot.x)*(glm::abs(srot.x)<0.0001f),
			srot.y+(crot.y-srot.y)*(glm::abs(srot.y)<0.0001f),
			srot.z+(crot.z-srot.z)*(glm::abs(srot.z)<0.0001f));
}
// TODO: a little hacky don't you think? maybe try to do this more *elegantly*?!?
// TODO: hamilton instead of euler?!?!?

/*
	create_card(vec2,bool) -> void (private)
	tex_id: vector corresponding to the respective subtexture position on texture atlas
	deck_id: definition if created card uses backside of first or second card game
	purpose: create a shader index upload for a new playing card
*/
void CardSystem::create_card(glm::vec2 tex_id,bool deck_id)
{
	pcards->rqueue.push_back(0);pcards->rqueue.push_back(0);pcards->rqueue.push_back(0);
	pcards->rqueue.push_back(tex_id.x);pcards->rqueue.push_back(tex_id.y);
	pcards->rqueue.insert(pcards->rqueue.end(),{ 0,0,0,1,1,1 });
	pcards->rqueue.push_back(deck_id);
}

/*
	card_to_queue(uint8_t) -> void (private)
	purpose: insert all index upload floats belonging to the given card as next in render queue
*/
/*void CardSystem::card_to_queue(uint8_t id)
{
	uint16_t rid = id*CARDSYSTEM_INDEX_REPEAT;
	for (uint8_t i=0;i<CARDSYSTEM_INDEX_REPEAT;i++) pcards->render_queue.push_back(icpos[rid+i]);
}*/

/*
	get_card_screen_space(uint8_t) -> vec3
	purpose: transform card vertices into screen space to make it smoothly interact with 2D elements
	returns: upper-left card edge, translated into screen space coordinate system
*/
glm::vec3 CardSystem::get_card_screen_space(uint8_t id)
{
	glm::vec3 card_pos = get_position(id)-glm::vec3(CARD_HWIDTH,0,0);
	glm::vec4 clip_space = m_setRigs->cam3D[3].proj3D
			* m_setRigs->cam3D[3].view3D*glm::vec4(card_pos,1);
	return glm::vec3(clip_space)/glm::vec3(clip_space.w);
}

/*
	update_hand_position() -> void (private)
	purpose: update all animations for cards held by player character to build an even fan pattern
*/
void CardSystem::update_hand_position()
{
	// move cards into homogeneous fan structure
	for (uint8_t i=0;i<hand.size();i++)
		create_animation(hand[i],
				glm::vec3(-5.5f-1.5f*(hand.size()/112.0f)+((float)i/hand.size()*5),7+0.001f*i,11),
				glm::vec3(glm::radians(-45.0f),0,0),20);
}

/*
	update_opponent(uint8_t) -> void (private)
	purpose: update all animations for cards held by given opponent to build an even fan pattern
*/
void CardSystem::update_opponent(uint8_t oid)
{
	// precalculate card additions
	glm::vec2 norm_pos = -glm::normalize(ops[oid].position);

	// create insert animation
	for (uint8_t i=0;i<ops[oid].cards.size();i++) {
		glm::vec4 add_dir = phead_mat*glm::vec4(norm_pos.x,norm_pos.y,0,1);
		add_dir *= 4*((i+1)/(float)(ops[oid].cards.size()+1));
		add_dir += glm::vec4(ops[oid].position.x,ops[oid].position.y,0,0);
		create_animation(ops[oid].cards[i],glm::vec3(add_dir.x,0.001f*i,add_dir.y),20);
	}
}

/*
	PARAMETER DEFINITIONS:
	pos: position to move playing card to until animation ends
	rot: rotation to interpolate towards until animation ends
	etime: frames the animation should take to complete playing card movement
*/

/*
	create_animation(uint8_t,vec3,uint16_t) -> void (private)
	purpose: create an animation of given playing card, changing only position in given time
*/
void CardSystem::create_animation(uint8_t id,glm::vec3 pos,uint16_t etime)
{
	int16_t idx = get_animation_id(id);
	glm::vec3 rot = get_rotation(id);
	CardAnimation tmp = { id,get_position(id),rot,pos,rot,0,etime };
	if (!(idx+1)) c_anims.push_back(tmp);
	else c_anims[idx] = tmp;
}

/*
	create_animation(uint8_t,vec3,vec3,uint16_t) -> void (private)
	overloads: create_animation . (uint8_t,vec3,uint16_t) -> (uint8_t,vec3,vec3,uint16_t)
	purpose: create an animation of given playing card, changing both position and rotation
*/
void CardSystem::create_animation(uint8_t id,glm::vec3 pos,glm::vec3 rot,uint16_t etime)
{
	int16_t idx = get_animation_id(id);
	CardAnimation tmp = { id,get_position(id),get_rotation(id),pos,rot,0,etime };
	if (!(idx+1)) c_anims.push_back(tmp);
	else c_anims[idx] = tmp;
}

/*
	force_create_animation(uint8_t,vec3,uint16_t) -> void (private)
	purpose: create new animation without checking duplications, changing only position.
		trade off: less expensive than create_animation counterpart
*/
void CardSystem::force_create_animation(uint8_t id,glm::vec3 pos,uint16_t etime)
{
	glm::vec3 rot = get_rotation(id);
	c_anims.push_back({ id,get_position(id),rot,pos,rot,0,etime });
}
// FIXME: search for possible usages to improve animation creation performance

/*
	force_create_animation(uint8_t,vec3,vec3,uint16_t) -> void (private)
	overloads: force_create_animation . (uint8_t,vec3,uint16_t) -> (uint8_t,vec3,vec3,uint16_t)
	purpose: create new animation without checkint duplications, changing both position & rotation.
		trade off: less expensive than create_animation counterpart
*/
void CardSystem::force_create_animation(uint8_t id,glm::vec3 pos,glm::vec3 rot,uint16_t etime)
{ c_anims.push_back({ id,get_position(id),get_rotation(id),pos,rot,0,etime }); }

/*
	remove_animation(uint8_t) -> void (private)
	purpose: delete current animation of given playing card from animation task list, if exsits
*/
void CardSystem::remove_animation(uint8_t id)
{
	int16_t idx = get_animation_id(id);
	if (idx+1) c_anims.erase(c_anims.begin()+idx,c_anims.begin()+idx+1);
}

/*
	get_animation_id(uint8_t) -> int16_t (private)
	purpose: get not only the animation index of given playing card, but also if an animation exists
	returns: either index of animation list, or -1 if no such animation exists
*/
int16_t CardSystem::get_animation_id(uint8_t id)
{
	int16_t out = -1;
	for (uint8_t i=0;i<c_anims.size();i++) out += (1+i)*(c_anims[i].card_id==id);
	return out;
}
// FIXME: use uint8_t and use 255 (or such) to index non-exsitance