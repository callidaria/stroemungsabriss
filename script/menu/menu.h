#pragma once

#include <iostream>

#include "../../ccb/frm/frame.h"
#include "../../ccb/fcn/ccb_linpr.h"
#include "../../ccb/fcn/ccb_manager.h"

#include "../../ccb/gfx/renderer2d.h"
#include "../../ccb/gfx/renderer3d.h"
#include "../../ccb/gfx/rendereri.h"
#include "../../ccb/gfx/light3d.h"
#include "../../ccb/gfx/material3d.h"

#include "../../ccb/mat/camera2d.h"
#include "../../ccb/mat/camera3d.h"

#include "../../ccb/frm/framebuffer.h"

#include "../../ccb/fcn/text.h"
#include "../../ccb/fcn/buffer.h"

#include "../../ccb/ppe/msaa.h"

#include "menu_list.h"
#include "menu_dialogue.h"

#include "../pcs/jaeger_jet.h"
#include "../systems/input_map.h"
#include "../struct/world_structures.h"
#include "../scenes/nepal_mountain_woods.h"
#include "../boss/dpilot.h"
#include "../ui/action_menu.h"

#include "../world.h"

#define NCOLOUR 1

constexpr uint8_t GVERSION_RELEASE = 0;
constexpr uint8_t GVERSION_SUBRELEASE = 0;
constexpr uint8_t GVERSION_DEVSTEP = 5;
constexpr char GVERSION_SUFFIX = 'd';

enum MenuMode
{
	MENU_TITLE,			// rv = 0
	MENU_SELECTION,		// rv = 1
	MENU_START,			// rv = 2
	MENU_DIFFS,			// rv = 3
	MENU_LISTING,		// rv = 4
	MENU_SUBLIST		// rv = 5
};

// title position animation constants
constexpr glm::vec3 TITLE_START = glm::vec3(300,300,0);
constexpr glm::vec3 ENTITLE_START = glm::vec3(590,800,0);
constexpr glm::vec3 TITLE_MENU = glm::vec3(100,250,0);
constexpr glm::vec3 ENTITLE_MENU = glm::vec3(590,642,0);
constexpr glm::vec3 title_dir = TITLE_MENU-TITLE_START;
constexpr glm::vec3 entitle_dir = ENTITLE_MENU-ENTITLE_START;

// colour scheme constants
#if NCOLOUR == 0
constexpr glm::vec3 s_rgb = glm::vec3(.245f,.606f,.564f);
constexpr glm::vec3 h_rgb = glm::vec3(.341f,.341f,.129f);
#elif NCOLOUR == 1
constexpr glm::vec3 s_rgb = glm::vec3(.0985f,.270f,.037f);
constexpr glm::vec3 h_rgb = glm::vec3(.75f,.4125f,0);
#endif
constexpr glm::vec3 l_rgb = glm::vec3(1,0,0);
constexpr glm::vec3 r_rgb = glm::vec3(0,0,1);

class Menu : public UI
{
public:

	// construction
	Menu() {  }
	Menu(World* world,CCBManager* ccbm,CascabelBaseFeature* ccbf);
	~Menu();

	// draw
	virtual void render(FrameBuffer* game_fb,uint32_t &running,bool &reboot);

private:

	// engine components
	CCBManager* m_ccbm;
	Buffer buffer = Buffer();
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	Camera3D cam3d = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f),
			orthocam = Camera3D(1280.0f,720.0f);
	Material3D mat0;
	Shader sshd = Shader();
	FrameBuffer fb,globe_fb;
	Font fnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",25,25);
	Text tft,vtft;
	MenuMode mm = MenuMode::MENU_TITLE;
	MSAA msaa;
	CascabelBaseFeature* m_ccbf;
	JaegerJet jj;
	DPilot dpilot;
	NepalMountainWoods nmw;
	World* m_world;
	// TODO: remove all that exists in cascabel base feature
	// join ccbbf and engine reference

	// input definition
	bool trg_start=false,trg_b=false,trg_lft=false,trg_rgt=false,trg_dwn=false,trg_up=false;

	// menu selection
	MenuList mls[12];
	int32_t lselect = 0,lbounds = 7,lscroll = 0,diffsel = 0,opt_index = 0;

	// animation
	int32_t sbar[8] = { 0 };
	uint32_t SELTRANS[16]
			= { 900,2500, 34,170, 221,380, 419,609, 644,810, 871,997, 1027,1258, 1212,1498 };
	glm::mat4 pos_title = glm::mat4(1.0f),pos_entitle = glm::mat4(1.0f);
	float ptrans = 0,dtrans = 0;
	uint32_t msindex = 0;
	uint8_t mselect = 7;
	glm::vec2 mve = glm::vec2(0),mvj = glm::vec2(0);
	float edge_mod = 0;
	bool edge_sel = false;
	uint8_t md_disp = 0;
	uint8_t hrz_title = 0,vrt_title = 3;
	float dlgrot_val = 0,dlgrot_cnt = 0;

	// menu title speedup animation
	bool neg_vscl = 0,neg_hscl = 0,neg_vrot = 0,neg_hrot = 0;
	float val_vscl = 1,val_hscl = 1,val_vrot = 0,val_hrot = 0;

	// dialogue
	std::vector<const char*> pth_diff = {
		"res/diffs/normal_diff.png","res/diffs/master_diff.png",
		"res/diffs/gmaster_diff.png","res/diffs/rhack_diff.png"
	};
	std::vector<const char*> pth_conf = { "res/ok.png","res/no.png" };
	MenuDialogue md_diff,md_conf;
	uint8_t dsi_diff = 0,dsi_conf = 0;

	// game
	//Game game = Game(m_frame,m_r2d,m_r3d,m_rI,m_cam2d,imap);
	ActionMenu action_menu;
	uint8_t difflv = 0;

	// globe preview
	uint16_t ridx_terra;
	float pitch = 15;
	float yaw = -110;
};