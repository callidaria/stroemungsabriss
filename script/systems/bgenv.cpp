#include "bgenv.h"

void BGEnv::load(uint32_t rstate,CCBManager* ccbm)
{
	std::string path = "lvload/"+lvpathlib.at(rstate)+".ccb";
	idx2d = ccbm->add_lv(path.c_str());
}
void BGEnv::update(uint32_t &rstate) { env_slib.at(rstate)(m_r2d,idx2d); }
void BGEnv::env_lchild(Renderer2D* r2d,uint32_t &m_idx2d)
{
	r2d->prepare();
	r2d->render_sprite(m_idx2d,m_idx2d+1);
}
void BGEnv::env_dpilot(Renderer2D* r2d,uint32_t &m_idx2d)
{
	r2d->prepare();
	r2d->render_sprite(m_idx2d,m_idx2d+1);
}
