#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "../frm/frame.h"
#include "../gfx/renderer2d.h"
#include "../mat/camera2d.h"
#include "ccb_linpr.h"
#include "text.h"
#include "font.h"

class CCBManager
{
public:
	CCBManager(Frame* frame,Renderer2D* r2d,Camera2D* cam2d);
	int add_lv(const char* path,Text* txt=nullptr);
	void dev_console(bool &dactive);
private:
	// background
	Frame* m_frame;
	Renderer2D* m_r2d;
	Camera2D* m_cam2d;
	std::vector<CCBLInterpreter> linpr;

	// console control
	bool activeonsc = false,activeonentr=false;
	//std::string iline;
	glm::mat4 cscroll = glm::mat4(1.0f);
	int console_y = 30;

	// console graphics
	Font cf;
	Text ct,cl;
};
