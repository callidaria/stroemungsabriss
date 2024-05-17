#include "board.h"


/*
	TODO
*/
void NevadaCasino::load(LoadStorage& lds)
{
	NevadaCasino* c = new NevadaCasino();
	c->process_id = lds.world->board_master.size();
	lds.world->board_master.push_back({
			.data = (Board*)c,
			.logic = NevadaCasino::update_showroom
		});

	// scene register
	CasinoSpike* cspike = new CasinoSpike(lds.m_ccbf,lds.progress,.5f);
	lds.world->add_scene(cspike);
	//CasinoTable* ctable = new CasinoTable(lds.m_ccbf,lds.progress,.5f);
	// FIXME: enable second load will override critical setup (e.g. shadow projection)
	// TODO: add scene later, when requested, this preemtively loads
}

/*
	TODO
*/
void NevadaCasino::update_showroom(Board* b)
{
	TestArea* c = (TestArea*)b;
	// TODO
}

/*
	TODO
*/
void NevadaCasino::update_cardtable(Board* b)
{
	TestArea* c = (TestArea*)b;
	// TODO
}


/*
	TODO
*/
void NepalAirfield::load(LoadStorage& lds)
{
	NepalAirfield* c = new NepalAirfield();
	c->process_id = lds.world->board_master.size();
	lds.world->board_master.push_back({
			.data = (Board*)c,
			.logic = NepalAirfield::update_airfield
		});

	// load
	Core::gCamera3D = Camera3D(1280.0f,720.0f);  // TODO: this belongs inside the scene loader
	NepalMountainWoods* nmw = new NepalMountainWoods(lds.m_ccbm,lds.m_ccbf);
	lds.progress += .2f;
	/*JaegerJet* jj = new JaegerJet(m_ccbf);
	progress += .2f;*/
	DPilot* dpilot = new DPilot(lds.m_ccbf);
	lds.progress += .2f;
	lds.world->add_scene(nmw);
	//m_world->add_playable(jj);
	lds.world->add_boss(dpilot);
}

/*
	TODO
*/
void NepalAirfield::update_airfield(Board* b)
{
	NepalAirfield* c = (NepalAirfield*)b;
	// TODO
}

/*
	TODO
*/
void NepalAirfield::update_pilots(Board* b)
{
	NepalAirfield* c = (NepalAirfield*)b;
	// TODO
}


/*
	TODO
*/
void TestArea::load(LoadStorage& lds)
{
	TestArea* c = new TestArea();
	lds.world->board_master.push_back({
			.data = (Board*)c,
			.logic = TestArea::update
		});
	// TODO: generalize, maybe board_logic pointer definition too specific and has to be update-general

	// adding test sprites
	c->sprite_rID = Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");

	// adding test sprite animations
	c->anim_rID = Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/continue_dialogue.png",3,2,6,60);
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/test_bullet_anim.png",3,2,6,30);
}
// FIXME: this loading structure results in a lot of repitition and a bad routine in general

/*
	TODO
*/
void TestArea::update(Board* b)
{
	TestArea* c = (TestArea*)b;

	// prepare and render sprites
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite(c->sprite_rID);
	Core::gRenderer.render_sprite(c->sprite_rID+1);

	// render sprite animations
	Core::gRenderer.render_sprite_animated(c->anim_rID);
	Core::gRenderer.render_sprite_animated(c->anim_rID+11);
}