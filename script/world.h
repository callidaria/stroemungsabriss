#ifndef SCR_ROOT_WORLD
#define SCR_ROOT_WORLD

#include "../ccb/core.h"
#include "../ccb/frm/framebuffer.h"
#include "../ccb/frm/gbuffer.h"

#include "../ccb/gfx/cubemap.h"

#include "../ccb/mat/lighting.h"
#include "../ccb/fcn/ccb_manager.h"

#include "systems/bullet_system.h"


//		BOARD DEFINITION
struct Board { };
typedef void (*board_logic)(Board*);


// describes any form of large scale user interface the player interacts with mainly, when enabled
class UI
{
public:
	virtual ~UI() {  }
	virtual void render(FrameBuffer*,bool&,bool&) {  }
};

// describes all environments the player visits during playtime
class Scene
{
public:
	virtual ~Scene() {  }
	virtual void render() {  }
};

// describes all objects/character, that can be controlled by the player
class Player
{
public:
	virtual ~Player() {  }
	virtual void update() {  }

public:
	glm::vec3 position = glm::vec3(0);
};

// describes any form of enemy, that can be considered significant and/or deserves a health bar
class Boss
{
public:
	virtual ~Boss() {  }
	virtual void update(glm::vec2) {  }
};


//		REGISTRATION STRUCTURE
struct BoardTuple
{
	Board* data;
	board_logic logic;
};
// TODO: split render instructions into 2D, 3D, transparency, etc... to reduce preparation and possible render
//		pipeline issues


// update pipeline handling
class World
{
public:

	// construction
	World();
	~World() {  }

	// creation
	inline void add_scene(Scene* scene) { scene_master.push_back(scene); }
	inline void add_playable(Player* player) { player_master.push_back(player); }
	inline void add_boss(Boss* boss) { boss_master.push_back(boss); }

	// load
	void load(float& progress,float ldsplit);

	// draw
	void render(bool& running,bool& reboot);

public:

	// public indices
	static inline uint8_t active_daui = 0;

public:

	// ui components
	std::vector<UI*> ui_master;

	// board components
	std::vector<BoardTuple> board_master;

private:

	// cascabel
	FrameBuffer game_fb = FrameBuffer("./shader/standard/framebuffer.vs","./shader/menu/mainmenu.fs");
	uint8_t rtarget_id;

	// scene components
	std::vector<Scene*> scene_master;
	std::vector<Player*> player_master;
	std::vector<Boss*> boss_master;
};


struct LoadStorage
{
	CCBManager* m_ccbm;
	World* world;
	bool ldfb_showing = false;
	float progress = .0;
};
// FIXME: just give world pointer as parameter, some of these will be global, some of these belong in world

#endif
