#include "nepal_mountain_woods.h"

/*
	constructor(CCBManager*,CascabelBaseFeature*)
	ccb_manager: cascabel manager to load mountain woods level file
	ccbf: structure holding all most important cascabel tools & features
	purpose: load the nepal mountain scene, representing the backdrop for dpilot fight
*/
NepalMountainWoods::NepalMountainWoods(CCBManager* ccb_manager,CascabelBaseFeature* ccbf)
	: m_ccbf(ccbf)
{ index_r2D = ccb_manager->add_lv("lvload/dpilot.ccb"); }

/*
	render() -> void (virtual)
	purpose: draw the loaded scene
*/
void NepalMountainWoods::render()
{
	m_ccbf->rnd->prepare_sprites();
	m_ccbf->rnd->render_sprite(index_r2D);
}