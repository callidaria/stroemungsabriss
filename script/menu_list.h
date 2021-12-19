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
	int32_t sl_min=0,sl_max=1;	// definition of visibile slider caps
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
	void render(float dtrans,float lscroll,uint16_t index,float &edge_mod,int8_t delta,bool rsl,uint8_t &md);
	void write_tempID(uint8_t index);
private:
	std::string breakgrind(std::string nl,uint32_t &i);
	uint8_t textgrind(std::string nl,uint32_t &i);
	uint8_t get_readmode(std::string nl,uint32_t &i);
	std::vector<std::string> split_arguments(std::string ext,char sep);
	uint32_t translate_index(std::vector<std::string> lev,std::string arg);
public:
	uint16_t esize = 0;
private:
	std::vector<struct LEntity> les;	// list of all extracted list entity elements
	int32_t lscroll = 515,dscroll = 600;	// defines the scrolling values of description and list entity
	bool lf_open = false;			// saves if sublist was open last frame
	uint8_t t_slID;				// temporarily saves the sublist selection, def by user input
};

// FIXME: parameter lscroll & private variable lscroll naming bad practice
