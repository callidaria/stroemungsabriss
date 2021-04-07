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
	void dev_console(bool &running,bool &dactive);
private:
	// background
	Frame* m_frame;
	Renderer2D* m_r2d;
	Camera2D* m_cam2d;
	std::vector<CCBLInterpreter> linpr;
	std::vector<int> index;

	// console control
	bool activeonsc = false,activeonentr=false;
	bool mv = false;
	glm::mat4 cscroll = glm::mat4(1.0f);
	int console_y = 30;

	// edit control
	glm::vec2 mlf;
	int i_lv=0,i_rf=0;

	// console graphics
	Font cf;
	Text ct,cl;
};
