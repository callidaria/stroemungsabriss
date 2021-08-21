#include "menu.h"

Menu::Menu(CCBManager* ccbm,Frame* f,Renderer2D* r2d,RendererI* rI,Camera2D* cam2d)
	: m_frame(f),m_r2d(r2d),m_rI(rI)
{
	const char* GVERSION = "0.0.2d";

	msindex = ccbm->add_lv("lvload/menu.ccb");

	Font fnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",25,25);
	Font vfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	tft = Text(fnt);vtft = Text(vfnt);
	tft.add("press START if you DARE",glm::vec2(450,250));
	vtft.add("yomisensei by callidaria. danmaku version 0.0.2d - running on cascabel 1.3.1v (OpenGL)",
			glm::vec2(630,20)); // FIXME: hardcoded version number
	tft.load_wcam(cam2d);vtft.load_wcam(cam2d);
	ml_stages = MenuList(cam2d,"lvload/ml_stages");
	ml_mopt = MenuList(cam2d,"lvload/ml_mopt");

	glGenVertexArrays(1,&svao);glGenBuffers(1,&svbo);
	sshd = Shader();
	float sverts[] = { // TODO: remove hardcoded secondary segment coords
		// title splash
		-25,0,25,0,.5f,0,0,0,420,720,-25,720,.5f,0,0,1,600,720,25,720,.5f,0,0,2,
		600,720,25,720,.5f,0,0,2,50,0,160,0,.5f,0,0,3,-25,0,25,0,.5f,0,0,0,
		// head splash
		0,500,0,500,.245f,.606f,.564f,0,0,500,0,550,.245f,.606f,.564f,1,
		0,500,1280,600,.245f,.606f,.564f,2,0,500,1280,600,.245f,.606f,.564f,2,
		0,500,1280,470,.245f,.606f,.564f,3,0,500,0,500,.245f,.606f,.564f,0,
		// select splash
		630,0,630,0,.341f,.341f,.129f,0,630,0,0,720,.341f,.341f,.129f,1,650,0,0,720,.341f,.341f,.129f,2,
		650,0,0,720,.341f,.341f,.129f,2,650,0,650,0,.341f,.341f,.129f,3,630,0,630,0,.341f,.341f,.129f,0
	}; // ??clockwise rotation triangle hardcoded replace
	glBindVertexArray(svao); // §§??
	glBindBuffer(GL_ARRAY_BUFFER,svbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sverts),sverts,GL_STATIC_DRAW);
	sshd.compile_vCols("shader/fbv_select.shader","shader/fbf_select.shader");
	sshd.upload_matrix("view",cam2d->view2D);sshd.upload_matrix("proj",cam2d->proj2D);

	fb=FrameBuffer(f->w_res,f->h_res,"shader/fbv_menu.shader","shader/fbf_menu.shader",false);
	splash_fb=FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
	title_fb=FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
	select_fb=FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);

	for (int i=0;i<4;i++) m_r2d->sl.at(msindex+16+i).scale_arbit(1,0);

	if (f->m_gc.size()>0) { // TODO: include all axis and common intuitive input systems
		cnt_b = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_B];
		cnt_start = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_A];
		cnt_lft = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_LEFT];
		cnt_rgt = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_RIGHT];
		cnt_dwn = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_DOWN];
		cnt_up = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_DPAD_UP];
	} else {
		cnt_b = &f->kb.ka[SDL_SCANCODE_Q];
		cnt_start = &f->kb.ka[SDL_SCANCODE_RETURN];
		cnt_lft = &f->kb.ka[SDL_SCANCODE_LEFT];
		cnt_rgt = &f->kb.ka[SDL_SCANCODE_RIGHT];
		cnt_dwn = &f->kb.ka[SDL_SCANCODE_DOWN];
		cnt_up = &f->kb.ka[SDL_SCANCODE_UP];
	}
}
Menu::~Menu() {  }
void Menu::render(uint32_t &running) // !!kill frame parameter
{
	bool is_shift;uint8_t tmm;
	switch (mm) {
	case MenuMode::MENU_TITLE:
		mm = (MenuMode)(MenuMode::MENU_SELECTION*(*cnt_start&&!trg_start));
		break;
	case MenuMode::MENU_SELECTION:
		tmm = 1;
		tmm += 3*(*cnt_start&&!trg_start);				// j listing
		tmm -= 2*(*cnt_start&&!trg_start&&mselect==7); 			// j start
		tmm = tmm*!(*cnt_b&&!trg_b);					// j title
		running = !(*cnt_start&&!trg_start&&mselect==2);		// exit
		mm = (MenuMode)tmm;
		is_shift = (tmm<5&&tmm>2)||(dtrans>.01f); // ??breakdown logic | determine when to shift title
		mve=(glm::vec2(360-(m_r2d->sl.at(msindex+mselect*2-2).sclx/2),650)
			-m_r2d->sl.at(msindex+mselect*2-2).pos)*glm::vec2(is_shift); // setting title destination
		mvj=(glm::vec2(50,50)-m_r2d->sl.at(msindex+mselect*2-1).pos)*glm::vec2(is_shift);
		mselect+=*cnt_rgt*(mselect<8&&!trg_rgt)-*cnt_lft*(mselect>2&&!trg_lft); // update selection marker
		break;
	case MenuMode::MENU_DIFFS:
		tmm = 3;
		tmm -= *cnt_start&&!trg_start;
		tmm += *cnt_b&&!trg_b;
		mm = (MenuMode)tmm;
		break;
	case MenuMode::MENU_LISTING:
		tmm = 4;
		tmm -= *cnt_start&&!trg_start;
		tmm -= 3*(*cnt_b&&!trg_b);
		mm = (MenuMode)tmm;
		lselect += (*cnt_dwn&&!trg_dwn)-(*cnt_up&&!trg_up&&lselect>0);
		break;
	default:running=lselect+2;game.run(running);
	} trg_start=*cnt_start;trg_b=*cnt_b;trg_lft=*cnt_lft;trg_rgt=*cnt_rgt;trg_dwn=*cnt_dwn;trg_up=*cnt_up;
	// !!break branch with static function pointer list

	// move non-used out of view
	for (int i=2;i<9;i++) { // !!i will regret this tomorrow ...just a test
		float tval = m_r2d->sl.at(msindex+i*2-1).pos.x+250;
		m_r2d->sl.at(msindex+i*2-1).model=glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(-tval,0,0));
		tval = m_r2d->sl.at(msindex+i*2-2).pos.x+250;
		m_r2d->sl.at(msindex+i*2-2).model=glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(-tval,0,0));
	}

	// animate movement within title positions
	ptrans+=.1f*(mm==1&&ptrans<1);ptrans-=.1f*(mm==0&&ptrans>.01f); // !!use an epsilon, pretty please
	dtrans+=.1f*(mm>2&&dtrans<1);dtrans-=.1f*(mm<2&&dtrans>.01f); // ??maybe reduce this
	strans+=.1f*(mm==3&&strans<1);strans-=.1f*(mm!=3&&strans>.01f);
	m_r2d->sl.at(msindex+mselect*2-2).model=glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(mve.x,mve.y,0));
	m_r2d->sl.at(msindex+mselect*2-1).model=glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(mvj.x,mvj.y,0));
	pos_title = glm::translate(glm::mat4(1.0f),TITLE_START+title_dir*ptrans+dtrans*glm::vec3(-140,0,0));
	pos_entitle = glm::translate(glm::mat4(1.0f),ENTITLE_START+entitle_dir*(ptrans-dtrans));
	for (int i=0;i<4;i++) m_r2d->sl.at(msindex+16+i).scale_arbit(1,strans);

	// menu list selection and scrolling
	lscroll += (lselect>lbounds)-(lselect<(lbounds-7));
	bool scddiff = lselect>lbounds;bool scudiff = lselect<(lbounds-7);
	lbounds = (!scddiff&&!scudiff)*lbounds+scddiff*lselect+scudiff*(lselect+7);
	int lcscroll = 45*(lselect+7-lbounds);
	for (int i=0;i<4;i++) sbar[i] = (diffsel!=lselect)*(rand()%37-20)+(diffsel==lselect)*sbar[i];

	// render the selection splash
	sshd.enable();glBindVertexArray(svao);
	sshd.upload_float("ptrans",ptrans);
	sshd.upload_vec2("idx_mod[0]",glm::vec2(0,0));sshd.upload_vec2("idx_mod[1]",glm::vec2(0,0));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(0,0));sshd.upload_vec2("idx_mod[3]",glm::vec2(0,0));
	splash_fb.bind();m_frame->clear(0,0,0);glDrawArrays(GL_TRIANGLES,0,6);splash_fb.close();
	sshd.upload_vec2("idx_mod[0]",glm::vec2(0,-15*(mm>2)-lcscroll+sbar[0]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[1]",glm::vec2(0,-30*(mm>2)-lcscroll+sbar[1]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(0,-80*(mm>2)-lcscroll+sbar[2]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(0,15*(mm>2)-lcscroll+sbar[3]+(rand()%10-5)));
	title_fb.bind();m_frame->clear(0,0,0);glDrawArrays(GL_TRIANGLES,6,6);title_fb.close();
	sshd.upload_vec2("idx_mod[0]",glm::vec2(300*dtrans,0));
	sshd.upload_vec2("idx_mod[1]",glm::vec2(SELTRANS[(mselect-1)*2]*(1-dtrans)+SELTRANS[0]*dtrans,0));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(SELTRANS[(mselect-1)*2+1]*(1-dtrans)+SELTRANS[1]*dtrans,0));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(325*dtrans,0));
	select_fb.bind();m_frame->clear(0,0,0);glDrawArrays(GL_TRIANGLES,12,6);select_fb.close();
	diffsel = lselect;

	// render menu
	fb.bind();m_frame->clear(0,0,0);
	m_r2d->prepare();
	m_r2d->s2d.upload_float("ptrans",ptrans);
	m_r2d->sl.at(msindex).model=pos_title;m_r2d->sl.at(msindex+1).model=pos_entitle;
	m_r2d->render_sprite(msindex,msindex+2);
	m_r2d->reset_shader();
	m_r2d->render_sprite(msindex+2,msindex+16*(mm>0));
	tft.prepare();tft.render(50*(1-ptrans),glm::vec4(1,0,0,1));
	vtft.prepare();vtft.render(75,glm::vec4(0,0,.5f,1));
	ml_stages.render(dtrans*(mselect==4),lscroll);
	ml_mopt.render(dtrans*(mselect==3),lscroll);
	fb.close();m_frame->clear(0,0,0);
	fb.render_wOverlay(splash_fb.get_tex(),title_fb.get_tex(),select_fb.get_tex(),ptrans);
	m_r2d->prepare();m_r2d->render_sprite(msindex+16,msindex+20);
}
