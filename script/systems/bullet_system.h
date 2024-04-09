#ifndef SCR_SYSTEMS_BULLET_SYSTEM
#define SCR_SYSTEMS_BULLET_SYSTEM

#include <iostream>
#include <vector>
#include <glm/glm.hpp>

#include "../../ccb/frm/frame.h"
#include "../../ccb/gfx/rendereri.h"

struct BulletCluster {
	uint16_t width,height;
	uint16_t bullet_count;
	uint32_t bullet_cap;
	std::vector<glm::vec2> directions;
	std::vector<int32_t> ticks;
};

class BulletSystem
{
public:

	// construction
	BulletSystem(Frame* frame,RendererI* rI);
	~BulletSystem() {  }

	// creation
	uint16_t add_cluster(uint16_t width,uint16_t height,const uint32_t caps,const char* tPath,
			uint8_t rows,uint8_t cols,uint8_t itn,uint8_t f);
	void spwn_blt(uint8_t cluster,glm::vec2 nPos,glm::vec2 nDir=glm::vec2(0));
	void spwn_blt(uint8_t cluster,glm::vec2 nPos,glm::vec2 nDir,float r);

	// relative movement
	void delta_bltPos(uint8_t cluster,uint32_t index,glm::vec2 dPos);
	void delta_fDir(uint8_t cluster);

	// update
	void inc_tick(uint8_t cluster);
	inline void reset_tick(uint8_t cluster,uint32_t index) { clusters[cluster].ticks[index] = 0; }

	// set
	inline void set_bltPos(uint8_t cluster,uint32_t index,glm::vec2 nPos)
		{ m_rI->set_aOffset(cluster,index,nPos); }
	inline void set_bltDir(uint8_t cluster,uint32_t index,glm::vec2 nDir)
		{ clusters[cluster].directions[index] = nDir; }

	// get
	inline glm::vec2 get_bltPos(uint8_t cluster,uint32_t index)
		{ return m_rI->get_aOffset(cluster,index); }
	inline glm::vec2 get_bltDir(uint8_t cluster,uint32_t index) { return clusters[cluster].directions[index]; }
	inline uint16_t get_bCount(uint8_t cluster) { return clusters[cluster].bullet_count; }
	inline int32_t get_ts(uint8_t cluster,uint32_t index) { return clusters[cluster].ticks[index]; }
	uint8_t get_pHit(uint8_t cluster,glm::vec2 pos,float hr,float br);

	// draw
	void render();

private:

	// cascabel
	Frame* m_frame;								// target frame pointer
	RendererI* m_rI;							// index renderer pointer

	// bullet physics
	std::vector<BulletCluster> clusters;
};

#endif
