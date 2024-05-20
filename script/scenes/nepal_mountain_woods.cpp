#include "nepal_mountain_woods.h"

/*
	constructor(CCBManager*)
	ccb_manager: cascabel manager to load mountain woods level file
	purpose: load the nepal mountain scene, representing the backdrop for dpilot fight
*/
NepalMountainWoods::NepalMountainWoods(CCBManager* ccb_manager)
{
	index_r2D = ccb_manager->add_lv("lvload/dpilot.ccb");
}

/*
	render() -> void (virtual)
	purpose: draw the loaded scene
*/
void NepalMountainWoods::render()
{
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite(index_r2D);
}