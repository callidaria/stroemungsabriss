#pragma once

#include <iostream>
#include "../cld_lin/frm/frame.h"
#include "../cld_lin/gfx/renderer2d.h"
#include "../cld_lin/mat/camera2d.h"
#include "../cld_lin/frm/framebuffer.h"
#include "../cld_lin/fcn/text.h"

class Menu
{
public:
	Menu(Frame f,Renderer2D* r2d,Camera2D* cam2d);
	void render(Frame f);
private:
	uint32_t svao,svbo;
	Renderer2D* m_r2d;
	Shader sshd;
	FrameBuffer fb,splash_fb;
	Text tft,vtft;
	bool title = false;
	glm::mat4 pos_title = glm::mat4(1.0f);
};
