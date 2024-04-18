#include "cursor.h"

/*
	constructor(Frame*,Renderer*)
	frame: reference to cascabel frame, used to interpret mouse input
	rnd: reference to renderer, used to render in-program cursor elements
	purpose: create a cursor object to render a stylish representation of mouse position
*/
Cursor::Cursor(Frame* frame,Renderer* rnd)
	: pFrame(frame),pRnd(rnd)
{
	rindex = pRnd->add_sprite(glm::vec2(-20),40,40,"./res/menu/bgr_seldom.png");
	pRnd->add_sprite(glm::vec2(-20),40,40,"./res/menu/seldom.png");
}

/*
	
	render() -> void
	purpose: draw animated cursor at mouse position & interpret position towards 720p
*/
void Cursor::render()
{
	// calculate drawn position
	position = glm::vec2(pFrame->mouse.mxfr*MATH_CARTESIAN_XRANGE,pFrame->mouse.myfr*MATH_CARTESIAN_YRANGE);

	// render cursor counter rotation
	pRnd->prepare_sprites();
	pRnd->sprites[rindex].transform.model = glm::mat4(1.f);
	pRnd->sprites[rindex].transform.rotate(bgr_crot);
	pRnd->sprites[rindex].transform.translate(position+glm::vec2(-5,10));
	pRnd->render_sprite(rindex);

	// render cursor front rotation
	pRnd->sprites[rindex+1].transform.model = glm::mat4(1.f);
	pRnd->sprites[rindex+1].transform.rotate(front_crot);
	pRnd->sprites[rindex+1].transform.translate(position);
	pRnd->render_sprite(rindex+1);

	// update rotations
	bgr_crot -= 7-(bgr_crot<-360)*360;
	front_crot += 2-(front_crot>360)*360;
}