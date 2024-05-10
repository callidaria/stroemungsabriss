#include "testing.h"

/*
	TODO
*/
void TestArea::load()
{
	// adding test sprites
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/bllt_ffdir.png");

	// adding test sprite animations
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/continue_dialogue.png",3,2,6,60);
	Core::gRenderer.add_sprite(glm::vec2(100,100),200,200,"./res/test_bullet_anim.png",3,2,6,30);
}

/*
	TODO
*/
void TestArea::update(Board& b)
{
	// prepare and render sprites
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite(1);
	Core::gRenderer.render_sprite(2);

	// render sprite animations
	Core::gRenderer.render_sprite_animated(0);
	Core::gRenderer.render_sprite_animated(1);
}
