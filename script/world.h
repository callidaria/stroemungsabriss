#pragma once

// essentials
#include <iostream>
#include <vector>

// engine
#include "../ccb/frm/frame.h"
#include "../ccb/frm/framebuffer.h"

#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/renderer3d.h"

// scripts
#include "struct/world_structures.h"

struct EngineReference
{
	Frame* frame;
	Renderer2D* r2d;
	Renderer3D* r3d;
};

class World
{
public:

	// construction
	World(EngineReference eref);
	~World() {  }

	// creation
	void add_ui(UI* ui);
	void add_boss(Boss* boss);

	// destruction
	void remove_ui(uint8_t ui_id);
	void remove_boss(uint8_t boss_id);

	// draw
	void render(uint32_t &running,bool &reboot);

public:

	// public indices
	uint8_t active_menu = 0;

private:

	// cascabel
	EngineReference m_eref;
	FrameBuffer game_fb;

	// scene components
	std::vector<UI*> ui_master;
	std::vector<Boss*> boss_master;
};