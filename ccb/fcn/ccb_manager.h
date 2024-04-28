#ifndef CCB_FEATURE_CCBMANAGER
#define CCB_FEATURE_CCBMANAGER

#include <iostream>
#include <string>
#include <vector>
#include "../core.h"
#include "ccb_linpr.h"
#include "text.h"
#include "font.h"

class CCBManager
{

public:

	// construction
	CCBManager();

	// load & unload
	uint16_t add_lv(const char* path);

	// functionality
	void dev_console(bool &running,bool &dactive);

private:

	// background
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
