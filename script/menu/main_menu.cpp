#include "main_menu.h"


/**
 * 		Menu List Implementation with .ldc Language Interpreter
 * 
 * TODO: expand this segment documentation
 * 
 * 
 * 		List Definition Code Language Design
 * 
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
 * The following implementation is the MenuList constructor, holding the language interpreter
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

	// post-process list & visuals creation
	} for (MenuListCluster &cluster : clusters) {
		int32_t vscroll = MENU_LIST_SCROLL_START;
		uint8_t i_seg = 0;

		// convert cluster name references to cluster id
		for (auto pid : cluster.parents) {
			uint8_t i = 0;
			while (clusters[i].id!=cluster.elist[pid].child_name&&i<clusters.size()) i++;
			cluster.elist[pid].child_id = i;

		// process list segment heads
		} for (uint8_t i=0;i<cluster.slist.size();i++) {
			cluster.elist[cluster.slist[i].position].jsegment
				= cluster.slist[i].position<cluster.elist.size();

			// write segment information in-between list elements
			Text stext = Text(st_font);
			stext.add(cluster.slist[i].title.c_str(),glm::vec2(MENU_LIST_HEADPOS_X
					+ MENU_LIST_SEGMENT_PUSH_X,MENU_LIST_SCROLL_START
					- (cluster.slist[i].position+i)*MENU_LIST_SCROLL_Y)),
				stext.load();
			cluster.tx_slist.push_back(stext);

		// process cluster entities
		} for (auto entity : cluster.elist) {
			vscroll -= MENU_LIST_SCROLL_Y*entity.jsegment;
			Text etext = Text(st_font);
			etext.add(entity.head.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),etext.load();
			cluster.tx_elist.push_back(etext);
			vscroll -= MENU_LIST_SCROLL_Y;
		}
	}
}

/*
	TODO
*/
void MenuList::update(int8_t &grid,bool conf,bool &back)
{
	// translate input
	uint16_t gsel = lscroll+grid;
	//gsel += clusters[active_cluster_id].elist[gsel].jsegment;
	int16_t didx = clusters[active_cluster_id].elist.size()-(gsel+1);
	grid += didx*(didx<0);
	grid *= grid>0;
	gsel = lscroll+grid;
	// FIXME: don't just update gsel variable. it's bad form i think

	// switching list activation
	if (conf&&clusters[active_cluster_id].elist[gsel].child_name.size())
		active_cluster_id = clusters[active_cluster_id].elist[gsel].child_id;

	// list navigation towards parent
	bool stall_back = active_cluster_id;
	active_cluster_id *= !back;
	back = !stall_back&&back;

	// draw segments & entities
	for (auto txs : clusters[active_cluster_id].tx_slist)
		txs.prepare(),txs.set_scroll(glm::vec2(0,lscroll*MENU_LIST_SCROLL_Y)),
			txs.render(1024,glm::vec4(.7f,.7f,.7f,1.f));
	for (auto txe : clusters[active_cluster_id].tx_elist)
		txe.prepare(),txe.set_scroll(glm::vec2(0,lscroll*MENU_LIST_SCROLL_Y)),
			txe.render(1024,glm::vec4(1));
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
	ml.clusters.back().parents.push_back(ml.clusters.back().elist.size()-1);
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
 *		Start Implementation of Popup Dialogue for Main Menu:
 *
 * TODO: explain
*/

/*
	TODO
*/
uint8_t MenuDialogue::add_dialogue_window(const char* title,std::vector<const char*> options,
		glm::vec2 center,float width,float height)
{
	// setup dialogue data with list start position
	SingularDialogueData dgd;
	dgd.liststart_y = center.y+MENU_DIALOGUE_OPTION_SIZE*(options.size()>>1)
			+ (MENU_DIALOGUE_OPTION_SIZE>>1)*(options.size()&1);

	// precalculation & background vertices
	float hwidth = width*.5f,hheight = height*.5f;
	bgr_verts.push_back({ glm::vec2(center.x-hwidth,center.y-hheight),0 });
	bgr_verts.push_back({ glm::vec2(center.x+hwidth,center.y+hheight),0 });
	bgr_verts.push_back({ center,1 });
	bgr_verts.push_back({ glm::vec2(center.x-hwidth,center.y-hheight),0 });
	bgr_verts.push_back({ center,2 });
	bgr_verts.push_back({ glm::vec2(center.x+hwidth,center.y+hheight),0 });

	// selector vertices
	float xoffcenter = center.x-hwidth;
	slc_verts.push_back(glm::vec2(xoffcenter-10,dgd.liststart_y-20));
	slc_verts.push_back(glm::vec2(xoffcenter+10,dgd.liststart_y));
	slc_verts.push_back(glm::vec2(xoffcenter-10,dgd.liststart_y));
	slc_verts.push_back(glm::vec2(xoffcenter-10,dgd.liststart_y-20));
	slc_verts.push_back(glm::vec2(xoffcenter+10,dgd.liststart_y-20));
	slc_verts.push_back(glm::vec2(xoffcenter+10,dgd.liststart_y));
	// TODO: dynamify text/selector scaling

	// dialogue title text setup
	dgd.tx_title = Text(title_font);
	dgd.tx_title.add(title,center+glm::vec2(0,hheight*MENU_DIALOGUE_OFFSET_FACTOR));
	dgd.tx_title.load();

	// dialogue option text setup
	uint8_t cscr = 0;
	dgd.tx_options = Text(option_font);
	for (auto option : options)
		dgd.tx_options.add(option,glm::vec2(center.x-hwidth*MENU_DIALOGUE_OFFSET_FACTOR,
				dgd.liststart_y-MENU_DIALOGUE_OPTION_SIZE*cscr++));
	dgd.tx_options.load();

	// additional data
	dgd.max_options = options.size()-1;
	dgd.max_width = hwidth,dgd.max_height = hheight;
	dg_data.push_back(dgd);
	return dg_data.size()-1;
}

/*
	TODO
*/
void MenuDialogue::load()
{
	// upload background vertices
	bgr_buffer.bind();
	bgr_buffer.upload_vertices(bgr_verts);

	// coordinate system setup
	Camera2D cam2D = Camera2D(1280.f,720.f);

	// background shader setup
	bgr_shader.compile("./shader/main_menu/vdialogue.shader","./shader/main_menu/fdialogue.shader");
	bgr_shader.def_attributeF("position",2,0,DIALOGUEBGR_VERTEX_FLOAT_COUNT);
	bgr_shader.def_attributeI("disp_id",1,2,DIALOGUEBGR_VERTEX_FLOAT_COUNT);
	bgr_shader.upload_vec2("displace[0]",glm::vec2(0));
	bgr_shader.upload_camera(cam2D);

	// upload selector vertices
	slc_buffer.bind();
	slc_buffer.upload_vertices(slc_verts);

	// selector shader setup
	slc_shader.compile("./shader/main_menu/vdlgselector.shader",
			"./shader/main_menu/fdlgselector.shader");
	slc_shader.def_attributeF("position",2,0,2);
	slc_shader.upload_camera(cam2D);
}

/*
	TODO
*/
void MenuDialogue::open_dialogue(uint8_t did)
{
	// move dialogue id to opening state
	active_ids.erase(std::remove(active_ids.begin(),active_ids.end(),did),active_ids.end());
	closing_ids.erase(std::remove(closing_ids.begin(),closing_ids.end(),did),closing_ids.end());
	opening_ids.push_back(did);

	// randomize expansion edges
	dg_data[did].dim_width = dg_data[did].max_width-dg_data[did].max_width*(rand()%50+25)*.01f;
	dg_data[did].dim_height = dg_data[did].max_height-dg_data[did].max_height*(rand()%50+25)*.01f;

	// set dialogue info active
	dg_data[did].dg_active = true;
}

/*
	TODO
*/
void MenuDialogue::close_dialogue(uint8_t did)
{
	// move dialogue id to closing state
	opening_ids.erase(std::remove(opening_ids.begin(),opening_ids.end(),did),opening_ids.end());
	active_ids.erase(std::remove(active_ids.begin(),active_ids.end(),did),active_ids.end());
	closing_ids.push_back(did);

	// set dialogue info closed
	dg_data[did].dg_active = false;
}

/*
	TODO
*/
void MenuDialogue::update(int8_t imv,float mypos,bool mperiph,bool conf,bool back)
{
	// reset dialogue state to default=0, representing no results this frame
	dg_state = 0;

	// check selector update conditions
	// this does not include the render preparations, breaks if idling & opening simultaneously
	// excused for now due to the following if statement & exclusivity of open dialogue case
	// it also adds input security when calling selection_component() every frame
	if (active_ids.size()&&!opening_ids.size()) {

		// close dialogue on request & skip selection update
		if (back) {
			close_dialogue(active_ids.back());
			return;
		}

		// update interactions, input will act upon the last element of idle dialogues
		// this can result in a recursive dialogue interaction (filo principle)
		// starting with selection
		SingularDialogueData* csdd = &dg_data[active_ids.back()];
		if (!mperiph) csdd->sindex += imv;
		else csdd->sindex = (mypos-csdd->liststart_y)/MENU_DIALOGUE_OPTION_SIZE*-1;
		csdd->sindex = (csdd->sindex<0) ? 0 : (csdd->sindex>csdd->max_options)
				? csdd->max_options : csdd->sindex;

		// confirmation handling
		dg_state = csdd->sindex*conf;
	}

	// setup selector draw
	slc_buffer.bind();
	slc_shader.enable();

	// draw idle selectors
	for (uint8_t id : active_ids) {
		slc_shader.upload_float("mv_select",dg_data[id].sindex*MENU_DIALOGUE_OPTION_SIZE);
		glDrawArrays(GL_TRIANGLES,id*6,6);
	}
}
// FIXME: text tech results in ugly edge artifacts around letters after inverting to background (mdc)
// FIXME: avoid constant draw recalling, maybe better performance with triangle crunching (mdc)
// FIXME: (rare) flickering dialogue disappearance when closing dialogue (mdc)

/*
	TODO
*/
void MenuDialogue::background_component(float transition_delta)
{
	// setup draw
	bgr_buffer.bind();
	bgr_shader.enable();

	// draw closing dialogues
	uint8_t i = 0;
	while (i<closing_ids.size()) {
		uint8_t id = closing_ids[i];
		draw_dialogue(id);

		// transition closing dialogues
		dg_data[id].dgtrans -= transition_delta;
		if (dg_data[id].dgtrans<.0f) {
			dg_data[id].dgtrans = .0f;
			closing_ids.erase(closing_ids.begin()+i);
		} else i++;
	}

	// draw idle dialogues
	for (uint8_t id : active_ids) draw_dialogue(id);

	// draw opening dialogues
	i = 0;
	while (i<opening_ids.size()) {
		uint8_t id = opening_ids[i];
		draw_dialogue(id);

		// transition opening dialogues
		dg_data[id].dgtrans += transition_delta;
		if (dg_data[id].dgtrans>1.f) {
			dg_data[id].dgtrans = 1.f;
			active_ids.push_back(id);
			opening_ids.erase(opening_ids.begin()+i);
		} else i++;
	}
}

/*
	TODO
*/
void MenuDialogue::draw_dialogue(uint8_t id)
{
	// upload current vertex expasion by transition progress
	bgr_shader.upload_float("tprogress",dg_data[id].dgtrans);

	// upload vertex target displacement
	bgr_shader.upload_vec2("displace[1]",glm::vec2(-dg_data[id].dim_width,dg_data[id].dim_height));
	bgr_shader.upload_vec2("displace[2]",glm::vec2(dg_data[id].dim_width,-dg_data[id].dim_height));

	// draw background geometry
	glDrawArrays(GL_TRIANGLES,id*6,6);

	// write text
	dg_data[id].tx_title.prepare(),
		dg_data[id].tx_title.render(128,glm::vec4(DIALOGUE_HEAD_COLOUR,1.f));
	dg_data[id].tx_options.prepare(),
		dg_data[id].tx_options.render(128,glm::vec4(DIALOGUE_OPTION_COLOUR,1.f));
}
// TODO: exactly define how many instances are rendered per text object (mdc)


/**
 * 		The real menu implementation starts here!
 * 
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
	create_splash(sverts,glm::vec2(SPLICE_TITLE_LOWER_START,.0f),
			glm::vec2(SPLICE_TITLE_UPPER_START,720.f),glm::vec3(.5f,.0f,.0f));
	create_splash(sverts,glm::vec2(.0f,SPLICE_HEAD_ORIGIN_POSITION),
			glm::vec2(1280.f,SPLICE_HEAD_ORIGIN_POSITION),SPLICE_HEAD_COLOUR);
	create_splash(sverts,glm::vec2(.0f),glm::vec2(.0f,720.f),SPLICE_SELECTION_COLOUR);
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
		interface_behaviour[INTERFACE_LOGIC_PRACTICE] = interface_behaviour_practice,
		interface_behaviour[INTERFACE_LOGIC_LOAD] = interface_behaviour_load,
		interface_behaviour[INTERFACE_LOGIC_CONTINUE] = interface_behaviour_continue,
		interface_behaviour[INTERFACE_LOGIC_NEWGAME] = interface_behaviour_newgame;
	// TODO: a lot of performance and translation testing necessary to analyze the flipsides

	// dialogue setup
	std::vector<const char*> cnt_diffs = { "original","master","grandmaster","headmaster" };
	std::vector<const char*> cnt_options = { "continue autosave","continue manual load","back" };
	dg_diffs = mdialogues.add_dialogue_window("challenge selection",cnt_diffs,
			glm::vec2(670,310),320,140);
	dg_continue = mdialogues.add_dialogue_window("continue?",cnt_options,
			glm::vec2(640,360),320,250);
	mdialogues.load();
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
	udmv = (m_ccbf->iMap->get_input_triggered(IMP_REQDOWN)
			- m_ccbf->iMap->get_input_triggered(IMP_REQUP))*menu_action;
	crd_mouse = glm::vec2(m_ccbf->frame->mouse.mxfr*1280.f,m_ccbf->frame->mouse.myfr*720.f);
	trg_lmb = m_ccbf->frame->mouse.mb[0],trg_rmb = m_ccbf->frame->mouse.mb[2];
	// FIXME: as soon as the title screen has been passed, start press will become return request
	// FIXME: why is vertical selector's left/right selection input bound to option list size?
	// FIXME: up/down movement inverted with controller stick input for some reason

	// timing
	transition_delta = TRANSITION_SPEED*m_ccbf->frame->time_delta;
	bool anim_go = anim_timing>ANIMATION_UPDATE_TIMEOUT;
	anim_timing += m_ccbf->frame->time_delta;
	dt_tshiftdown += m_ccbf->frame->time_delta*speedup,
			dt_tnormalize += m_ccbf->frame->time_delta*!speedup;

	// focus transition
	Toolbox::transition_float_on_condition(ftransition,transition_delta,interface_logic_id);
	inv_ftransition = 1.f-ftransition;
	// FIXME: transition to 1.f takes 2 frames while transition to .0f takes about 12?

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

	// START RENDER MENU BUFFER
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
	// TODO: differenciate between list- & free mode for head splice displacement
	// TODO: also move active title to upper-left screen position while de-writing non-active

	// render titles
	m_ccbf->r2d->al[index_ranim+1].model = glm::translate(m_ccbf->r2d->al[index_ranim+1].model,
			glm::vec3(0,150*ftransition,0));
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(index_ranim,glm::vec2(3,0));
	m_ccbf->r2d->render_state(index_ranim+1,glm::vec2(0,0));

	// update & draw dialogues
	mdialogues.update(udmv,crd_mouse.y,m_ccbf->frame->mpref_peripheral,hit_a,hit_b);

	// START MULTISAMPLED RENDER
	FrameBuffer::close();
	msaa.bind();
	Frame::clear();

	// splash render
	sh_buffer.bind();
	sh_shader.enable();

	// head splash upload & render
	modify_splash(glm::vec2(0,SPLICE_HEAD_LOWER_START*inv_ftransition+lhead_translation_y),
			glm::vec2(0,SPLICE_HEAD_UPPER_START*inv_ftransition+uhead_translation_y),
			(lr_head_extend+SPLICE_HEAD_LOWER_WIDTH*inv_ftransition)*mtransition,
			(ur_head_extend+SPLICE_HEAD_UPPER_WIDTH*inv_ftransition)*mtransition,true);
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

	// draw dialogue selectors
	mdialogues.background_component(transition_delta);

	// STOP MULTISAMPLED RENDER
	msaa.blit();

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
 * 
 * a collection of additional methods, helping with main menu logic
 * TODO: improve section documentation
*/

/*
	TODO
*/
void MainMenu::update_list_grid(MenuList &ml)
{
	// mouse grid selection
	int8_t tid = vgrid_id;
	if (m_ccbf->frame->mpref_peripheral) {
		float org_delta = (MENU_LIST_SCROLL_START-crd_mouse.y);
		vgrid_id = org_delta/MENU_LIST_SCROLL_Y;

	// button delta selection
	} else vgrid_id += udmv;

	// process menu list input & render
	ml.update(vgrid_id,hit_a,hit_b);

	// update selection splash geometry
	if (tid!=vgrid_id) {
		float htrans = -MENU_LIST_SCROLL_Y*vgrid_id;
		lhead_translation_y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD);
		uhead_translation_y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD);
		lr_head_extend = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_WIDTH),
			ur_head_extend = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_WIDTH);
	}
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
 * 
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
	Toolbox::transition_float_on_condition(tm.mtransition,tm.transition_delta,tm.menu_action);
	/*uint8_t tmin = (mtransition<.0f),tmax = (mtransition>1.f);
	mtransition = mtransition-(mtransition-1.f)*tmax+abs(mtransition)*tmin;*/
	// TODO: compare linear transition with sinespeed transition implementation
	// 		also relate the results of this todo to the ftransition in main render method
	tm.inv_mtransition = 1.f-tm.mtransition;

	// processing selection input
	tm.vselect += tm.lrmv;
	bool ch_select = tm.lrmv!=0;
	if (tm.m_ccbf->frame->mpref_peripheral) {
		uint8_t out_id = tm.vselect;
		while (tm.crd_mouse.x<(tm.mo_cposition[out_id].x-tm.mo_prog.x)&&out_id>0) out_id--;
		while (tm.crd_mouse.x>(tm.mo_cposition[out_id].x+tm.mo_twidth[out_id]+tm.mo_prog.x)
				&& out_id<MENU_MAIN_OPTION_CAP) out_id++;
		ch_select = ch_select||(out_id!=tm.vselect);
		tm.vselect = out_id;
	}

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

	// reset
	tm.lhead_translation_y = 0,tm.uhead_translation_y = 0;
	tm.diff_popup = false,tm.shot_popup = false;
}

/*
	TODO
*/
void interface_behaviour_options(MainMenu &tm)
{
	tm.update_list_grid(tm.ml_options);
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
	// open dialogue for continuation request
	if (!tm.shot_popup) {
		tm.mdialogues.open_dialogue(tm.dg_continue);
		tm.shot_popup = true;
	}

	// TODO

	// closing behaviour
	if (tm.mdialogues.dg_state==2) tm.mdialogues.close_dialogue(tm.dg_continue);
	tm.interface_logic_id *= tm.mdialogues.dg_data[tm.dg_continue].dg_active
			&& tm.mdialogues.dg_state!=2;
}

/*
	TODO
*/
void interface_behaviour_newgame(MainMenu &tm)
{
	// opening difficulty popup dialogue when option first is chosen
	if (!tm.diff_popup) {
		tm.mdialogues.open_dialogue(tm.dg_diffs);
		tm.diff_popup = true;
	}

	// TODO

	// closing condition
	tm.interface_logic_id *= !tm.hit_b;
}
