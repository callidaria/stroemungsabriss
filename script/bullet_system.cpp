#include "bullet_system.h"

BulletSystem::BulletSystem(RendererI* rI)
	: m_rI(rI) {  }
uint16_t BulletSystem::add_cluster(uint16_t width,uint16_t height,const uint32_t caps,const char* tPath)
{
	m_rI->add(glm::vec2(0,0),width,height,tPath);
	std::vector<glm::vec2> t_dirs(caps);
	std::vector<int32_t> t_ts(caps);

	bCount.push_back(0);
	countCaps.push_back(caps);
	dirs.push_back(t_dirs);
	ts.push_back(t_ts);
	return bCount.size()-1;
}
// FIXME: add hitbox parameters when adding cluster
void BulletSystem::spwn_blt(uint8_t cluster,glm::vec2 nPos,glm::vec2 nDir)
{
	bCount.at(cluster)%=countCaps.at(cluster); // ??how performant is modulo spamming in this context
	m_rI->set_offset(cluster,bCount.at(cluster),nPos);
	set_bltDir(cluster,bCount.at(cluster),nDir);
	reset_tick(cluster,bCount.at(cluster));
	bCount.at(cluster)++;
}
void BulletSystem::set_bltPos(uint8_t cluster,uint32_t index,glm::vec2 nPos)
{
	m_rI->set_offset(cluster,index,nPos);
}
void BulletSystem::set_bltDir(uint8_t cluster,uint32_t index,glm::vec2 nDir) { dirs.at(cluster)[index] = nDir; }
void BulletSystem::delta_bltPos(uint8_t cluster,uint32_t index,glm::vec2 dPos)
{
	// setting frame update time defined delta between bullet positions (e.g. bullet movement)
	m_rI->il.at(cluster).o[index] += dPos;
}
void BulletSystem::delta_bltDir(uint8_t cluster,uint32_t index,glm::vec2 dDir)
{
	dirs.at(cluster)[index] += dDir; // ??is this method stoopid and irrelevant
}
void BulletSystem::delta_fDir(uint8_t cluster)
{
	// FIXME: static update loop counter
	for (int i=0;i<countCaps.at(cluster);i++)
		m_rI->il.at(cluster).o[i] += dirs.at(cluster)[i];
}
void BulletSystem::inc_tick(uint8_t cluster) { for(int i=0;i<bCount.at(cluster);i++)ts.at(cluster).at(i)++; }
void BulletSystem::reset_tick(uint8_t cluster,uint32_t index) { ts.at(cluster).at(index)=0; }
// FIXME: deviation & naming
glm::vec2 BulletSystem::get_bltPos(uint8_t cluster,uint32_t index) { return m_rI->il.at(cluster).o[index]; }
glm::vec2 BulletSystem::get_bltDir(uint8_t cluster,uint32_t index) { return dirs.at(cluster)[index]; }
uint16_t BulletSystem::get_bCount(uint8_t cluster) { return bCount.at(cluster); }
int32_t BulletSystem::get_ts(uint8_t cluster,uint32_t index) { return ts.at(cluster).at(index); }
uint8_t BulletSystem::get_pHit(uint8_t cluster,glm::vec2 pos,uint8_t width,uint8_t height)
{
	uint8_t out = 0;
	for (int i=0;i<countCaps[cluster];i++) {
		glm::vec2 cPos = m_rI->il[cluster].o[i];
		bool hit = cPos.x<pos.x+width&&cPos.x>pos.x&&cPos.y<pos.y+height&&cPos.y>pos.y;
		m_rI->il[cluster].o[i] += glm::vec2(-10000,-10000)*glm::vec2((int)hit);
		out += hit;
	}
	return out;
}
void BulletSystem::render()
{
	m_rI->prepare();
	for (int i=0;i<bCount.size();i++) m_rI->render(i,countCaps.at(i));
	// FIXME: find a solution to the bullets rendering at origin because of capping before setting
}
// TODO: ??maybe add some sort of bullet cleaning system for oos bullets and reschedule (if not too perf. hoggy)
// TODO: add a reset method
// TODO: add projectile rendering rotation
