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
#include "ui/ui.h"

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

	// destruction
	void remove_ui(uint8_t ui_id);

	// draw
	void render(uint32_t &running,bool &reboot);

private:

	// cascabel
	EngineReference m_eref;
	FrameBuffer game_fb;

	// scene components
	std::vector<UI*> ui_master;
};