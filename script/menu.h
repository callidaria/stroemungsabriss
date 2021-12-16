#pragma once

#include <iostream>
#include "../ccb/frm/frame.h"
#include "../ccb/fcn/ccb_linpr.h"
#include "../ccb/fcn/ccb_manager.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/rendereri.h"
#include "../ccb/mat/camera2d.h"
#include "../ccb/frm/framebuffer.h"
#include "../ccb/fcn/text.h"
#include "menu_list.h"
#include "game.h"

enum MenuMode
{
	MENU_TITLE,		// rv = 0
	MENU_SELECTION,		// rv = 1
	MENU_START,		// rv = 2
	MENU_DIFFS,		// rv = 3
	MENU_LISTING,		// rv = 4
	MENU_SUBLIST		// rv = 5
};

class Menu
{
public:
	Menu(CCBManager* ccbm,Frame* f,Renderer2D* r2d,RendererI* rI,Camera2D* cam2d);
	~Menu();
	void render(uint32_t &running);
private:
	uint32_t svao,svbo;
	CCBManager* m_ccbm;
	Frame* m_frame;
	Renderer2D* m_r2d;
	RendererI* m_rI;
	Camera2D* m_cam2d;
	Shader sshd;
	FrameBuffer fb,splash_fb,title_fb,select_fb,cross_fb;
	Text tft,vtft;
	MenuMode mm = MenuMode::MENU_TITLE;
	int32_t lselect = 0,lbounds = 7,lscroll = 0,diffsel = 0,opt_index = 0;
	int32_t sbar[8];
	MenuList mls[12];

	// input definition
	bool *cnt_b,*cnt_start,*cnt_lft,*cnt_rgt,*cnt_dwn,*cnt_up;
	bool trg_start=false,trg_b=false,trg_lft=false,trg_rgt=false,trg_dwn=false,trg_up=false;

	// animation values
	const glm::vec3 TITLE_START = glm::vec3(250,100,0);
	const glm::vec3 ENTITLE_START = glm::vec3(340,720,0);
	const glm::vec3 TITLE_MENU = glm::vec3(50,50,0);
	const glm::vec3 ENTITLE_MENU = glm::vec3(340,600,0);
	uint32_t SELTRANS[16] = { 900,2500, 34,170, 221,380, 419,609, 644,810, 871,997, 1027,1258, 1212,1498 };
	glm::mat4 pos_title,pos_entitle;
	float ptrans=0,dtrans=0,strans=0;
	glm::vec3 title_dir = TITLE_MENU-TITLE_START;
	glm::vec3 entitle_dir = ENTITLE_MENU-ENTITLE_START;
	uint32_t msindex;
	uint8_t mselect = 7;
	glm::vec2 mve,mvj;
	float edge_mod;
	bool edge_sel = false;

	Game game = Game(m_frame,m_r2d,m_rI,m_cam2d);
};
