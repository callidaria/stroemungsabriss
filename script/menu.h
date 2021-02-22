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
	FrameBuffer fb,splash_fb,title_fb,select_fb;
	Text tft,vtft;

	// start animation values
	const glm::vec3 TITLE_START = glm::vec3(250,100,0);
	const glm::vec3 ENTITLE_START = glm::vec3(340,720,0);
	const glm::vec3 TITLE_MENU = glm::vec3(50,50,0);
	const glm::vec3 ENTITLE_MENU = glm::vec3(340,600,0);
	bool title = false;
	glm::mat4 pos_title,pos_entitle;
	float ptrans = 0;
	glm::vec3 title_dir = TITLE_MENU-TITLE_START;
	glm::vec3 entitle_dir = ENTITLE_MENU-ENTITLE_START;
};
