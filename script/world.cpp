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

	// framebuffer setup
	game_fb = FrameBuffer(eref->frame->w_res,eref->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_mainmenu.shader",false);
	rtarget_id = eref->r3d->add_target(eref->frame);
}

/*
	free_memory() -> void
	purpose: clear memory of all dynamically created objects that still exist
*/
void World::free_memory()
{
	for (auto ui : ui_master) delete ui;
	for (auto scene : scene_master) delete scene;
	for (auto player : player_master) delete player;
	for (auto boss : boss_master) delete boss;
}

/*
	remove_<element>(uint8_t) -> void
	argument[0]: list index of element to remove from world
	purpose: remove an element from the world by it's list id
*/
void World::remove_ui(uint8_t ui_id)
{
	delete ui_master[ui_id];
	ui_master.erase(ui_master.begin()+ui_id);
}
void World::remove_scene(uint8_t scene_id)
{
	delete scene_master[scene_id];
	scene_master.erase(scene_master.begin()+scene_id);
}
void World::remove_playable(uint8_t player_id)
{
	delete player_master[player_id];
	player_master.erase(player_master.begin()+player_id);
}
void World::remove_boss(uint8_t boss_id)
{
	delete boss_master[boss_id];
	boss_master.erase(boss_master.begin()+boss_id);
}

/*
	load(float&,float) -> void !O(1)
	purpose: load geometry of added objects & upload static lighting & maps
	\param progress: load progress to be changed while loading
	\param ldsplit: partition of load progress influenced by load progression
*/
void World::load(float &progress,float ldsplit)
{
	float org_progress = progress,pr_progress = ldsplit/4.0f;
	m_ccbf->r2d->load(progress,pr_progress);
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
void World::render(bool &running,bool &reboot)
{
	// 3D
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_BLEND);

	// update
	m_ccbf->r3d->update_animations(m_ccbf->frame->time_delta);
	m_ccbf->r3d->update_shadows(m_ccbf->frame->w_res,m_ccbf->frame->h_res);

	// start geometry pass deferred scene
	m_ccbf->r3d->start_target(rtarget_id);

	// handle environments, bosses & player
	for (auto scene : scene_master) scene->render();
	for (auto boss : boss_master) boss->update(glm::vec2(100));
	for (auto player : player_master) player->update();

	// TRANSPARENCY
	m_ccbf->r3d->switch_target_transparency(rtarget_id);

	// render particles
	m_ccbf->pSys->update(Core::gCamera3D,m_ccbf->frame->time_delta);
	m_ccbf->pSys->auto_render(Core::gCamera3D);

	// run deferred shading
	glDisable(GL_DEPTH_TEST);
	game_fb.bind();
	m_ccbf->r3d->render_target(rtarget_id,Core::gCamera3D,&Core::gLighting);

	// render bullets
	m_ccbf->bSys->render();
	game_fb.close();

	// render ui
	ui_master[active_daui]->render(&game_fb,running,reboot);
}
