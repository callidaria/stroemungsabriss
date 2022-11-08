#include "action_menu.h"

/*
	construction(Frame*)
	frame: pointer to frame the game is drawn onto
	purpose: creates the lightweight in-action menu to pause and use while in-game
*/
ActionMenu::ActionMenu(Frame* frame)
	: m_frame(frame)
{
	// sepia menu background framebuffer
	game_fb = FrameBuffer(m_frame->w_res,m_frame->h_res,
			"./shader/fbv_menu.shader","./shader/fbf_menu.shader",false);

	// vertices for selection splash
	float sverts[] = { 0,-25,0, 0,0,1, 1280,0,2, 1280,0,2, 1280,-25,3, 0,-25,0, };
	splash_buffer.bind();
	splash_buffer.upload_vertices(sverts,sizeof(sverts));
	splash_shader.compile("shader/fbv_lselect.shader","shader/fbf_lselect.shader");
	splash_shader.def_attributeF("position",2,0,3);
	splash_shader.def_attributeF("index",1,2,3);
	splash_shader.upload_camera(Camera2D(1280,720));

	// add menu text
	mtext = Text(&tfont);
	mtext.add("continue",glm::vec2(240,TEXT_YPOSITION_SYS));
	mtext.add("settings (reduced)",glm::vec2(240,TEXT_YPOSITION_SYS-TEXT_SIZE));
	mtext.add("quit",glm::vec2(240,TEXT_YPOSITION_SYS-2*TEXT_SIZE));
	mtext.add("phone",glm::vec2(2240,TEXT_YPOSITION_INFO));
	mtext.add("hitlist",glm::vec2(2240,TEXT_YPOSITION_INFO-TEXT_SIZE));
	mtext.add("collection",glm::vec2(2240,TEXT_YPOSITION_INFO-2*TEXT_SIZE));
	mtext.add("world order",glm::vec2(2240,TEXT_YPOSITION_INFO-3*TEXT_SIZE));
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	mtext.load(&cam2d);

	// add text shadow
	stext = Text(&tfont);
	stext.add("continue",glm::vec2(237,TEXT_YPOSITION_SYS-4));
	stext.add("settings (reduced)",glm::vec2(237,TEXT_YPOSITION_SYS-TEXT_SIZE-4));
	stext.add("quit",glm::vec2(237,TEXT_YPOSITION_SYS-2*TEXT_SIZE-4));
	stext.add("phone",glm::vec2(2237,TEXT_YPOSITION_INFO-4));
	stext.add("hitlist",glm::vec2(2237,TEXT_YPOSITION_INFO-TEXT_SIZE-4));
	stext.add("collection",glm::vec2(2237,TEXT_YPOSITION_INFO-2*TEXT_SIZE-4));
	stext.add("world order",glm::vec2(2237,TEXT_YPOSITION_INFO-3*TEXT_SIZE-4));
	stext.load(&cam2d);
}

/*
	update(Player*) -> void
	player: player controller holding custom input addresses for menu interaction
	purpose: animation updates and user request interpretation regarding action menu
*/
void ActionMenu::update(Player* player,uint32_t &run_state)
{
	// action menu open requests
	bool req_sysmenu = *player->cnt.pause&&!menu_trg&&!menu_inf;
	menu_sys = menu_sys*!req_sysmenu+!menu_sys*req_sysmenu;
	bool req_infomenu = *player->cnt.rdetails&&!menu_trg&&!menu_sys;
	menu_inf = menu_inf*!req_infomenu+!menu_inf*req_infomenu;
	menu_trg = *player->cnt.pause||*player->cnt.rdetails;

	// game time modification
	ptrans += -.1f*((menu_sys||menu_inf)&&ptrans>0)+.1f*((!menu_sys&&!menu_inf)&&ptrans<1);
	m_frame->set_tmod(ptrans);

	// process user selection
	int8_t smod = ((*player->cnt.abs_down&&!trg_smod)*((msel<2&&menu_sys)||(isel<3&&menu_inf))
			-(*player->cnt.abs_up&&!trg_smod)*((msel>0&&menu_sys)||(isel>0&&menu_inf)))
			*(ptrans<.1f);
	msel += smod*menu_sys;
	isel += smod*menu_inf;
	trg_smod = *player->cnt.abs_down||*player->cnt.abs_up;

	// get new edge values
	for (int i=0;i<4*(smod!=0&&(menu_sys||menu_inf));i++) sEdges[i] = rand()%15-10;

	// close when quit selected and hit
	run_state *= !(*player->cnt.rng_focus&&msel==2&&menu_sys);
}

/*
	bind() -> void
	purpose: bind and clear game framebuffer so that it can serve as menu background
	!! DISCLAIMER: has to be used right before all the game board components !!
*/
void ActionMenu::bind()
{
	game_fb.bind();
	m_frame->clear();
}

/*
	render() -> void
	purpose: render current menu state
		ptrans ~= 1.0f: renders game normally as the game has not been stopped at this moment
		ptrans ~= 0.0f: renders game sepia filtered and the menu is placed on top
	!! DISCLAIMER: has to be run after game board components, as well as all other ui !!
*/
void ActionMenu::render()
{
	// sepia colourspace when paused
	FrameBuffer::close();
	m_frame->clear();
	game_fb.render(ptrans);

	// selection splash modifications
	uint16_t strans = TEXT_YPOSITION_SYS*menu_sys+TEXT_YPOSITION_INFO*menu_inf
			-(msel*menu_sys+isel*menu_inf)*TEXT_SIZE;
	splash_shader.enable();
	splash_shader.upload_vec2("idx_mod[0]",glm::vec2(0,strans+(rand()%10-5)-sEdges[0]));
	splash_shader.upload_vec2("idx_mod[1]",glm::vec2(0,strans+(rand()%10-5)+sEdges[1]));
	splash_shader.upload_vec2("idx_mod[2]",glm::vec2(0,strans+(rand()%10-5)+sEdges[2]));
	splash_shader.upload_vec2("idx_mod[3]",glm::vec2(0,strans+(rand()%10-5)-sEdges[3]));

	// render selection splash
	splash_buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6*(menu_sys||menu_inf));

	// render menu text
	stext.prepare();
	stext.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(-2000*menu_inf,0,0)));
	stext.render(1024*(menu_sys||menu_inf),glm::vec4(0,0,0,.4f));			// text shadow
	mtext.prepare();
	mtext.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(-2000*menu_inf,0,0)));
	mtext.render(1024*(menu_sys||menu_inf),glm::vec4(.6f,.0824f,.6f,1));	// menu text
}