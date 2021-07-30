#pragma once

#include <iostream>
#include "../ccb/mat/camera2d.h"
#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"

class MenuList
{
public:
	MenuList() {  }
	MenuList(Camera2D* cam2d,const char* path);
	~MenuList();
	void render(float dtrans,float lscroll);
private:
	Text ltxt;
	int lscroll = 515;
};
