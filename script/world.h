#ifndef SCR_ROOT_WORLD
#define SCR_ROOT_WORLD

#include "../ccb/core.h"
#include "../ccb/frm/framebuffer.h"
#include "../ccb/frm/gbuffer.h"

#include "../ccb/gfx/renderer3d.h"
#include "../ccb/gfx/cubemap.h"

#include "../ccb/mat/lighting.h"
#include "../ccb/fcn/ccb_manager.h"

#include "struct/feature_base.h"
#include "struct/world_structures.h"


struct BoardTuple
{
	Board* data;
	board_logic logic;
};


class World
{
public:

	// construction
	World(CascabelBaseFeature* eref);
	~World() {  }

	// creation
	inline void add_scene(Scene* scene) { scene_master.push_back(scene); }
	inline void add_playable(Player* player) { player_master.push_back(player); }
	inline void add_boss(Boss* boss) { boss_master.push_back(boss); }

	// destruction
	void free_memory();
	void remove_ui(uint8_t ui_id);
	void remove_scene(uint8_t scene_id);
	void remove_playable(uint8_t player_id);
	void remove_boss(uint8_t boss_id);

	// load
	void load(float& progress,float ldsplit);

	// draw
	void render(bool& running,bool& reboot);

public:

	// public indices
	uint8_t active_daui = 0;

public:

	// ui components
	std::vector<UI*> ui_master;

	// board components
	std::vector<BoardTuple> board_master;

private:

	// cascabel
	CascabelBaseFeature* m_ccbf;
	FrameBuffer game_fb = FrameBuffer("./shader/standard/framebuffer.vs","./shader/menu/mainmenu.fs");
	uint8_t rtarget_id;

	// scene components
	std::vector<Scene*> scene_master;
	std::vector<Player*> player_master;
	std::vector<Boss*> boss_master;
};


struct LoadStorage
{
	CascabelBaseFeature* m_ccbf;
	CCBManager* m_ccbm;
	World* world;
	bool ldfb_showing = false;
	float progress = .0;
};
// FIXME: just give world pointer as parameter, some of these will be global, some of these belong in world

#endif
