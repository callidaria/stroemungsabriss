#include "testing.h"

/*
	TODO
*/
void TestArea::load(Board* b)
{
	TestArea* c = (TestArea*)b;

	// adding test sprites
	c->sprite_rID = Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");

	// adding test sprite animations
	c->anim_rID = Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/continue_dialogue.png",3,2,6,60);
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/test_bullet_anim.png",3,2,6,30);
}

/*
	TODO
*/
void TestArea::update(Board* b)
{
	TestArea* c = (TestArea*)b;

	// prepare and render sprites
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite(c->sprite_rID);
	Core::gRenderer.render_sprite(2);

	// render sprite animations
	Core::gRenderer.render_sprite_animated(c->anim_rID);
	Core::gRenderer.render_sprite_animated(1);
}
