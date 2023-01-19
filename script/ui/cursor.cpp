#include "cursor.h"

/*
	TODO
*/
Cursor::Cursor(Frame* frame,Renderer2D* r2d)
	: pFrame(frame),pR2D(r2d)
{
	rindex = pR2D->add(glm::vec2(-20),40,40,"./res/menu/bgr_seldom.png");
	pR2D->add(glm::vec2(-20),40,40,"./res/menu/seldom.png");
}

/*
	TODO
*/
void Cursor::render()
{
	// calculate drawn position
	glm::vec2 cpos = glm::vec2(pFrame->mouse.mxfr,pFrame->mouse.myfr);

	// render cursor counter rotation
	pR2D->prepare();
	pR2D->sl[rindex].model
			= glm::translate(glm::mat4(1),glm::vec3(cpos.x,cpos.y,0)+glm::vec3(-5,10,0))
			* glm::rotate(glm::mat4(1),glm::radians(bgr_crot),glm::vec3(0,0,1));
	pR2D->render_sprite(rindex,rindex+1);

	// render cursor front rotation
	pR2D->sl[rindex+1].model
			= glm::translate(glm::mat4(1),glm::vec3(cpos.x,cpos.y,0))
			* glm::rotate(glm::mat4(1),glm::radians(front_crot),glm::vec3(0,0,1));
	pR2D->render_sprite(rindex+1,rindex+2);

	// update rotations
	bgr_crot -= 7-(bgr_crot<-360)*360;
	front_crot += 2-(front_crot>360)*360;
}