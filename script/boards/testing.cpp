#include "testing.h"

/*
	TODO
*/
void TestArea::load(LoadStorage& lds)
{
	TestArea* c = new TestArea();
	lds.world->board_master.push_back({
			.data = (Board*)c,
			.logic = update
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
