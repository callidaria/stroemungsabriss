#include "nepal_mountain_woods.h"

/*
	TODO
*/
NepalMountainWoods::NepalMountainWoods(CCBManager* ccb_manager,CascabelBaseFeature* ccbf)
	: m_ccbf(ccbf)
{ index_r2D = ccb_manager->add_lv("lvload/dpilot.ccb"); }

/*
	TODO
*/
void NepalMountainWoods::render()
{
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_sprite(index_r2D,index_r2D+1);
}