#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../ccb/mat/camera2d.h"
#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"

class MenuList
{
public:
	MenuList();
	MenuList(Camera2D* cam2d,const char* path);
	~MenuList() {  }
	void add_lines(Camera2D* cam2d,std::vector<const char*> lines);
	void render(float dtrans,float lscroll,uint16_t index);
private:
	std::string breakgrind(std::string nl,uint32_t &i);
	uint8_t textgrind(std::string nl,uint32_t &i);
	uint8_t get_readmode(std::string nl,uint32_t &i);
public:
	uint16_t esize = 0;
private:
	Text ltxt;
	std::vector<Text> dtxt; // ??maybe do this with less SPACE
	int32_t lscroll = 515,dscroll = 600;
};
