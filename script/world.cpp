#include "world.h"

/*
	constructor(CascabelBaseFeature*,StageSetup*)
	eref: references to all relevant engine objects
	purpose: create a world handling entity for a better loop structure
*/
World::World(CascabelBaseFeature* eref)
	: m_ccbf(eref)
{
	// memfail avoidance shadow setup
	eref->r3d->create_shadow(glm::vec3(0),glm::vec3(3),1,1,1,1);
	// FIXME: if r3d would be a good renderer this would be unnecessary!!

	// framebuffer setup
	rtarget_id = eref->r3d->add_target();
}

/*
	load(float&,float) -> void !O(1)
	purpose: load geometry of added objects & upload static lighting & maps
	\param progress: load progress to be changed while loading
	\param ldsplit: partition of load progress influenced by load progression
*/
void World::load(float& progress,float ldsplit)
{
	float org_progress = progress,pr_progress = ldsplit/4.0f;
	Core::gRenderer.load();
	m_ccbf->rI->load(progress,pr_progress);
	m_ccbf->r3d->load(Core::gCamera3D,progress,pr_progress);
	m_ccbf->r3d->upload_target_static_lighting(rtarget_id,&Core::gLighting);
	m_ccbf->pSys->load();
	progress = org_progress+ldsplit;
}

/*
	render(uint32_t&,bool&) -> void
	running: runstate of the program, used to store progression & if program is still running
	reboot: holds if program should reboot after it has been closed
	purpose: render & handle world entites
*/
void World::render(bool& running,bool& reboot)
{
	// 3D
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// update
	m_ccbf->r3d->update_animations(Core::gFrame.time_delta);
	m_ccbf->r3d->update_shadows();

	// start geometry pass deferred scene
	m_ccbf->r3d->start_target(rtarget_id);

	// handle environments, bosses & player
	for (auto scene : scene_master) scene->render();
	for (auto boss : boss_master) boss->update(glm::vec2(100));
	for (auto player : player_master) player->update();

	// TRANSPARENCY
	m_ccbf->r3d->switch_target_transparency(rtarget_id);

	// render particles
	m_ccbf->pSys->update(Core::gCamera3D,Core::gFrame.time_delta);
	m_ccbf->pSys->auto_render(Core::gCamera3D);

	// run deferred shading
	glDisable(GL_DEPTH_TEST);
	game_fb.bind();
	m_ccbf->r3d->render_target(rtarget_id,Core::gCamera3D,&Core::gLighting);

	// render bullets
	m_ccbf->bSys->render();

	// logic test
	for (BoardTuple board : board_master) board.logic(board.data);

	FrameBuffer::close();

	// render ui
	ui_master[active_daui]->render(&game_fb,running,reboot);
}
