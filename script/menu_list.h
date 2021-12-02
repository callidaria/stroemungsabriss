#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include "../ccb/mat/camera2d.h"
#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"


//	To save list entity in a less cluttered way
struct LEntity
{	
	Text ltxt,dtxt;			// variables for list entity text & description
	std::vector<Text> lee;		// list of list entity elements in sublist
	std::vector<std::string> lev;	// list of list entity flag values to write to config
	bool slide = false;		// identifier of slideable values in list entity
	uint32_t sl_min,sl_max=1;	// definition of visibile slider caps
	float sl_vmin,sl_vmax;		// definition of slider save caps in file
	uint32_t sID = 0;		// selection ID for active chosen option / slider input
	std::string saveID;		// value, storing the config variable destination
	// TODO: elements for save destination, values & additional parsing
};

class MenuList
{
public:
	MenuList();
	MenuList(Camera2D* cam2d,const char* path);
	~MenuList() {  }
	void save();
	void render(float dtrans,float lscroll,uint16_t index,float &edge_mod);
private:
	std::string breakgrind(std::string nl,uint32_t &i);
	uint8_t textgrind(std::string nl,uint32_t &i);
	uint8_t get_readmode(std::string nl,uint32_t &i);
	std::vector<std::string> split_arguments(std::string ext);
public:
	uint16_t esize = 0;
private:
	std::vector<struct LEntity> les;
	int32_t lscroll = 515,dscroll = 600;
};
