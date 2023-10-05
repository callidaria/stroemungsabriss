#include "main_menu.h"

/**
 * 		Menu List Implementation with .ldc Language Interpreter
 * TODO: expand this segment documentation
 * 
 * 
 * 		List Definition Code Language Design
 * prefix ':' marks a definition statement in ldc, the following expression until ' ' or '\n'
 * following expression are valid and will be handled by menu list interpreter:
 * (the definitions are using <> to show where custom naming replaces "<contents>")
 * 
 * 
 * 		GENERAL LANGUAGE FEATURES
 * 
 * 		:cluster <cluster_name>
 * this statement marks the definition of a list, sublist or list entity collection
 * any cluster can be referred to by name within the list definition file
 * to define the main list use :cluster main
 * 
 * 		:logic <condition_list_size>
 * setup of extern condition logic list to change viewability of entities with :condition command
 * 
 * 		:define <entity_name>
 * define the name of the list entity, that will be shown alongside other list member entities
 * 
 * 		:describe
 * 		<description>
 * to add a description to the list entity, that will be shown when currently selected
 * 
 * 		:segment <segment_name>
 * whereever a segment is called within the list defintion file, a dividing, stylized line will be
 * drawn between the imperatively previous and following definition
 * 
 * 		:condition <condition_id>
 * set a condition to activate list entity.
 * the condition id will read the respective boolean from an extern condition list.
 * 
 * 
 * 		BEHAVIOUR DEFINITIONS
 * (ideally choose one of the following commands per :define)
 * 
 * 		:subsequent <cluster_name>
 * confirming selection on this entity will load a sublist linked by it's cluster
 * the list will then be shown under the heading of this list entities :define
 * 
 * 		:checkbox
 * create a simple checkbox for this entity, used to choose between true or false states
 * 
 * 		:dropdown
 * 		<fist_option>;<second_option>;<third_option>
 * add a dropdown option list to this entity, holding selectable options divided by a ';'
 * using :dropdown without defining options will still create a dropdown and the list can be
 * assembled dynamically
 * 
 * 		:slider
 * make this entity contain a horizontally adjustable slider
 * 
 * 		:return <output_value>
 * selecting this entity stops list interaction immediately and returns given value
 * 
 * 
 * The the following implementation is the MenuList constructor, holding the language interpreter
 * conforming to the above language definitions and statements.
*/

/*
	TODO
*/
MenuList::MenuList(const char* path)
	: fpath(path)
{
	// logic switch overhead
	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
		command_logic_cluster,command_logic_logiclist,command_logic_define,command_logic_describe,
		command_logic_segment,command_logic_condition,command_logic_subsequent,
		command_logic_checkbox,command_logic_dropdown,command_logic_slider,
		command_logic_return,command_logic_syntax_error
	};

	// iterate definition file
	std::ifstream file(path,std::ios::in);
	while (!file.eof()) {

		// skip until command
		std::string line;
		getline(file,line);
		if (!line.rfind(':',0)) {

			// translate command to handling logic address & store in command buffer
			ListLanguageCommand llc;
			size_t cmd_split = line.find(' ');
			while (llc.id<LIST_LANGUAGE_COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id])
				llc.id++;

			// add command to command buffer & handle command attributes
			llc.tail = line.erase(0,cmd_split+1);
			cmd_buffer.push_back(llc);
		} else cmd_buffer.back().buffer += line+' ';
	} file.close();

	// execute commands
	for (auto cmd : cmd_buffer) {
		interpreter_behaviour[cmd.id](*this,cmd);
		cline++;
	}

	// post-process list & visuals creation
	for (MenuListCluster &cluster : clusters) {
		int32_t vscroll = MENU_LIST_SCROLL_START;
		for (uint16_t i=0;i<cluster.elist.size();i++) {

			// create list segments by id and increase list text scroll
			// TODO

			// convert cluster name references to cluster id
			// TODO

			// create list text
			Text etext = Text(st_font);
			etext.add(cluster.elist[i].head.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll));
			etext.load();
			cluster.tx_list.push_back(etext);
			vscroll -= MENU_LIST_SCROLL_Y;
		}
	}
}

/*
	TODO
*/
void MenuList::update()
{
	for (auto txe : clusters[active_cluster_id].tx_list)
		txe.prepare(),txe.render(128,glm::vec4(1));
}

/*
	TODO
*/
void command_logic_cluster(MenuList &ml,const ListLanguageCommand &cmd)
{
	MenuListCluster cluster;
	cluster.id = cmd.tail;
	ml.clusters.push_back(cluster);
}

/*
	TODO
*/
void command_logic_logiclist(MenuList &ml,const ListLanguageCommand &cmd)
{ ml.condition_list = std::vector<bool>(stoi(cmd.tail)); }

/*
	TODO
*/
void command_logic_define(MenuList &ml,const ListLanguageCommand &cmd)
{
	MenuListEntity entity;
	entity.head = cmd.tail;
	ml.clusters.back().elist.push_back(entity);
}

/*
	TODO
*/
void command_logic_describe(MenuList &ml,const ListLanguageCommand &cmd)
{ ml.clusters.back().elist.back().description = cmd.buffer; }

/*
	TODO
*/
void command_logic_segment(MenuList &ml,const ListLanguageCommand &cmd)
{
	MenuListSegment segment;
	segment.position = ml.clusters.back().elist.size();
	segment.title = cmd.tail;
	ml.clusters.back().slist.push_back(segment);
}

/*
	TODO
*/
void command_logic_condition(MenuList &ml,const ListLanguageCommand &cmd)
{ ml.clusters.back().elist.back().condition_id = stoi(cmd.tail); }

/*
	TODO
*/
void command_logic_subsequent(MenuList &ml,const ListLanguageCommand &cmd)
{
	ml.clusters.back().elist.back().child_name = cmd.tail;
	ml.clusters.back().elist.back().etype = LIST_ENTITY_TYPE_PARENT;
}

/*
	TODO
*/
void command_logic_checkbox(MenuList &ml,const ListLanguageCommand &cmd)
{ ml.clusters.back().elist.back().etype = LIST_ENTITY_TYPE_CHECKBOX; }

/*
	TODO
*/
void command_logic_dropdown(MenuList &ml,const ListLanguageCommand &cmd)
{
	std::stringstream bfss(cmd.buffer);
	std::string ddoption;
	while (getline(bfss,ddoption,';')) 
		ml.clusters.back().elist.back().dropdown_options.push_back(ddoption);
	ml.clusters.back().elist.back().etype = LIST_ENTITY_TYPE_DROPDOWN;
}

/*
	TODO
*/
void command_logic_slider(MenuList &ml,const ListLanguageCommand &cmd)
{ ml.clusters.back().elist.back().etype = LIST_ENTITY_TYPE_SLIDER; }

/*
	TODO
*/
void command_logic_return(MenuList &ml,const ListLanguageCommand &cmd)
{
	ml.clusters.back().elist.back().rval = stoi(cmd.tail);
	ml.clusters.back().elist.back().etype = LIST_ENTITY_TYPE_RETURN;
}

/*
	TODO
*/
void command_logic_syntax_error(MenuList &ml,const ListLanguageCommand &cmd)
{ printf("\033[1;31msyntax error in %s:%i menu list definition code\033[0m\n",ml.fpath,ml.cline); }


/**
 * 		The real menu implementation starts here!
 * TODO: expand this segment documentation
*/

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
	create_splash(sverts,glm::vec2(SPLICE_TITLE_LOWER_START,0),
			glm::vec2(SPLICE_TITLE_UPPER_START,720),glm::vec3(.5f,0,0));
	create_splash(sverts,glm::vec2(0,SPLICE_HEAD_ORIGIN_POSITION),
			glm::vec2(1280,SPLICE_HEAD_ORIGIN_POSITION),SPLICE_HEAD_COLOUR);
	create_splash(sverts,glm::vec2(0),glm::vec2(0,720),SPLICE_SELECTION_COLOUR);
	sh_buffer.bind();
	sh_buffer.upload_vertices(sverts);
	sh_shader.compile("./shader/main_menu/vsplash.shader","./shader/main_menu/fsplash.shader");
	sh_shader.def_attributeF("position",2,0,SPLICE_VERTEX_FLOAT_COUNT);
	sh_shader.def_attributeF("colour",3,2,SPLICE_VERTEX_FLOAT_COUNT);
	sh_shader.def_attributeF("edge_id",1,5,SPLICE_VERTEX_FLOAT_COUNT);
	sh_shader.upload_camera(Camera2D(1280.f,720.f));

	// version annotation text setup
	std::string vmessage = "yomisensei by callidaria. danmaku v"
			+ std::to_string(INFO_VERSION_RELEASE)+'.'+std::to_string(INFO_VERSION_SUBRELEASE)+'.'
			+ std::to_string(INFO_VERSION_DEVSTEP)+INFO_VERSION_MODE_SUFFIX
			+ " - running on cascabel base (OpenGL)";
	tx_version.add(vmessage.c_str(),TEXT_VERSION_POSITION);
	tx_version.load();
	tcap_version = vmessage.length();

	// menu options text
	for (uint8_t i=0;i<MENU_MAIN_OPTION_COUNT;i++) {
		uint32_t wwidth = fnt_mopts.calc_wordwidth(main_options[i]);
		mo_twidth[i] = wwidth,mo_hwidth[i] = wwidth*.5f;
		mo_prog.x -= wwidth;
	} mo_prog /= glm::vec2(MENU_MAIN_OPTION_COUNT);
	glm::vec2 mo_cursor = MENU_OPTIONS_CLEFT+glm::vec2(mo_hwidth[0],0);
	for (uint8_t i=0;i<MENU_MAIN_OPTION_COUNT;i++) {
		tx_mopts[i].add(main_options[i],glm::vec2(-mo_hwidth[i],0)),tx_mopts[i].load();
		mo_cposition[i] = mo_cursor;
		mo_cursor += mo_prog+glm::vec2(mo_twidth[i],0);
	}

	// peripheral sensitive input request annotations
	update_peripheral_annotations();

	// buffers
	msaa = MSAA("./shader/fbv_standard.shader","./shader/fbf_standard.shader",
			m_ccbf->frame->w_res,m_ccbf->frame->h_res,8);
	fb_nslice = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/fbf_standard.shader");
	fb_menu = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_mainmenu.shader");
	fb_slice = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_splash.shader");
	fb_slice.s.upload_int("gbuffer_colour",0);
	fb_slice.s.upload_int("gbuffer_normals",1);
	fb_slice.s.upload_int("menu_fb",2);

	// setup logic collection
	interface_behaviour[INTERFACE_LOGIC_MACRO] = interface_behaviour_macro,
		interface_behaviour[INTERFACE_LOGIC_OPTIONS] = interface_behaviour_options,
		interface_behaviour[INTERFACE_LOGIC_EXTRAS] = interface_behaviour_extras,
		interface_behaviour[INTERFACE_LOGIC_PRACTICE] = interface_behaviour_extras,
		interface_behaviour[INTERFACE_LOGIC_LOAD] = interface_behaviour_extras,
		interface_behaviour[INTERFACE_LOGIC_CONTINUE] = interface_behaviour_extras,
		interface_behaviour[INTERFACE_LOGIC_NEWGAME] = interface_behaviour_extras;
	// TODO: a lot of performance and translation testing necessary to analyze the flipsides
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
	bool plmb = m_ccbf->frame->mouse.mb[0]&&!trg_lmb,prmb = m_ccbf->frame->mouse.mb[2]&&!trg_rmb;
	hit_a = (m_ccbf->iMap->get_input_triggered(IMP_REQPAUSE)&&!menu_action)
			|| m_ccbf->iMap->get_input_triggered(IMP_REQFOCUS)||plmb
			|| m_ccbf->iMap->get_input_triggered(IMP_REQCONFIRM),
		hit_b = (m_ccbf->iMap->get_input_triggered(IMP_REQPAUSE)&&menu_action)
			|| m_ccbf->iMap->get_input_triggered(IMP_REQBOMB)||prmb;
	lrmv = ((m_ccbf->iMap->get_input_triggered(IMP_REQRIGHT)&&vselect<MENU_MAIN_OPTION_CAP)
			- (m_ccbf->iMap->get_input_triggered(IMP_REQLEFT)&&vselect>0))*menu_action;
	trg_lmb = m_ccbf->frame->mouse.mb[0],trg_rmb = m_ccbf->frame->mouse.mb[2];
	// FIXME: as soon as the title screen has been passed, start press will become return request

	// timing
	bool anim_go = anim_timing>ANIMATION_UPDATE_TIMEOUT;
	anim_timing += m_ccbf->frame->time_delta;
	dt_tshiftdown += m_ccbf->frame->time_delta*speedup,
			dt_tnormalize += m_ccbf->frame->time_delta*!speedup;

	// focus transition
	ftransition += (interface_logic_id-!interface_logic_id)*TRANSITION_SPEED
			* m_ccbf->frame->time_delta;
	ftransition = (ftransition<.0f) ? .0f : (ftransition>1.f) ? 1.f : ftransition;
	// TODO: i feel like this implementation lacks wit
	// FIXME: transition to 1.f takes 2 frames while transition to .0f takes about 12?
	inv_ftransition = 1.f-ftransition;

	// title rattle animation
	uint8_t rattle_mobility = RATTLE_THRESHOLD+RATTLE_THRESHOLD_RAGEADDR*menu_action,
		rattle_countermove = rattle_mobility/2;
	glm::vec3 title_action = glm::vec3((rand()%rattle_mobility-rattle_countermove)*anim_go,
			(rand()%rattle_mobility-rattle_countermove)*anim_go,0);

	// title shiftdown animation
	dt_tshiftdown *= menu_action,dt_tnormalize *= menu_action,speedup = speedup||!menu_action;
	float tshift = 1.f+SHIFTDOWN_ZOOM_INCREASE*((speedup) ? sqrt(sin(dt_tshiftdown*MATH_OCTAPI))
			: 1.f-sqrt(dt_tnormalize));

	// combined title animation
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

	// START MULTISAMPLED RENDER
	msaa.bind();
	Frame::clear();

	// splash render
	sh_buffer.bind();
	sh_shader.enable();

	// head splash upload & render
	modify_splash(glm::vec2(0,SPLICE_HEAD_LOWER_START*inv_ftransition),
			glm::vec2(0,SPLICE_HEAD_UPPER_START*inv_ftransition),
			(SPLICE_HEAD_ORIGIN_WIDTH+SPLICE_HEAD_LOWER_WIDTH*inv_ftransition)*mtransition,
			(SPLICE_HEAD_ORIGIN_WIDTH+SPLICE_HEAD_UPPER_WIDTH*inv_ftransition)*mtransition,true);
	glDrawArrays(GL_TRIANGLES,6,6);

	// selection splash upload & render
	float ctransition = mtransition-ftransition;
	modify_splash(vrt_lpos,vrt_upos,vrt_lwidth*ctransition,vrt_uwidth*ctransition,false);
	glDrawArrays(GL_TRIANGLES,12,6);
	// FIXME: splash dimensions to prevent aesthetically unfortunate proportions

	// title splash upload & render
	float tlpos = SPLICE_TITLE_LOWER_MOD*mtransition,tupos = SPLICE_TITLE_UPPER_MOD*mtransition;
	float tlext = SPLICE_TITLE_LOWER_SWIDTH+SPLICE_TITLE_LWIDTH_MOD*mtransition,
		tuext = SPLICE_TITLE_UPPER_SWIDTH+SPLICE_TITLE_UWIDTH_MOD*mtransition;
	modify_splash(glm::vec2(tlpos,0),glm::vec2(tupos,0),tlext,tuext,false);
	glDrawArrays(GL_TRIANGLES,0,6);

	// START RENDER MENU BUFFER
	msaa.blit();
	fb_menu.bind();
	Frame::clear();

	// interface logic
	interface_behaviour[interface_logic_id](*this);
	running = !request_close;

	// render text
	tx_dare.prepare();
	tx_dare.set_scroll(glm::vec2(title_action));
	tx_dare.render(tcap_dare*inv_mtransition,TEXT_DARE_COLOUR);
	tx_version.prepare();
	tx_version.render(tcap_version*inv_mtransition,TEXT_VERSION_COLOUR);

	// render main options
	for (uint8_t i=0;i<MENU_MAIN_OPTION_COUNT;i++) {
		tx_mopts[i].prepare();
		glm::mat4 opt_trans = glm::translate(glm::mat4(1.f),
				glm::vec3(mo_cposition[i].x+mo_hwidth[i],mo_cposition[i].y+250*ftransition,0));
		glm::vec4 opt_colour = glm::vec4(.5f,1.f,.5f,mtransition);
		if (i==vselect) {
			opt_trans = glm::scale(opt_trans,glm::vec3(MENU_OPTIONS_SCALE_THRES));
			opt_trans = glm::rotate(opt_trans,st_rot,glm::vec3(0,0,1));
			opt_colour.z = 1.f;
		} tx_mopts[i].set_scroll(opt_trans);
		tx_mopts[i].render(strlen(main_options[i]),opt_colour);
	}

	// render titles
	m_ccbf->r2d->al[index_ranim+1].model = glm::translate(m_ccbf->r2d->al[index_ranim+1].model,
			glm::vec3(0,150*ftransition,0));
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(index_ranim,glm::vec2(3,0));
	m_ccbf->r2d->render_state(index_ranim+1,glm::vec2(0,0));

	// STOP RENDER MENU BUFFER
	FrameBuffer::close();

	// render menu
	fb_menu.render(mtransition);
	fb_slice.bind();
	Frame::clear();
	msaa.prepare();
	MSAA::render();
	FrameBuffer::close();

	// render overlay
	fb_slice.prepare();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,fb_nslice.tex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,fb_menu.tex);
	glActiveTexture(GL_TEXTURE0);
	fb_slice.s.upload_float("mtrans",mtransition);
	fb_slice.render();
	// FIXME: remove special treatment and transfer to a more controllable implementation

	// finishing
	bool shiftdown_over = dt_tshiftdown>TITLE_SHIFTDOWN_TIMEOUT,
			normalize_over = dt_tnormalize>TITLE_NORMALIZATION_TIMEOUT;
	anim_timing -= ANIMATION_UPDATE_TIMEOUT*anim_go;
	dt_tshiftdown -= TITLE_SHIFTDOWN_TIMEOUT*shiftdown_over,
			dt_tnormalize -= TITLE_NORMALIZATION_TIMEOUT*normalize_over;
	speedup = (speedup&&!shiftdown_over)||normalize_over;
}


/**
 * 		Menu Logic Functions
 * a collection of additional methods, helping with main menu logic
 * TODO: improve section documentation
*/

/*
	TODO
*/
uint8_t MainMenu::get_selected_main_option(float mx,bool &ch_select)
{
	float tsmx = mx*1280.f;
	uint8_t out_id = vselect;
	while (tsmx<(mo_cposition[out_id].x-mo_prog.x)&&out_id>0) out_id--;
	while (tsmx>(mo_cposition[out_id].x+mo_twidth[out_id]+mo_prog.x)&&out_id<MENU_MAIN_OPTION_CAP)
		out_id++;
	ch_select = ch_select||(out_id!=vselect);
	return out_id;
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


/**
 * 		Start Splash Logic
 * these methods simplify creation & modification of selection splash geometry
 * TODO: extend section documentation
*/

/*
	TODO
*/
void MainMenu::create_splash(std::vector<float> &sverts,glm::vec2 l,glm::vec2 u,glm::vec3 c)
{
	std::vector<float> verts = {
		l.x,l.y,c.x,c.y,c.z,0, u.x,u.y,c.x,c.y,c.z,3, u.x,u.y,c.x,c.y,c.z,2,
		u.x,u.y,c.x,c.y,c.z,3, l.x,l.y,c.x,c.y,c.z,0, l.x,l.y,c.x,c.y,c.z,1
	}; sverts.insert(sverts.end(),verts.begin(),verts.end());
}

/*
	TODO
	NOTE: selection shader has to be enabled before calling this function
*/
void MainMenu::modify_splash(glm::vec2 lp,glm::vec2 up,float le,float ue,bool hrz)
{
	sh_shader.upload_vec2("lupos[0]",lp),sh_shader.upload_vec2("lupos[1]",up);
	sh_shader.upload_float("luext[0]",le),sh_shader.upload_float("luext[1]",ue);
	sh_shader.upload_int("is_hrz",hrz);
}


/**
 * 		Start Implementation of Interface Behaviour
 * this section defines the static functions the renderer will point to for logic switching
 * TODO: expand documentation of this section
*/

/*
	TODO
*/
void interface_behaviour_macro(MainMenu &tm)
{
	// process macro selection
	bool action_request = tm.hit_a&&tm.menu_action;
	tm.request_close = (tm.vselect==MENU_MAIN_OPTION_EXIT)*action_request;
	tm.interface_logic_id = tm.vselect*action_request;

	// menu transition
	bool req_transition = tm.hit_a&&!tm.menu_action;
	tm.menu_action = (tm.menu_action||tm.hit_a)&&!tm.hit_b;
	tm.mtransition += (tm.menu_action-!tm.menu_action)*TRANSITION_SPEED*tm.m_ccbf->frame->time_delta;
	tm.mtransition = (tm.mtransition<.0f) ? .0f : (tm.mtransition>1.f) ? 1.f : tm.mtransition;
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/
	// TODO: compare linear transition with sinespeed transition implementation
	// 		also relate the results of this todo to the ftransition in main render method
	tm.inv_mtransition = 1.f-tm.mtransition;

	// processing selection input
	bool ch_select = tm.lrmv!=0;
	tm.vselect += tm.lrmv;
	tm.vselect = (tm.m_ccbf->frame->mpref_peripheral)
			? tm.get_selected_main_option(tm.m_ccbf->frame->mouse.mxfr,ch_select) : tm.vselect;

	// selection splash update calculations
	if (ch_select||req_transition) {

		// selector dimensions
		tm.vrt_lwidth = rand()%(uint16_t)tm.mo_hwidth[tm.vselect],
			tm.vrt_uwidth = rand()%(uint16_t)tm.mo_hwidth[tm.vselect];
		glm::vec2 vrt_cpos = tm.mo_cposition[tm.vselect]+glm::vec2(tm.mo_hwidth[tm.vselect],0);
		tm.vrt_lpos = glm::vec2((vrt_cpos.x-MENU_HALFSCREEN_UI)
				* SPLICE_OFFCENTER_MV+MENU_HALFSCREEN_UI,0);
		glm::vec2 vrt_dir = vrt_cpos-tm.vrt_lpos;
		float vrt_extend_II = (720.f-vrt_cpos.y)/vrt_dir.y;
		tm.vrt_upos = glm::vec2(vrt_cpos.x+vrt_dir.x*vrt_extend_II,0);

		// menu option text
		tm.st_rot = glm::radians((float)(rand()%MENU_OPTIONS_RDEG_THRES)*-((rand()%2)*2-1));
	}
}

/*
	TODO
*/
void interface_behaviour_options(MainMenu &tm)
{
	tm.ml_options.update();
	tm.interface_logic_id *= !tm.hit_b;
}

/*
	TODO
*/
void interface_behaviour_extras(MainMenu &tm)
{
	// TODO
	tm.interface_logic_id *= !tm.hit_b;
}

/*
	TODO
*/
void interface_behaviour_practice(MainMenu &tm)
{
	// TODO
	tm.interface_logic_id *= !tm.hit_b;
}

/*
	TODO
*/
void interface_behaviour_load(MainMenu &tm)
{
	// TODO
	tm.interface_logic_id *= !tm.hit_b;
}

/*
	// TODO
*/
void interface_behaviour_continue(MainMenu &tm)
{
	// TODO
	tm.interface_logic_id *= !tm.hit_b;
}

/*
	TODO
*/
void interface_behaviour_newgame(MainMenu &tm)
{
	// TODO
	tm.interface_logic_id *= !tm.hit_b;
}
