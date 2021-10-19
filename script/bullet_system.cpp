#include "bullet_system.h"

BulletSystem::BulletSystem(RendererI* rI)
	: m_rI(rI) {  }
void BulletSystem::add_cluster(uint16_t width,uint16_t height,const uint32_t caps,const char* tPath)
{
	m_rI->add(glm::vec2(0,0),width,height,tPath);
	std::vector<glm::vec2> t_dirs(caps);
	bCount.push_back(0);countCaps.push_back(caps);dirs.push_back(t_dirs);
}
// FIXME: add hitbox parameters when adding cluster
void BulletSystem::spwn_blt(uint8_t cluster,glm::vec2 nPos,glm::vec2 nDir)
{
	bCount.at(cluster)%=countCaps.at(cluster); // ??how performant is modulo spamming in this context
	m_rI->set_offset(cluster,bCount.at(cluster),nPos);
	set_bltDir(cluster,bCount.at(cluster),nDir);
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
	dirs.at(cluster)[index] += dDir; // ??is this method stoopid, irrelevant and has to die
}
void BulletSystem::delta_fDir(uint8_t cluster)
{
	// FIXME: static update loop counter
	for (int i=0;i<countCaps.at(cluster);i++)
		m_rI->il.at(cluster).o[i] += dirs.at(cluster)[i];
}
// FIXME: deviation & naming
glm::vec2 BulletSystem::get_bltPos(uint8_t cluster,uint32_t index) { return m_rI->il.at(cluster).o[index]; }
glm::vec2 BulletSystem::get_bltDir(uint8_t cluster,uint32_t index) { return dirs.at(cluster)[index]; }
uint16_t BulletSystem::get_bCount(uint8_t cluster) { return bCount.at(cluster); }
bool BulletSystem::get_pHit(Player player)
{
	// TODO: check if pHitbox intersects with framestopped bullet clusters
	return false;
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
