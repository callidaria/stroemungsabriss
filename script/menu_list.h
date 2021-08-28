#pragma once

#include <iostream>
#include <vector>
#include "../ccb/mat/camera2d.h"
#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"

class MenuList
{
public:
	MenuList() {  }
	MenuList(Camera2D* cam2d,const char* path);
	~MenuList();
	void add_lines(Camera2D* cam2d,std::vector<const char*> lines);
	void render(float dtrans,float lscroll);
public:
	uint16_t esize = 0;
private:
	Text ltxt;
	int32_t lscroll = 515;
};
