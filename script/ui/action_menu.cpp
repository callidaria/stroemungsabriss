#include "action_menu.h"

/*
	construction(Frame*,InputMap*)
	frame: pointer to frame the game is drawn onto
	input_map: input map to use prehandled controller/keyboard/stick input
	purpose: creates the lightweight in-action menu to pause and use while in-game
*/
ActionMenu::ActionMenu(Frame* frame,InputMap* input_map,float &progress,float pseq)
	: m_frame(frame),imap(input_map)
{
	// vertices for selection splash
	float sseq = pseq/3.0f;
	float sverts[] = { 0,-25,0, 1280,0,2, 0,0,1, 1280,0,2, 0,-25,0, 1280,-25,3, };
	splash_buffer.bind();
	splash_buffer.upload_vertices(sverts,sizeof(sverts));
	splash_shader.compile("shader/fbv_lselect.shader","shader/fbf_lselect.shader");
	splash_shader.def_attributeF("position",2,0,3);
	splash_shader.def_attributeF("index",1,2,3);
	splash_shader.upload_camera(Camera2D(1280,720));
	progress += sseq;

	// add menu text
	mtext.add("continue",glm::vec2(240,TEXT_YPOSITION_SYS));
	mtext.add("settings (reduced)",glm::vec2(240,TEXT_YPOSITION_SYS-ACT_TEXT_SIZE));
	mtext.add("quit",glm::vec2(240,TEXT_YPOSITION_SYS-2*ACT_TEXT_SIZE));
	mtext.add("phone",glm::vec2(2240,TEXT_YPOSITION_INFO));
	mtext.add("hitlist",glm::vec2(2240,TEXT_YPOSITION_INFO-ACT_TEXT_SIZE));
	mtext.add("collection",glm::vec2(2240,TEXT_YPOSITION_INFO-2*ACT_TEXT_SIZE));
	mtext.add("world order",glm::vec2(2240,TEXT_YPOSITION_INFO-3*ACT_TEXT_SIZE));
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	mtext.load();
	progress += sseq;

	// add text shadow
	stext.add("continue",glm::vec2(237,TEXT_YPOSITION_SYS-4));
	stext.add("settings (reduced)",glm::vec2(237,TEXT_YPOSITION_SYS-ACT_TEXT_SIZE-4));
	stext.add("quit",glm::vec2(237,TEXT_YPOSITION_SYS-2*ACT_TEXT_SIZE-4));
	stext.add("phone",glm::vec2(2237,TEXT_YPOSITION_INFO-4));
	stext.add("hitlist",glm::vec2(2237,TEXT_YPOSITION_INFO-ACT_TEXT_SIZE-4));
	stext.add("collection",glm::vec2(2237,TEXT_YPOSITION_INFO-2*ACT_TEXT_SIZE-4));
	stext.add("world order",glm::vec2(2237,TEXT_YPOSITION_INFO-3*ACT_TEXT_SIZE-4));
	stext.load();
	progress += sseq;
}

/*
	render(FrameBuffer*,uint32_t&,bool&) -> void (virtual)
	conforming to: UI::render
	purpose: render current menu state
		ptrans ~= 1.0f: renders game normally as the game has not been stopped at this moment
		ptrans ~= 0.0f: renders game sepia filtered and the menu is placed on top
	!! DISCLAIMER: has to be run after game board components, but before all other ui using bb !!
*/
void ActionMenu::render(FrameBuffer* game_fb,bool &running,bool &reboot)
{
	// action menu open requests
	bool pause = imap->input_val[InputID::PAUSE],details = imap->input_val[InputID::DETAILS];
	bool down = imap->input_val[InputID::DOWN],up = imap->input_val[InputID::UP];
	bool req_sysmenu = pause&&!menu_trg&&!menu_inf;
	menu_sys = menu_sys*!req_sysmenu+!menu_sys*req_sysmenu;
	bool req_infomenu = details&&!menu_trg&&!menu_sys;
	menu_inf = menu_inf*!req_infomenu+!menu_inf*req_infomenu;
	menu_trg = pause||details;

	// game time modification
	ptrans += -.1f*((menu_sys||menu_inf)&&ptrans>0)+.1f*((!menu_sys&&!menu_inf)&&ptrans<1);
	m_frame->time_mod = ptrans;

	// process user selection
	int8_t smod = ((down&&!trg_smod)*((msel<2&&menu_sys)||(isel<3&&menu_inf))
			-(up&&!trg_smod)*((msel>0&&menu_sys)||(isel>0&&menu_inf)))
			*(ptrans<.1f);
	msel += smod*menu_sys;
	isel += smod*menu_inf;
	trg_smod = down||up;

	// get new edge values
	for (int i=0;i<4*(smod!=0&&(menu_sys||menu_inf));i++) sEdges[i] = rand()%15-10;

	// close when quit selected and hit
	running *= !(imap->request(InputID::FOCUS)&&msel==2&&menu_sys);

	// sepia colourspace when paused
	m_frame->clear();
	game_fb->prepare();
	game_fb->s.upload_vec2(
			"ratio",
			glm::vec2(Init::iConfig[FRAME_RESOLUTION_WIDTH,Init::iConfig[FRAME_RESOLUTION_HEIGHT]])
		);
	game_fb->s.upload_float("vignette",.44f+(float)(rand()%21)*.001f);
	game_fb->s.upload_float("mtransition",ptrans);
	game_fb->render();
	// FIXME: this requires ratio to already be uploaded to game_fb shader at load time

	// selection splash modifications
	uint16_t strans = TEXT_YPOSITION_SYS*menu_sys+TEXT_YPOSITION_INFO*menu_inf
			-(msel*menu_sys+isel*menu_inf)*ACT_TEXT_SIZE;
	splash_shader.enable();
	splash_shader.upload_vec2("idx_mod[0]",glm::vec2(0,strans+(rand()%10-5)-sEdges[0]));
	splash_shader.upload_vec2("idx_mod[1]",glm::vec2(0,strans+(rand()%10-5)+sEdges[1]));
	splash_shader.upload_vec2("idx_mod[2]",glm::vec2(0,strans+(rand()%10-5)+sEdges[2]));
	splash_shader.upload_vec2("idx_mod[3]",glm::vec2(0,strans+(rand()%10-5)-sEdges[3]));

	// render selection splash
	splash_buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6*(menu_sys||menu_inf));

	// render menu text with text shadow
	stext.prepare();
	stext.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(-2000*menu_inf,0,0)));
	stext.render(1024*(menu_sys||menu_inf),glm::vec4(0,0,0,.4f));
	mtext.prepare();
	mtext.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(-2000*menu_inf,0,0)));
	mtext.render(1024*(menu_sys||menu_inf),glm::vec4(0,.4f,0,1));
}
