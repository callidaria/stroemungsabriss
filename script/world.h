#pragma once

// essentials
#include <iostream>
#include <vector>

// engine
#include "../ccb/frm/frame.h"
#include "../ccb/frm/framebuffer.h"
#include "../ccb/frm/gbuffer.h"

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/renderer3d.h"
#include "../ccb/gfx/light3d.h"

// scripts
#include "struct/feature_base.h"
#include "struct/world_structures.h"

class World
{
public:

	// construction
	World(CascabelBaseFeature* eref);
	~World() {  }

	// creation
	void add_ui(UI* ui);
	void add_scene(Scene* scene);
	void add_playable(Player* player);
	void add_boss(Boss* boss);

	// set creation
	void add_camera(Camera2D cam2D);
	void add_camera(Camera3D cam3D);

	// destruction
	void free_memory();
	void remove_ui(uint8_t ui_id);
	void remove_scene(uint8_t scene_id);
	void remove_playable(uint8_t player_id);
	void remove_boss(uint8_t boss_id);

	// load
	void load_geometry();

	// draw
	void render(uint32_t &running,bool &reboot);

public:

	// public indices
	uint8_t active_daui = 0,active_cam2D = 0,active_cam3D = 0;

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	FrameBuffer deferred_fb,game_fb;
	GBuffer gbuffer = GBuffer(1600.0f,900.0f);

	// scene components
	std::vector<UI*> ui_master;
	std::vector<Scene*> scene_master;
	std::vector<Player*> player_master;
	std::vector<Boss*> boss_master;

	// set components
	std::vector<Camera2D> cam2D_master;
	std::vector<Camera3D> cam3D_master;
	std::vector<Light3D> light_master;
};