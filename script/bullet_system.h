#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include "../ccb/gfx/rendereri.h"
#include "player.h"

class BulletSystem
{
public:
	BulletSystem(RendererI* rI);
	~BulletSystem() {  }
	void add_cluster(uint16_t width,uint16_t height,uint32_t caps,const char* tPath);
	void spwn_blt(uint8_t cluster,glm::vec2 nPos);
	void set_bltPos(uint8_t cluster,uint32_t index,glm::vec2 nPos);
	void delta_bltPos(uint8_t cluster,uint32_t index,glm::vec2 dPos);
	glm::vec2 get_bltPos(uint8_t cluster,uint32_t index);
	uint16_t get_bCount(uint8_t cluster);
	bool get_pHit(Player player);
	void render();
private:
	RendererI* m_rI;
	std::vector<bool> fCount;
	std::vector<uint16_t> bCount;
	std::vector<uint32_t> countCaps;
};
