#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/frm/frame.h"
#include "../../ccb/gfx/renderer2d.h"
#include "../../ccb/mat/camera2d.h"

class Cursor
{
public:

	Cursor(Frame* frame,Renderer2D* r2d);
	void render();

private:

	// cascabel
	Frame* pFrame;
	Renderer2D* pR2D;
	Camera2D cam2D = Camera2D(1280.0f,720.0f);

	// draw
	uint16_t rindex;

	// animation
	float bgr_crot = 0,front_crot = 0;
};