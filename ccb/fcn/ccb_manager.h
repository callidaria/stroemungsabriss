#ifndef CCB_FEATURE_CCBMANAGER
#define CCB_FEATURE_CCBMANAGER

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

	// construction
	CCBManager(Frame* frame,Renderer2D* r2d,Camera2D* cam2d);

	// load & unload
	uint16_t add_lv(const char* path);
	void rm_lv(uint16_t id);
	void vanish();

	// functionality
	void dev_console(bool &running,bool &dactive);

private:

	// background
	Frame* m_frame;
	Renderer2D* m_r2d;
	Camera2D* m_cam2d;
	std::vector<CCBLInterpreter> linpr;
	std::vector<int> index;

	// console control
	bool activeonsc=false,activeonentr=false,activeonmcl=false;
	bool mv=false,scl=false;
	glm::mat4 cscroll = glm::mat4(1.0f);
	int console_y = 30;

	// edit control
	glm::vec2 mlf;
	int i_lv=0,i_rf=0;
	float deltascl = 1;
	float tmp_wscale=0,tmp_hscale=0;

	glm::mat4 tmp_model = glm::mat4(1.0f);

	// console graphics
	Font cf = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",20,20);
	Text ct = Text(cf),cl = Text(cf);
};

#endif