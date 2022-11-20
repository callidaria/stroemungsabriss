#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "../../ccb/frm/frame.h"
#include "../../ccb/gfx/rendereri.h"

class BulletSystem
{
public:

	// construction
	BulletSystem(Frame* frame,RendererI* rI);
	~BulletSystem() {  }

	// creation
	uint16_t add_cluster(uint16_t width,uint16_t height,const uint32_t caps,const char* tPath);
	void spwn_blt(uint8_t cluster,glm::vec2 nPos,glm::vec2 nDir=glm::vec2(0));

	// set
	void set_bltPos(uint8_t cluster,uint32_t index,glm::vec2 nPos);
	void set_bltDir(uint8_t cluster,uint32_t index,glm::vec2 nDir);

	// relative movement
	void delta_bltPos(uint8_t cluster,uint32_t index,glm::vec2 dPos);
	void delta_bltDir(uint8_t cluster,uint32_t index,glm::vec2 dDir);
	void delta_fDir(uint8_t cluster);

	// update
	void inc_tick(uint8_t cluster);
	void reset_tick(uint8_t cluster,uint32_t index);

	// get
	glm::vec2 get_bltPos(uint8_t cluster,uint32_t index);
	glm::vec2 get_bltDir(uint8_t cluster,uint32_t index);
	uint16_t get_bCount(uint8_t cluster);
	int32_t get_ts(uint8_t cluster,uint32_t index);
	uint8_t get_pHit(uint8_t cluster,glm::vec2 pos,float hr,float br);

	// draw
	void render();

private:

	// cascabel
	Frame* m_frame;								// target frame pointer
	RendererI* m_rI;							// index renderer pointer

	// bullet physics
	std::vector<uint16_t> c_width;				// widths of each cluster
	std::vector<uint16_t> c_height;				// heights of each cluster
	std::vector<uint16_t> bCount;				// bullet count to render
	std::vector<uint32_t> countCaps;			// maximum bullet capacity loop cutoff
	std::vector<std::vector<glm::vec2>> dirs;	// lists of all bullet directions
	std::vector<std::vector<int32_t>> ts;		// tick counter list
};