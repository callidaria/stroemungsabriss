#include "world.h"

/*
	constructor(CascabelBaseFeature*,StageSetup*)
	eref: references to all relevant engine objects
	set_rigs: stage setup
	purpose: create a world handling entity for a better loop structure
*/
World::World(CascabelBaseFeature* eref,StageSetup* set_rigs)
	: m_ccbf(eref),m_setRigs(set_rigs)
{
	// g-buffer setup
	//gbuffer = GBuffer(eref->frame->w_res,eref->frame->h_res);

	// memfail avoidance shadow setup
	eref->r3d->create_shadow(glm::vec3(0),glm::vec3(3),1,1,1,1);

	// framebuffer setup
	game_fb = FrameBuffer(eref->frame->w_res,eref->frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);
	rtarget_id = eref->r3d->add_target(eref->frame);
	/*deferred_fb = FrameBuffer(eref->frame->w_res,eref->frame->h_res,
			"./shader/fbv_standard.shader","./shader/gbf_lighting.shader",false);
	deferred_fb.s.upload_int("gbuffer_colour",0);
	deferred_fb.s.upload_int("gbuffer_position",1);
	deferred_fb.s.upload_int("gbuffer_normals",2);
	deferred_fb.s.upload_int("gbuffer_materials",3);
	deferred_fb.s.upload_int("irradiance_map",4);
	deferred_fb.s.upload_int("specular_map",5);
	deferred_fb.s.upload_int("specular_brdf",6);
	deferred_fb.s.upload_int("shadow_map",7);*/
}

/*
	add_<element>(<element>*) -> void
	argument[0]: element to add to element list it belongs to
	purpose: create an element in the world that will be handled according to the world's logic
*/
void World::add_ui(UI* ui)
{ ui_master.push_back(ui); }
void World::add_scene(Scene* scene)
{ scene_master.push_back(scene); }
void World::add_playable(Player* player)
{ player_master.push_back(player); }
void World::add_boss(Boss* boss)
{ boss_master.push_back(boss); }

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
	load_geometry() -> void
	purpose: upload 2D and 3D geometry of added objects based on the active cameras
*/
void World::load_geometry(float &progress,float ldsplit)
{
	float org_progress = progress,pr_progress = ldsplit/3.0f;
	m_ccbf->r2d->load(&m_setRigs->cam2D[active_cam2D],progress,pr_progress);
	m_ccbf->rI->load(progress,pr_progress);
	m_ccbf->r3d->load(m_setRigs->cam3D[active_cam3D],progress,pr_progress);
	progress = org_progress+ldsplit;
}

/*
	upload_lighting() -> void
	purpose: upload lighting to deferred shader
*/
void World::upload_lighting()
{
	// upload simulated lights
	deferred_fb.s.enable();
	m_setRigs->lighting.upload(&deferred_fb.s);
}

/*
	upload_lightmap() -> void
	purpose: upload diffusion convolution and specular integral maps to shader
*/
void World::upload_lightmap()
{
	// upload irradiance maps
	glActiveTexture(GL_TEXTURE4);
	m_setRigs->lighting.upload_diffusion_map();
	glActiveTexture(GL_TEXTURE5);
	m_setRigs->lighting.upload_specular_map();
	glActiveTexture(GL_TEXTURE6);
	m_setRigs->lighting.upload_specular_brdf();
}

/*
	render(uint32_t&,bool&) -> void
	running: runstate of the program, used to store progression & if program is still running
	reboot: holds if program should reboot after it has been closed
	purpose: render & handle world entites
*/
void World::render(uint32_t &running,bool &reboot)
{
	// update
	glDisable(GL_BLEND);
	m_ccbf->r3d->update_animations(m_ccbf->frame->get_time_delta());
	m_ccbf->r3d->update_shadows(m_ccbf->frame->w_res,m_ccbf->frame->h_res);

	// start geometry pass deferred scene
	//gbuffer.bind();
	m_ccbf->r3d->start_target(rtarget_id);
	m_ccbf->frame->clear(.1f,.1f,.1f);

	// handle environments, bosses & player
	for (auto scene : scene_master) scene->render();
	for (auto boss : boss_master) boss->update(glm::vec2(100));
	for (auto player : player_master) player->update();
	//for (auto ui : ui_master) ui->render_gbuffer();

	// end geometry pass deferred scene
	FrameBuffer::close();
	glEnable(GL_BLEND);

	// render bullets
	m_ccbf->bSys->render();

	// upload g-buffer components to deferred light shader
	game_fb.bind();
	/*deferred_fb.prepare();
	glBindTexture(GL_TEXTURE_2D,gbuffer.get_colour());
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,gbuffer.get_position());
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,gbuffer.get_normals());
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,gbuffer.get_materials());
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,m_ccbf->r3d->shadow_map);*/
	m_ccbf->r3d->render_target(rtarget_id,m_setRigs->cam3D[active_cam3D],&m_setRigs->lighting);

	// deferred light shading
	//m_setRigs->lighting.upload(&deferred_fb.s);
	//m_setRigs->lighting.upload(&m_ccbf->r3d->rtargets[rtarget_id].cbuffer.s);
	/*deferred_fb.s.upload_vec3("view_pos",m_setRigs->cam3D[active_cam3D].pos);
	deferred_fb.s.upload_vec3("light_position",m_ccbf->r3d->slight_pos);
	deferred_fb.s.upload_matrix("shadow_matrix",m_ccbf->r3d->scam_projection);*/
	//glDrawArrays(GL_TRIANGLES,0,6);

	// render ui
	game_fb.close();
	ui_master[active_daui]->render(&game_fb,running,reboot);
}