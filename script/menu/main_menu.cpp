#include "main_menu.h"

/*
	constructor(CCBManager*,CascabelBaseFeature*,World*,float&,float)
	purpose: setup menu environment, populate with lists and define input possibilities
	\param ccbm: cascabel manager used to load environment and level loader files
	\param ccbf: collection of basic cascabel features
	TODO: extend documentation
*/
MainMenu::MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,
		float &progress,float pseq)
	: m_ccbm(ccbm),m_ccbf(ccbf),m_world(world)
{
	// asset load
	index_ranim = ccbf->r2d->al.size();
	index_rsprite = ccbm->add_lv("lvload/main_menu.ccb");

	// selection splash setup
	std::vector<float> sverts;
	create_splash(sverts,glm::vec2(12.5f,0),glm::vec2(510,720));
	sh_buffer.bind();
	sh_buffer.upload_vertices(sverts);
	sh_shader.compile("./shader/vsplash.shader","./shader/fsplash.shader");
	sh_shader.def_attributeF("position",2,0,3);
	sh_shader.def_attributeF("edge_id",1,2,3);
	sh_shader.upload_camera(Camera2D(1280,720));

	// text setup
	std::string vmessage = "yomisensei by callidaria. danmaku v"
			+ std::to_string(INFO_VERSION_RELEASE)+'.'+std::to_string(INFO_VERSION_SUBRELEASE)+'.'
			+ std::to_string(INFO_VERSION_DEVSTEP)+INFO_VERSION_MODE_SUFFIX
			+ " - running on cascabel base (OpenGL)";
	tx_version.add(vmessage.c_str(),TEXT_VERSION_POSITION);
	tx_version.load();
	tcap_version = vmessage.length();

	// peripheral sensitive input request annotations
	update_peripheral_annotations();

	// buffers
	msaa = MSAA("./shader/fbv_standard.shader","./shader/main_menu/fbf_splash.shader",
			m_ccbf->frame->w_res,m_ccbf->frame->h_res,8);
	msaa.sfb.upload_int("menu_fb",1);
	fb_menu = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_mainmenu.shader");
}

/*
	render(FrameBuffer*,bool&,bool&) -> void (virtual) !O(1)
	purpose: render the main menu, calculate geometry and process interactions
	\param game_fb: deferred shaded game scene framebuffer
	\param running: is application still running?
	\param reboot: will be rebooting with new settings after game closed?
*/
void MainMenu::render(FrameBuffer* game_fb,bool &running,bool &reboot)
{
	// input
	bool plmb = m_ccbf->frame->mouse.mcl&&!trg_lmb,prmb = m_ccbf->frame->mouse.mcr&&!trg_rmb;
	bool hit_a = (m_ccbf->iMap->get_input_triggered(IMP_REQPAUSE)&&!menu_action)
			|| m_ccbf->iMap->get_input_triggered(IMP_REQFOCUS)||plmb
			|| m_ccbf->iMap->get_input_triggered(IMP_REQCONFIRM),
		hit_b = (m_ccbf->iMap->get_input_triggered(IMP_REQPAUSE)&&menu_action)
			|| m_ccbf->iMap->get_input_triggered(IMP_REQBOMB)||prmb;
	trg_lmb = m_ccbf->frame->mouse.mcl,trg_rmb = m_ccbf->frame->mouse.mcr;

	// timing
	bool anim_go = anim_timing>ANIMATION_UPDATE_TIMEOUT;
	anim_timing += m_ccbf->frame->time_delta;
	dt_tshiftdown += m_ccbf->frame->time_delta*speedup,
			dt_tnormalize += m_ccbf->frame->time_delta*!speedup;

	// menu transition
	menu_action = (menu_action||hit_a)&&!hit_b;
	mtransition += (menu_action-!menu_action)*TRANSITION_SPEED*m_ccbf->frame->time_delta;
	mtransition = (mtransition<.0f) ? .0f : (mtransition>1.f) ? 1.f : mtransition;
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/
	// TODO: compare linear transition with sinespeed transition implementation
	float inv_mtransition = 1.f-mtransition;

	// title rattle animation
	uint8_t rattle_mobility = RATTLE_THRESHOLD+RATTLE_THRESHOLD_RAGEADDR*menu_action,
		rattle_countermove = rattle_mobility/2;
	glm::vec3 title_action = glm::vec3((rand()%rattle_mobility-rattle_countermove)*anim_go,
			(rand()%rattle_mobility-rattle_countermove)*anim_go,0);

	// title shiftdown animation
	dt_tshiftdown *= menu_action,dt_tnormalize *= menu_action,speedup = speedup||!menu_action;
	float tshift = 1.f+SHIFTDOWN_ZOOM_INCREASE*((speedup) ? sqrt(sin(dt_tshiftdown*MATH_OCTAPI))
			: 1.f-sqrt(dt_tnormalize));

	// title animation
	glm::vec3 vrt_position = VRT_TITLE_START+VRT_TITLE_TRANSITION*mtransition+title_action,
		hrz_position = HRZ_TITLE_START+HRZ_TITLE_TRANSITION*mtransition+title_action;
	glm::mat4 vrt_scale = glm::translate(glm::scale(glm::translate(glm::mat4(1),
			-VRT_TITLE_SCALESET),glm::vec3(tshift)),VRT_TITLE_SCALESET),
		hrz_scale = glm::translate(glm::scale(glm::translate(glm::mat4(1),
			-HRZ_TITLE_SCALESET),glm::vec3(tshift)),HRZ_TITLE_SCALESET);
	m_ccbf->r2d->al[index_ranim].model = glm::translate(glm::mat4(1),vrt_position)*vrt_scale;
	m_ccbf->r2d->al[index_ranim+1].model = glm::translate(glm::mat4(1),hrz_position)*hrz_scale;

	// peripheral switch for input request annotation
	if (cpref_peripheral!=m_ccbf->frame->cpref_peripheral) update_peripheral_annotations();

	// start multisampled render
	msaa.bind();

	// selection splash render
	sh_buffer.bind();
	sh_shader.enable();
	glDrawArrays(GL_TRIANGLES,0,6);

	// START RENDER MENU BUFFER
	msaa.blit();
	fb_menu.bind();
	Frame::clear();

	// render text
	tx_dare.prepare();
	tx_dare.set_scroll(glm::vec2(title_action));
	tx_dare.render(tcap_dare*inv_mtransition,TEXT_DARE_COLOUR);
	tx_version.prepare();
	tx_version.render(tcap_version*inv_mtransition,TEXT_VERSION_COLOUR);

	// render titles
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(index_ranim,glm::vec2(3,0));
	m_ccbf->r2d->render_state(index_ranim+1,glm::vec2(0,0));

	// STOP RENDER MENU BUFFER
	FrameBuffer::close();

	// render menu
	fb_menu.render(mtransition);
	msaa.prepare();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,fb_menu.tex);
	MSAA::render();
	glActiveTexture(GL_TEXTURE0);
	// FIXME: remove special treatment and transfer to a more controllable implementation

	// finishing
	bool shiftdown_over = dt_tshiftdown>TITLE_SHIFTDOWN_TIMEOUT,
			normalize_over = dt_tnormalize>TITLE_NORMALIZATION_TIMEOUT;
	anim_timing -= ANIMATION_UPDATE_TIMEOUT*anim_go;
	dt_tshiftdown -= TITLE_SHIFTDOWN_TIMEOUT*shiftdown_over,
			dt_tnormalize -= TITLE_NORMALIZATION_TIMEOUT*normalize_over;
	speedup = (speedup&&!shiftdown_over)||normalize_over;
}

/*
	TODO
*/
void MainMenu::create_splash(std::vector<float> &sverts,glm::vec2 l,glm::vec2 u)
{
	std::vector<float> verts
			= { l.x,l.y,0, u.x,u.y,2, u.x,u.y,1, u.x,u.y,2, l.x,l.y,0, l.x,l.y,3 };
	sverts.insert(sverts.end(),verts.begin(),verts.end());
}

/*
	TODO
*/
void MainMenu::update_peripheral_annotations()
{
	// update shown preferred peripheral
	cpref_peripheral = m_ccbf->frame->cpref_peripheral;

	// title screen dare message
	tx_dare.clear();
	std::string reqbutton = (cpref_peripheral) ? m_ccbf->iMap->cnt_name[IMP_REQPAUSE]
			: m_ccbf->iMap->key_name[IMP_REQCONFIRM];
	std::string dmessage = "press ["+reqbutton+"] if you DARE";
	tx_dare.add(dmessage.c_str(),TEXT_DARE_POSITION);
	tcap_dare = dmessage.length();
	tx_dare.load();
}