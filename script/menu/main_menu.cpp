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

	// buffers
	fb = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_mainmenu.shader","./shader/fbf_mainmenu.shader");
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

	// menu transition
	menu_action = (menu_action||hit_a)&&!hit_b;
	mtransition += (menu_action-!menu_action)*TRANSITION_SPEED*m_ccbf->frame->time_delta;
	mtransition = (mtransition<.0f) ? .0f : (mtransition>1.f) ? 1.f : mtransition;
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/

	// title animation
	m_ccbf->r2d->al[index_ranim].model = glm::translate(glm::mat4(1),
			VRT_TITLE_START+VRT_TITLE_TRANSITION*mtransition);
	m_ccbf->r2d->al[index_ranim+1].model = glm::translate(glm::mat4(1),
			HRZ_TITLE_START+HRZ_TITLE_TRANSITION*mtransition);

	// START RENDER MENU BUFFER
	fb.bind();
	Frame::clear();

	// render titles
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(index_ranim,glm::vec2(3,0));
	m_ccbf->r2d->render_state(index_ranim+1,glm::vec2(0,0));

	// STOP RENDER MENU BUFFER
	FrameBuffer::close();

	// render menu
	fb.render(mtransition);
}