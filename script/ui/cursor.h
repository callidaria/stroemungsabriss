#ifndef SCR_INTERFACE_CURSOR
#define SCR_INTERFACE_CURSOR

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/frame.h"
#include "../../ccb/gfx/renderer.h"
#include "../../ccb/mat/camera2d.h"

class Cursor
{
public:

	Cursor(Frame* frame,Renderer* rnd);
	void render();

public:

	glm::vec2 position;

private:

	// cascabel
	Frame* pFrame;
	Renderer* pRnd;

	// draw
	uint16_t rindex;

	// animation
	float bgr_crot = 0,front_crot = 0;
};

#endif