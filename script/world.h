#ifndef SCR_ROOT_WORLD
#define SCR_ROOT_WORLD

// essentials
#include <iostream>
#include <vector>

// engine
#include "../ccb/frm/frame.h"
#include "../ccb/frm/framebuffer.h"
#include "../ccb/frm/gbuffer.h"

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/renderer3d.h"
#include "../ccb/gfx/cubemap.h"

#include "../ccb/mat/lighting.h"

// scripts
#include "struct/feature_base.h"
#include "struct/world_structures.h"

class World
{
public:

	// construction
	World(CascabelBaseFeature* eref,StageSetup* set_rigs);
	~World() {  }

	// creation
	void add_ui(UI* ui);
	void add_scene(Scene* scene);
	void add_playable(Player* player);
	void add_boss(Boss* boss);

	// destruction
	void free_memory();
	void remove_ui(uint8_t ui_id);
	void remove_scene(uint8_t scene_id);
	void remove_playable(uint8_t player_id);
	void remove_boss(uint8_t boss_id);

	// load
	void load_geometry();
	void upload_lighting();
	void upload_lightmap();

	// draw
	void render(uint32_t &running,bool &reboot);

public:

	// public indices
	uint8_t active_daui = 0,active_cam2D = 0,active_cam3D = 0;

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	StageSetup* m_setRigs;
	FrameBuffer deferred_fb,game_fb;
	GBuffer gbuffer;

	// scene components
	std::vector<UI*> ui_master;
	std::vector<Scene*> scene_master;
	std::vector<Player*> player_master;
	std::vector<Boss*> boss_master;
};

#endif