#include "testing.h"

/*
	TODO
*/
void TestArea::load()
{
	// adding test sprites
	gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");
	gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");

	// adding test sprite animations
	gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/continue_dialogue.png",3,2,6,60);
	gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/test_bullet_anim.png",3,2,6,30);
}

/*
	TODO
*/
void TestArea::update(Board& b)
{
	// prepare and render sprites
	gRenderer.prepare_sprites();
	gRenderer.render_sprite(1);
	gRenderer.render_sprite(2);

	// render sprite animations
	gRenderer.render_animated(0);
	gRenderer.render_animated(1);
}
