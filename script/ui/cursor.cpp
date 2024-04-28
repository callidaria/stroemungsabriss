#include "cursor.h"

/*
	constructor()
	purpose: create a cursor object to render a stylish representation of mouse position
*/
Cursor::Cursor()
{
	rindex = Core::gRenderer.add_sprite(glm::vec2(-20),40,40,"./res/menu/bgr_seldom.png");
	Core::gRenderer.add_sprite(glm::vec2(-20),40,40,"./res/menu/seldom.png");
}

/*
	
	render() -> void
	purpose: draw animated cursor at mouse position & interpret position towards 720p
*/
void Cursor::render()
{
	// calculate drawn position
	position = glm::vec2(
			Core::gFrame.mouse.mxfr*MATH_CARTESIAN_XRANGE,
			Core::gFrame.mouse.myfr*MATH_CARTESIAN_YRANGE
		);

	// render cursor counter rotation
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.sprites[rindex].transform.model = glm::mat4(1.f);
	Core::gRenderer.sprites[rindex].transform.rotate(bgr_crot);
	Core::gRenderer.sprites[rindex].transform.translate(position+glm::vec2(-5,10));
	Core::gRenderer.render_sprite(rindex);

	// render cursor front rotation
	Core::gRenderer.sprites[rindex+1].transform.model = glm::mat4(1.f);
	Core::gRenderer.sprites[rindex+1].transform.rotate(front_crot);
	Core::gRenderer.sprites[rindex+1].transform.translate(position);
	Core::gRenderer.render_sprite(rindex+1);

	// update rotations
	bgr_crot -= 7-(bgr_crot<-360)*360;
	front_crot += 2-(front_crot>360)*360;
}