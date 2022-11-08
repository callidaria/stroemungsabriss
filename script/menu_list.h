#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/mat/camera2d.h"

#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"

// to identify how to interpret the values
enum ReadMode
{
	INVALID,			// invalid read mode
	HEAD,				// conservative menu list element
	DESCRIPTION,		// menu splash description
	DROPDOWN_ELEMENT,	// selectable menu sublist element in dropdown
	SLIDER,				// slider element inserted
	GDESTINATION,		// destination annotation
	TARGET_MONITOR,		// reading target monitor for menu list insert
	EST_DIFFICULTY,		// estimated difficulty rating
	GROTATION			// globe preview rotation towards coordinates
};

//	to save list entity in a less cluttered way
struct LEntity
{
	Text ltxt,dtxt;					// variables for list entity text & description
	std::vector<Text> lee;			// list of list entity elements in sublist
	std::vector<std::string> lev;	// list of list entity flag values to write to config
	bool slide = false;				// identifier of slideable values in list entity
	int32_t sl_min = 0,sl_max = 1;	// definition of visibile slider caps
	float sl_vmin,sl_vmax;			// definition of slider save caps in file
	uint8_t diff = 0;				// definition of estimated difficulty
	std::vector<int32_t> gRot = { 0,0,0,0,0,0 };	// definition of the globe preview rotation
	uint32_t sID = 0;				// selection ID for active chosen option / slider input
	std::string saveID;				// value, storing the config variable destination
};

class MenuList
{
public:

	// construction
	MenuList();
	MenuList(Renderer2D* r2d,Camera2D* cam2d,const char* path);
	~MenuList() {  }

	// modification
	void reset();
	void save();
	glm::vec2 globe_rotation(uint16_t li);
	void write_tempID(uint8_t index);

	// information
	bool was_changed();

	// draw
	void render(float dtrans,float lscroll,uint16_t index,float &edge_mod,int8_t delta,bool rsl,uint8_t &md);

private:

	std::string breakgrind(std::string nl,uint32_t &i);
	uint8_t textgrind(std::string nl,uint32_t &i);
	uint8_t get_readmode(std::string nl,uint32_t &i);
	std::vector<std::string> split_arguments(std::string ext,char sep);
	uint32_t translate_index(std::vector<std::string> lev,std::string arg);

public:

	uint16_t esize = 0;

private:

	// text
	Font lfnt = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",15,15);

	// cascabel
	Renderer2D* m_r2d;			// pointer to renderer

	// entities
	std::vector<LEntity> les;	// list of all extracted list entity elements
	std::vector<LEntity> rles;	// list to reset and compare to if changes given
	int32_t lscroll = 515,dscroll = 600;	// defines the scrolling values of description and list
	bool lf_open = false;		// saves if sublist was open last frame
	uint8_t t_slID;				// temporarily saves the sublist selection, def by user input
	uint32_t diffRID = 0;		// ID of the difficulty spritesheet in renderer

	// animation of estimated difficulty
	bool neg_scl=false,neg_rot=false;	// true if transformation of estimated difficulty is negative
	float val_scl=1,val_rot=0;			// values of description transformation
};

// FIXME: parameter lscroll & private variable lscroll naming bad practice