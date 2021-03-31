#pragma once

#include <iostream>
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
	void dev_console();
private:
	// background
	Frame* m_frame;
	Renderer2D* m_r2d;
	Camera2D* m_cam2d;
	std::vector<CCBLInterpreter> linpr;

	// console control
	bool activeonsc = false;
	bool dev_active = false;

	// console graphics
	Font cf;
	Text ct;
};
