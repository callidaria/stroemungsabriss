#include "menu.h"

/*
	constructor(CCBManager*,Frame*,Renderer2D*,Renderer3D*,RendererI*,Camera2D*,Camera3D*)
	ccbm: cascabel manager used to load environment and level loader files
	f: pointer to render window
	r2d: 2D renderer for sprites, states and animations
	r3d: 3D renderer for meshes and futurely collada
	rI: instance renderer for particles, bullet systems noise objects
	cam2d: camera to draw all objects of [r2d] and [rI] in relation to
	cam3d: camera to draw all objects of [r3d] in relation to
	purpose: setup menu environment, populate with menu lists and define input possibilities
*/
Menu::Menu(CCBManager* ccbm,Frame* f,Renderer2D* r2d,Renderer3D* r3d,RendererI* rI,
		Camera2D* cam2d,Camera3D* cam3d)
	: m_ccbm(ccbm),m_frame(f),m_r2d(r2d),m_r3d(r3d),m_rI(rI),m_cam2d(cam2d),m_cam3d(cam3d)
{
	// interpret level loader file
	msindex = ccbm->add_lv("lvload/menu.ccb");

	// add globe and pointer object
	ridx_terra = m_r3d->add("./res/terra.obj","./res/terra/albedo.jpg","./res/terra/spec.png",
			"./res/terra/norm.png","./res/none.png",glm::vec3(0,0,0),1.0f,glm::vec3(0,0,0));
	m_r3d->add("./res/selection.obj","./res/fast_bullet.png",":/res/none.png","./res/dnormal.png",
			"./res/none.png",glm::vec3(0,0,1),.015f,glm::vec3(120,0,0));
	m_r3d->load(m_cam3d);

	// shader materials and light for globe map preview
	Light3D l3d = Light3D(m_r3d,0,glm::vec3(-500,750,100),glm::vec3(1,1,1),1);
	l3d.upload();
	l3d.set_amnt(1);
	mat0 = Material3D(r3d,1,8,16);

	// setup dare message on idle screen
	tft = Text(&fnt);
	tft.add("press START if you DARE",glm::vec2(450,250));
	tft.load(cam2d);

	// print title version message
	Font vfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	vtft = Text(&vfnt);
	std::string title_vmessage = "yomisensei by callidaria. danmaku version "
			+std::to_string(GVERSION_RELEASE)+'.'+std::to_string(GVERSION_SUBRELEASE)+'.'
			+std::to_string(GVERSION_DEVSTEP)+GVERSION_SUFFIX+" - running on cascabel "
			+"1.3.1vE (OpenGL)";
	vtft.add(title_vmessage.c_str(),glm::vec2(600,20));
	vtft.load(cam2d);

	// submenu lists to interact with and choose from
	mls[0] = MenuList();
	mls[1] = MenuList(r2d,cam2d,"lvload/ml_mopt");
	mls[2] = MenuList(r2d,cam2d,"lvload/ml_stages");
	mls[3] = MenuList(r2d,cam2d,"lvload/ml_arcades");
	mls[4] = MenuList();
	mls[5] = MenuList();
	mls[6] = MenuList();
	mls[7] = MenuList(r2d,cam2d,"lvload/ml_optfrm");
	mls[8] = MenuList(r2d,cam2d,"lvload/ml_optaud");
	mls[9] = MenuList(r2d,cam2d,"lvload/ml_optgfx");
	mls[10] = MenuList(r2d,cam2d,"lvload/ml_optgm");
	mls[11] = MenuList(r2d,cam2d,"lvload/ml_optext");
	// FIXME: mess

	/*
		setup splash vertices by origin position, target position and colour:
		pattern: origin_position_x,origin_position_y,target_position_x,target_position_y,
				vertex_colour_r,vertex_colour_g,vertex_colour_b,vertex_edge_id
	*/
	float sverts[] = {

		// title splash
		-25,0,25,0,.5f,0,0,0, 420,720,-25,720,.5f,0,0,1, 600,720,25,720,.5f,0,0,2,
		600,720,25,720,.5f,0,0,2, 50,0,160,0,.5f,0,0,3, -25,0,25,0,.5f,0,0,0,

		// head splash
		0,500,0,500,h_rgb.x,h_rgb.y,h_rgb.z,0, 0,500,0,550,h_rgb.x,h_rgb.y,h_rgb.z,1,
		0,500,1280,600,h_rgb.x,h_rgb.y,h_rgb.z,2, 0,500,1280,600,h_rgb.x,h_rgb.y,h_rgb.z,2,
		0,500,1280,470,h_rgb.x,h_rgb.y,h_rgb.z,3, 0,500,0,500,h_rgb.x,h_rgb.y,h_rgb.z,0,

		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,4, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,5,
		1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,6, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,6,
		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,7, 1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,4,

		// select splash
		630,0,630,0,s_rgb.x,s_rgb.y,s_rgb.z,0, 630,0,0,720,s_rgb.x,s_rgb.y,s_rgb.z,1,
		650,0,0,720,s_rgb.x,s_rgb.y,s_rgb.z,2, 650,0,0,720,s_rgb.x,s_rgb.y,s_rgb.z,2,
		650,0,650,0,s_rgb.x,s_rgb.y,s_rgb.z,3, 630,0,630,0,s_rgb.x,s_rgb.y,s_rgb.z,0,

		// crossselect splash
		1280,500,1280,500,l_rgb.x,l_rgb.y,l_rgb.z,0, 1280,500,1280,550,l_rgb.x,l_rgb.y,l_rgb.z,1,
		1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,2, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,2,
		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,3, 1280,500,1280,500,l_rgb.x,l_rgb.y,l_rgb.z,0,

		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,3, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,2,
		1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,4, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,4,
		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,5, 1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,3,

		1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,5, 1280,500,1280,550,h_rgb.x,h_rgb.y,h_rgb.z,4,
		1280,500,1280,550,r_rgb.x,r_rgb.y,r_rgb.z,6, 1280,500,1280,550,r_rgb.x,r_rgb.y,r_rgb.z,6,
		1280,500,1280,500,r_rgb.x,r_rgb.y,r_rgb.z,7, 1280,500,1280,500,h_rgb.x,h_rgb.y,h_rgb.z,5
	};
	buffer.bind();
	buffer.upload_vertices(sverts,sizeof(sverts));
	// ??clockwise rotation triangle hardcoded replace
	// FIXME: mess
	// TODO: remove hardcoded secondary segment coords

	// compile shader, setup vertex attribute pattern and upload camera
	sshd.compile("shader/fbv_select.shader","shader/fbf_select.shader");
	sshd.def_attributeF("position",2,0,8);
	sshd.def_attributeF("dposition",2,2,8);
	sshd.def_attributeF("colour",3,4,8);
	sshd.def_attributeF("idx",1,7,8);
	sshd.upload_camera(*cam2d);

	// framebuffer creation
	fb = FrameBuffer(f->w_res,f->h_res,"shader/fbv_menu.shader","shader/fbf_menu.shader",false);
	globe_fb = FrameBuffer(f->w_res,f->h_res,f->w_res/4,f->h_res/4,"shader/fbv_standard.shader",
			"shader/fbf_standard.shader",false);

	// create msaa effect for selection splash
	msaa = MSAA("shader/fbv_splash.shader","shader/fbf_splash.shader",f->w_res,f->h_res,8);

	// minimize difficulty choice banners
	for (int i=0;i<4;i++) m_r2d->sl.at(msindex+14+i).scale_arbit(1,0);

	// setup user input
	if (f->m_gc.size()>0) {
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
		cam_up = &f->kb.ka[SDL_SCANCODE_I];
		cam_down = &f->kb.ka[SDL_SCANCODE_K];
		cam_left = &f->kb.ka[SDL_SCANCODE_J];
		cam_right = &f->kb.ka[SDL_SCANCODE_L];
	}  // TODO: include all axis and common intuitive input systems
} Menu::~Menu() {  }

/*
	render(uint32_t&)
	running: is application still running, and if so which lvindex?
	reboot: will be rebooting with new settings after game closed?
	purpose: render the main menu, calculate geometry and process interactions
*/
void Menu::render(uint32_t &running,bool &reboot)
{
	// process input triggers
	bool hit_a=*cnt_start&&!trg_start,hit_b=*cnt_b&&!trg_b;			// face buttons select & cancel
	bool hit_lft=*cnt_lft&&!trg_lft,hit_rgt=*cnt_rgt&&!trg_rgt;		// dpad directions horizontal

	// process sublist selections
	uint8_t i_ml = mselect-2+opt_index;						// calculate menu list index
	if (edge_sel&&hit_a) mls[i_ml].write_tempID(lselect);	// write selected choice from sublist
	uint8_t dlgmod = hit_rgt-hit_lft;						// calculate delta of dialogue selection
	// FIXME: branch in this cluster

	// stall user inputs when dialogue active
	std::vector<bool*> stall_trg = { &trg_start,&trg_b,&trg_up,&trg_dwn,&trg_lft,&trg_rgt };
	uint8_t diff_can = md_diff.stall_input(stall_trg,cnt_start,cnt_b);
	uint8_t conf_can = md_conf.stall_input(stall_trg,cnt_start,cnt_b);

	// read difficulty dialogue choice
	if (diff_can==1) mm = MenuMode::MENU_LISTING;	// close dialogue
	else if (diff_can>1) difflv = diff_can-1;		// choice or idle
	// FIXME: branch in main loop

	// read keep settings dialogue choice
	if (conf_can==1||(conf_can==2&&dsi_conf==2)) {	// close dialogue and reset
		for (int i=7;i<11;i++) mls[i].reset();
		mm = MenuMode::MENU_SELECTION;
	} else if (conf_can==2) {						// save changes to settings and restart

		// kill existing config file
		std::ofstream kill("config.ini");
		kill.close();

		// save changes to settings
		for (int i=7;i<11;i++) mls[i].save();

		// immediately restart program with new settings
		reboot = true;
		running = 0;
		return;
	}  // FIXME: branch in main loop

	// relevant variables for switch
	bool is_shift,changed;
	uint8_t tmm;
	int ml_delta = 0;
	uint8_t thrzt,tvrtt;
	int8_t mv_dlta = 0;

	// menu mode change switch
	switch (mm) {
	case MenuMode::MENU_TITLE:

		// waiting until start input
		mm = (MenuMode)(MenuMode::MENU_SELECTION*(*cnt_start&&!trg_start));
		break;

	case MenuMode::MENU_SELECTION:

		// process state changes
		tmm = 1;
		tmm += 3*(*cnt_start&&!trg_start);					// j listing
		tmm -= 2*(*cnt_start&&!trg_start&&mselect==7);		// j start
		tmm = tmm*!(*cnt_b&&!trg_b);						// j title
		running = !(*cnt_start&&!trg_start&&mselect==2);	// exit
		mm = (MenuMode)tmm;

		// reset list scrolling and selection
		lselect = 0;
		lscroll = 0; // FIXME: reduce one set call. idc which one

		// calculate title destinations if shifting
		is_shift = (tmm<5&&tmm>2)||(dtrans>.01f);
		mve = (glm::vec2(360-(m_r2d->sl[msindex+mselect*2-4].sclx/2),650)
			-m_r2d->sl[msindex+mselect*2-4].pos)*glm::vec2(is_shift);
		mvj = (glm::vec2(50,50)-m_r2d->sl[msindex+mselect*2-3].pos)*glm::vec2(is_shift);

		// update horizontal menu selection marker
		mv_dlta = *cnt_rgt*(mselect<8&&!trg_rgt)-*cnt_lft*(mselect>2&&!trg_lft);
		mselect += mv_dlta;

		// temporarily save title language index
		thrzt = hrz_title;
		tvrtt = vrt_title;

		// switch menu title languages
		hrz_title = rand()%4*(!!mv_dlta)+hrz_title*(!mv_dlta);
		hrz_title = (hrz_title+(hrz_title==thrzt&&!!mv_dlta))%4;
		vrt_title = rand()%4*(!!mv_dlta)+vrt_title*(!mv_dlta);
		vrt_title = (vrt_title+(vrt_title==tvrtt&&!!mv_dlta))%4;
		break;

	case MenuMode::MENU_DIFFS:  // TODO: remove this state and control through keystalling

		// process state changes
		tmm = 3;
		tmm -= difflv>0;//*cnt_start&&!trg_start;
		tmm += *cnt_b&&!trg_b;
		mm = (MenuMode)tmm;

		// open difficulty dialogue
		md_diff.open_dialogue();
		break;

	case MenuMode::MENU_LISTING:

		// process state changes
		tmm = 4;
		tmm += *cnt_start&&!trg_start&&mselect==3;
		tmm -= *cnt_start&&!trg_start&&mselect!=3;

		// check for changes in settings and open confirm dialogue
		changed = false;
		for (int i=7;i<11;i++) changed = mls[i].was_changed()||changed;
		if (*cnt_b&&!trg_b&&mselect==3&&changed) md_conf.open_dialogue();
		else tmm -= 3*(*cnt_b&&!trg_b);
		mm = (MenuMode)tmm;
		// FIXME: maybe a little heavy

		// update vertical menu list selection marker
		opt_index = (6+lselect)*(tmm==5);
		lselect += (*cnt_dwn&&!trg_dwn&&lselect<(mls[mselect-2].esize-1))
				-(*cnt_up&&!trg_up&&lselect>0);
		lselect *= tmm!=5;
		break;

	case MenuMode::MENU_SUBLIST:

		// process state changes
		tmm = 5;
		tmm -= *cnt_b&&!trg_b&&!edge_sel;
		mm = (MenuMode)tmm;

		// stall inputs when sublist should exist
		lselect += (*cnt_dwn&&!trg_dwn&&lselect<(mls[mselect-2+opt_index].esize-1)&&!edge_sel)
			-(*cnt_up&&!trg_up&&lselect>0&&!edge_sel);
		opt_index *= tmm==5;  // FIXME: doubled logical can be broken down in MENU_LISTING
		lselect *= tmm!=4;
		// FIXME: reduce to one increment calculation

		// calculate sublist selection
		edge_sel = !edge_sel*(hit_a||(edge_sel&&hit_b))+edge_sel*(!hit_a&&!(edge_sel&&hit_b));
		ml_delta = edge_sel*((*cnt_dwn&&!trg_dwn)-(*cnt_up&&!trg_up));
		ml_delta = edge_sel*ml_delta+!edge_sel*(*cnt_rgt-*cnt_lft);
		break;

	// run game at given position
	default:
		running = lselect;
		game.run(running,m_ccbm);
	}
	// FIXME: break branch with static function pointer list
	// FIXME: a button naming as cnt_start reference is off and should be changed
	// TODO: reduce menu list input to a movement process in menu list class with stalls

	// set triggers
	trg_start=*cnt_start;trg_b=*cnt_b;									// face button triggers
	trg_lft=*cnt_lft;trg_rgt=*cnt_rgt;trg_dwn=*cnt_dwn;trg_up=*cnt_up;	// dpad direction triggers

	// move non-used out of view
	for (int i=1;i<8;i++) {  // FIXME: i will regret this tomorrow ...just a test
		float tval = m_r2d->sl.at(msindex+i*2-1).pos.x+250;
		m_r2d->sl.at(msindex+i*2-1).model
				= glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(-tval,0,0));
		tval = m_r2d->sl.at(msindex+i*2-2).pos.x+250;
		m_r2d->sl.at(msindex+i*2-2).model
				= glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(-tval,0,0));
	}

	// animate movement within title positions
	ptrans += .1f*(mm==1&&ptrans<1);
	ptrans -= .1f*(mm==0&&ptrans>.01f); // !!use an epsilon, pretty please
	dtrans += .1f*(mm>2&&dtrans<1);
	dtrans -= .1f*(mm<2&&dtrans>.01f); // FIXME: reduce this
	m_r2d->sl.at(msindex+mselect*2-4).model
			= glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(mve.x,mve.y,0));
	m_r2d->sl.at(msindex+mselect*2-3).model
			= glm::translate(glm::mat4(1.0f),dtrans*glm::vec3(mvj.x,mvj.y,0));
	pos_title = glm::translate(glm::mat4(1.0f),
			TITLE_START+title_dir*ptrans+dtrans*glm::vec3(-140,0,0));
	pos_entitle = glm::translate(glm::mat4(1.0f),ENTITLE_START+entitle_dir*(ptrans-dtrans));

	// menu list selection and scrolling
	lscroll += (lselect>lbounds)-(lselect<(lbounds-7));
	bool scddiff = lselect>lbounds;
	bool scudiff = lselect<(lbounds-7);
	lbounds = (!scddiff&&!scudiff)*lbounds+scddiff*lselect+scudiff*(lselect+7);
	int lcscroll = 45*(lselect+7-lbounds);
	for (int i=0;i<8;i++) sbar[i] = (diffsel!=lselect)*(rand()%30-15)+(diffsel==lselect)*sbar[i];
	// FIXME: kill extra scrolling calculations

	// setup splashes
	sshd.enable();
	buffer.bind();

	// reset vertex modifications in shader program
	sshd.upload_float("ptrans",ptrans);
	sshd.upload_vec2("idx_mod[0]",glm::vec2(0));
	sshd.upload_vec2("idx_mod[1]",glm::vec2(0));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(0));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(0));

	// start splash msaa
	msaa.bind();
	m_frame->clear(0,0,0);

	// render title splash to framebuffer
	glDrawArrays(GL_TRIANGLES,0,6);

	// upload vertical selection scroll
	sshd.upload_vec2("idx_mod[0]",glm::vec2(300*dtrans,0));
	sshd.upload_vec2("idx_mod[1]",
			glm::vec2(SELTRANS[(mselect-1)*2]*(1-dtrans)*ptrans+SELTRANS[0]*dtrans,0));
	sshd.upload_vec2("idx_mod[2]",
			glm::vec2(SELTRANS[(mselect-1)*2+1]*(1-dtrans)*ptrans+SELTRANS[1]*dtrans,0));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(325*dtrans,0));

	// render vertical selection splash to framebuffer
	glDrawArrays(GL_TRIANGLES,18,6);

	// calculate selection splash vibration and presets
	int rnd_edge[4];
	for (int i=0;i<4;i++) rnd_edge[i] = (rand()%10-5);
	int32_t xscr0 = -700,xscr1 = -400;

	// upload horizontal selection scroll
	sshd.upload_vec2("idx_mod[0]",glm::vec2(0,-15*(mm>2)-lcscroll+sbar[0]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[1]",glm::vec2(0,-40*(mm>2)-lcscroll+sbar[1]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(xscr0*(mm==5),-90*(mm>2)-lcscroll+sbar[2]+rnd_edge[0]));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(xscr0*(mm==5),15*(mm>2)-lcscroll+sbar[3]+rnd_edge[1]));
	sshd.upload_vec2("idx_mod[4]",glm::vec2(xscr1*(mm==5),-15*(mm>2)-lcscroll+sbar[4]+rnd_edge[2]));
	sshd.upload_vec2("idx_mod[5]",glm::vec2(xscr1*(mm==5),-40*(mm>2)-lcscroll+sbar[5]+rnd_edge[3]));
	sshd.upload_vec2("idx_mod[6]",glm::vec2(1280,-40*(mm>2)-lcscroll+sbar[6]+(rand()%10-5)));
	sshd.upload_vec2("idx_mod[7]",glm::vec2(1280,-15*(mm>2)-lcscroll+sbar[7]+(rand()%10-5)));

	// render horizontal selection splash to framebuffer
	glDrawArrays(GL_TRIANGLES,6,12);

	// calculate the list splash edges for prism list selection
	bool af = edge_mod==-1;
	int32_t xsll = 5+290*edge_mod,xslr = -5-290*(1-edge_mod);  // ??reset and immediate usage
	int32_t yslu = (md_disp)*edge_sel+12*(!!edge_sel),ysld = -(md_disp)*edge_sel-12*(!!edge_sel);

	// upload prism list edge modifications
	sshd.upload_vec2("idx_mod[0]",glm::vec2(xscr0,-15-lcscroll+sbar[3]+rnd_edge[1]));
	sshd.upload_vec2("idx_mod[1]",glm::vec2(xscr0,-40-lcscroll+sbar[2]+rnd_edge[0]));
	sshd.upload_vec2("idx_mod[2]",glm::vec2(xscr0+xsll*!af+yslu*.1f,
			-40-lcscroll+(sbar[2]+rnd_edge[0])*af-10*!af+yslu));
	sshd.upload_vec2("idx_mod[3]",glm::vec2(xscr0+xsll*!af+yslu*.1f,
			-15-lcscroll+(sbar[3]+rnd_edge[1])*af+10*!af+ysld));
	sshd.upload_vec2("idx_mod[4]",glm::vec2(xscr1+xslr*!af-yslu*.1f,
			-40-lcscroll+(sbar[5]+rnd_edge[3])*af-10*!af+yslu));
	sshd.upload_vec2("idx_mod[5]",glm::vec2(xscr1+xslr*!af-yslu*.1f,
			-15-lcscroll+(sbar[4]+rnd_edge[2])*af+10*!af+ysld));
	sshd.upload_vec2("idx_mod[6]",glm::vec2(xscr1,-40-lcscroll+sbar[5]+rnd_edge[3]));
	sshd.upload_vec2("idx_mod[7]",glm::vec2(xscr1,-15-lcscroll+sbar[4]+rnd_edge[2]));

	// render prism list viewtype to framebuffer
	glDrawArrays(GL_TRIANGLES,24,18*(mm==5));

	// blit msaa buffers
	msaa.blit();

	// deltasave lselect to compare list selection modification next frame
	diffsel = lselect;

	// render menu
	fb.bind();
	m_frame->clear(0,0,0);
	m_r2d->prepare();
	m_r2d->s2d.upload_float("ptrans",ptrans);

	// transform vertical titles
	glm::mat4 tr_model = glm::scale(pos_title,glm::vec3(val_vscl,val_vscl,0));
	tr_model = glm::rotate(tr_model,glm::radians(val_vrot),glm::vec3(0,0,1));
	m_r2d->al[0].model = tr_model;

	// transform horizontal title
	tr_model = glm::scale(pos_entitle,glm::vec3(val_hscl,val_hscl,0));
	tr_model = glm::rotate(tr_model,glm::radians(val_hrot),glm::vec3(0,0,1));
	m_r2d->al[1].model = tr_model;

	// render titles
	m_r2d->render_state(0,glm::vec2(vrt_title,0));	// vertical
	m_r2d->render_state(1,glm::vec2(0,hrz_title));	// horizontal

	// animate title speedup effect
	neg_vscl += (val_vscl>1.1f&&!neg_vscl)-(val_vscl<.9f&&neg_vscl);
	val_vscl += ((rand()%6+6)*.0001f)*(1-2*neg_vscl);	// vertical scale anim
	neg_hscl += (val_hscl>1.1f&&!neg_hscl)-(val_hscl<.9f&&neg_hscl);
	val_hscl += ((rand()%6+6)*.0001f)*(1-2*neg_hscl);	// horizontal scale anim
	neg_vrot += (val_vrot>2&&!neg_vrot)-(val_vrot<-2&&neg_vrot);
	val_vrot += ((rand()%5+10)*.001f)*(1-2*neg_vrot);	// vertical rotation
	neg_hrot += (val_hrot>2&&!neg_hrot)-(val_hrot<-2&&neg_hrot);
	val_hrot += ((rand()%5+10)*.001f)*(1-2*neg_hrot);	// horizontal rotation
	// FIXME: breakdown the rhythm

	// draw horizontal menu options
	m_r2d->reset_shader();
	m_r2d->render_sprite(msindex,msindex+14*(mm>0));

	// write dare message
	tft.prepare();
	tft.render(19*(1-ptrans),glm::vec4(1,0,0,1));

	// write version notification printout
	vtft.prepare();
	vtft.render(100,glm::vec4(0,0,.5f,1));
	fb.close();

	// prepare globe preview draw
	globe_fb.bind();
	m_frame->clear(.1f,.1f,.1f);

	// rotate camera towards globe
	m_cam3d->front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
	m_cam3d->front.y = sin(glm::radians(pitch));
	m_cam3d->front.z = cos(glm::radians(pitch))*sin(glm::radians(yaw));
	m_cam3d->front = glm::normalize(m_cam3d->front);
	m_r3d->prepare(m_cam3d);
	// FIXME: dont update static camera constantly. either animate or do this outside loop

	// calculate globe rotation towards preview location
	glm::vec2 gRot = mls[i_ml].globe_rotation(lselect);
	glm::mat4 model = glm::rotate(glm::mat4(1.0f),glm::radians(gRot.x),glm::vec3(1,0,0));
	model = glm::rotate(model,glm::radians(gRot.y),glm::vec3(0,-1,0));
	m_r3d->s3d.upload_matrix("model",model);

	// render globe preview to framebuffer
	mat0.upload();
	m_r3d->render_mesh(ridx_terra,ridx_terra+1);
	m_r3d->s3d.upload_matrix("model",glm::mat4(1.0f));
	m_r3d->render_mesh(ridx_terra+1,ridx_terra+2);
	globe_fb.close();

	// render combined splash overlay
	m_frame->clear(0,0,0);
	fb.render(ptrans);

	// anti aliasing for selection splashes
	msaa.render(fb.get_tex());

	// render menu lists and sublists
	mls[i_ml].render(dtrans,lscroll,lselect,edge_mod,ml_delta,edge_sel,md_disp);

	// animate and move dialogue selector to dialogue choice
	m_r2d->prepare();
	bool dopen = dsi_diff||dsi_conf;
	glm::vec3 disp = glm::vec3(235,135,0)*glm::vec3(dsi_diff>0)+glm::vec3(257,285,0)*glm::vec3(dsi_conf>0);
	disp.x += (dsi_diff-(dsi_diff>0))*220+(dsi_conf-(dsi_conf>0))*125;
	glm::mat4 disptrans = glm::translate(glm::mat4(1.0f),disp+glm::vec3(-5,15,0));
	glm::mat4 disprot = glm::rotate(glm::mat4(1.0f),glm::radians(dlgrot_cnt),glm::vec3(0,0,1));
	m_r2d->sl[msindex+18].model = disptrans*disprot;

	// render menu dialogue base selector behind dialogue elements
	m_r2d->render_sprite(msindex+18,msindex+19*dopen);
	dlgrot_cnt -= 7-(dlgrot_cnt<-360)*360;

	// apply dialog selection changes
	dsi_diff += dlgmod;
	dsi_conf += dlgmod;

	// render menu dialogue overlays
	md_diff.render(dsi_diff);
	md_conf.render(dsi_conf);

	// animate and move menu dialogue focused selector
	disptrans = glm::translate(glm::mat4(1.0f),disp);
	disprot = glm::rotate(glm::mat4(1.0f),glm::radians(dlgrot_val),glm::vec3(0,0,1));
	m_r2d->sl[msindex+19].model = disptrans*disprot;

	// render menu dialogue focused selector atop dialogue elements
	m_r2d->render_sprite(msindex+19,msindex+20*dopen);
	dlgrot_val += 2-(dlgrot_val>360)*360;

	// animate globe location preview screen
	m_r2d->sl.at(msindex+20).scale_arbit(1,dtrans);
	m_r2d->sl.at(msindex+20).translate(glm::vec2(0,90*(1.0f-dtrans)+450*(lcscroll>180)));
	m_r2d->s2d.upload_float("vFlip",1.0f);
	// FIXME: this bullshit no reason bullshit stupid ass vectorflip

	// render globe location preview framebuffer
	m_r2d->render_sprite(msindex+20,msindex+21*(mselect==4||mselect==5),globe_fb.get_tex());
	m_r2d->s2d.upload_float("vFlip",0);
}