#include "main_menu.h"

/**
 *		LDC Compiler Implementation
 *
 * The LDC Compiler reads a menu list definition .ldc file and compiles it to a struct complex.
 * This resulting complex can be used write an intern list logic, which was externally defined and compiled
 * at load time.
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
 * 	:cluster <cluster_name>
 * this statement marks the definition of a list, sublist or list entity collection
 * any cluster can be referred to by name within the list definition file
 * to define the main list use :cluster main
 * 
 * 	:logic <condition_list_size>
 * setup of extern condition logic list to change viewability of entities with :condition command
 * TODO: add an option to define what will be shown if the condition wasn't met (greyed,???,gfx replacements)
 * 
 * 	:define <entity_name>
 * define the name of the list entity, that will be shown alongside other list member entities
 * 
 * 	:describe
 * 	<description>
 * to add a description to the list entity, that will be shown when currently selected
 *
 * 	:floats (<float><space>)*
 * add float constants to list entity, accessible for any reason, usage defined by logic
 *
 *
 * 	:segment <segment_name>
 * whereever a segment is called within the list defintion file, a dividing, stylized line will be
 * drawn between the imperatively previous and following definition
 * 
 * 	:condition (<condition_id><space>)*
 * specify an amount of conditions to activate list entity.
 * the condition ids will read the respective booleans from an extern condition list.
 * 
 * 
 * 		BEHAVIOUR DEFINITIONS
 * (ideally choose ONE of the following commands per :define)
 *
 * 	:system_behaviour <behaviour_id>
 * behaviour_ids:
 * 	0 = no behaviour
 * 	1 = terminate current list
 * purpose:
 * this will give the list option a system functionality, that will be executed when it is selected & confirmed.
 * behaviour ids can be defined differently in custom logic, but it is highly discouraged due to possible
 * confusion, when standard behaviour differs. individual checks into custom logic are generally avoided.
 * 
 * 	:subsequent <cluster_name>
 * confirming selection on this entity will load a sublist linked by it's cluster
 * the list will then be shown under the heading of this list entities :define
 * 
 * 	:checkbox
 * sign this entity to mark it switchable between true or false states
 * 
 * 	:dropdown
 * 	<fist_option>;<second_option>;<third_option>
 * add a sublist/dropdown option to this entity, holding selectable options divided by a ';'
 * using :dropdown without defining options will still create a dropdown and the list can be
 * assembled dynamically
 * TODO: add an option to escape buffer read by defining an end statement, else the full buffer will be dd
 * 
 * 	:slider
 * make this entity contain a changable floating point number to be adjusted by e.g. a slider
 * 
 * 	:return <output_value>
 * selecting this entity stops list interaction immediately and returns given value
 * 
 * 
 * The following implementation is the static compile function for LDC, holding the language interpreter
 * conforming to the above language definitions and statements.
 * The compiler only has one feature: compile, because this is what it does.
*/

/*
	compile(const char*) -> LDCProcessState (static) !O(n)b
	purpose: load .ldc files, break it up into a command buffer and compile it into a complex
	\param path: path to .ldc file
	\returns compiled struct complex to be used by UI feature
	NOTE: don't kill me for calling it "compile", i just wanted to be cute ok. i kinda does compile though
*/
LDCProcessState LDCCompiler::compile(const char* path)
{
	// logic id overhead
	const std::string mlcmd[LIST_LANGUAGE_COMMAND_COUNT] = {
		"cluster","logic","define","describe","floats","segment","condition","subsequent",
		"system_behaviour","checkbox","dropdown","slider","return",
	};
	interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {
		command_logic_cluster,command_logic_logiclist,command_logic_define,command_logic_describe,
		command_logic_attributes,command_logic_segment,command_logic_condition,command_logic_subsequent,
		command_logic_sysbehaviour,command_logic_checkbox,command_logic_dropdown,command_logic_slider,
		command_logic_return,command_logic_syntax_error,
	};
	// TODO: decide if those should be local to the compile function

	// first pass: extraction - iterate definition file
	std::ifstream file(path,std::ios::in);
	std::vector<ListLanguageCommand> cmd_buffer;
	uint16_t line_number = 1;
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

			// handle command attributes
			llc.tail = line.erase(0,cmd_split+1);
			llc.line_number = line_number;

			// assemble command buffer
			cmd_buffer.push_back(llc);
		} else cmd_buffer.back().buffer += line+' ';

		// incement line counter & finally close file
		line_number++;
	} file.close();

	// setup process state
	LDCProcessState state;
	state.fpath = path;

	// second pass: commands - execute extracted commands
	for (ListLanguageCommand &cmd : cmd_buffer) interpreter_behaviour[cmd.id](cmd,state);

	// third pass: interpretation - temporary instruction data
	for (LDCCluster &cluster : state.clusters) {

		// correlate child name to cluster id
		for (uint16_t pid : cluster.parent_ids) {
			uint8_t i = 0;
			while (state.clusters[i].id!=cluster.elist[pid].child_name) i++;
			cluster.elist[pid].tdata = i;

		// set segment jump write
		} for (LDCSegment &segment : cluster.slist)
			cluster.elist[segment.position].jsegment = true;

	// compiled list status
	} return state;
}

/**
 *		Start Implementation of Compiler Logic Pointers switched by Command
 *
 * Command ids from buffer, assembled at first compiler pass, jump to their processing logic by memory id.
 *
 * To define a new command, first add the command string without ':' to mlcmd list in LDCCompiler::compile,
 * then create the logic below and add the function to interpreter_behaviour in LDCCompiler::compile.
 * Finally increment LIST_LANGUAGE_COMMAND_COUNT constexpr in the header file.
 *
 * Features implemented according to the List Language Command language definition, established above the
 * compiler implementation.
*/

/*
	command_logic_cluster(const ListLanguageCommand&,LDCProcessState&) -> (static,global) void !O(1)
	purpose: open new cluster definition until next definition is opened. (cmd = :cluster)
	conforming to: void* interpreter_logic
*/
void command_logic_cluster(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	LDCCluster cluster;
	cluster.id = cmd.tail;
	state.clusters.push_back(cluster);
}

/*
	command_logic_logiclist(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: setup a boolean list, that can be interacted with to switch entity visibility (cmd = :logic)
	conforming to: void* interpreter_logic
*/
void command_logic_logiclist(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	try {
		state.condition_list = std::vector<bool>(stoi(cmd.tail));
	} catch (std::invalid_argument const &ex) {
		compiler_error_msg(state.fpath,"logic tail does not contain a valid number",cmd.line_number);
	} catch (std::out_of_range const &ex) {
		compiler_error_msg(state.fpath,"given number exceeds legal list range",cmd.line_number);
	}
}

/*
	command_logic_define(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: add named list entity to currently opened cluster (cmd = :define)
	conforming to: void* interpreter_logic
*/
void command_logic_define(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	LDCEntity entity;
	entity.head = cmd.tail;
	state.clusters.back().elist.push_back(entity);
}

/*
	command_logic_describe(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: add entity description (cmd = :describe)
	conforming to: void* interpreter_logic
*/
void command_logic_describe(const ListLanguageCommand &cmd,LDCProcessState &state)
{ state.clusters.back().elist.back().description = cmd.buffer; }

/*
	command_logic_attributes(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: store constant attributes in entity
	conforming to: void* interpreter_logic
*/
void command_logic_attributes(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	try {
		std::stringstream bfss(cmd.tail);
		std::string attrib;
		while (getline(bfss,attrib,' '))
			state.clusters.back().elist.back().fattribs.push_back(stof(attrib));
	} catch (std::invalid_argument const &ex) {
		compiler_error_msg(state.fpath,"attribute tail should only contain float values",cmd.line_number);
	} catch (std::out_of_range const &ex) {
		compiler_error_msg(state.fpath,"at least one attribute is out of float range",cmd.line_number);
	}
}

/*
	command_logic_segment(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: add a horrible segment that will cause problems in your logic later (cmd = :segment)
	conforming to: void* interpreter_logic
*/
void command_logic_segment(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	LDCSegment segment;
	segment.position = state.clusters.back().elist.size();
	segment.title = cmd.tail;
	state.clusters.back().slist.push_back(segment);
}

/*
	command_logic_condition(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: define entity activation boolean reference in logic list by memory id (cmd = :condition)
	conforming to: void* interpreter_logic
*/
void command_logic_condition(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	try {
		std::stringstream bfss(cmd.tail);
		std::string lid;
		while (getline(bfss,lid,' '))
			state.clusters.back().elist.back().condition_id.push_back(stoi(lid));
	} catch (std::invalid_argument const &ex) {
		compiler_error_msg(state.fpath,"condition tail does not contain a valid number",cmd.line_number);
	} catch (std::out_of_range const &ex) {
		compiler_error_msg(state.fpath,"given number exceeds reasonable range",cmd.line_number);
	}
}

/*
	command_logic_subsequent(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: link subsequent cluster to transition to as entity action (cmd = :subsequent)
	conforming to: void* interpreter_logic
*/
void command_logic_subsequent(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	state.clusters.back().elist.back().child_name = cmd.tail;
	state.clusters.back().parent_ids.push_back(state.clusters.back().elist.size()-1);
	state.clusters.back().elist.back().etype = SUBSEQUENT;
}

/*
	TODO
*/
void command_logic_sysbehaviour(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	try {
		state.clusters.back().elist.back().etype = SYSTEM;
		state.clusters.back().elist.back().tdata = stoi(cmd.tail);
	} catch (std::invalid_argument const &ex) {
		compiler_error_msg(state.fpath,"system command id not an interpretable number",cmd.line_number);
	} catch (std::out_of_range const &ex) {
		compiler_error_msg(state.fpath,"system command id out of reasonable defrange",cmd.line_number);
	}
}

/*
	command_logic_checkbox(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: set boolean on/off functionality as entity action (cmd = :checkbox)
	conforming to: void* interpreter_logic
*/
void command_logic_checkbox(const ListLanguageCommand &cmd,LDCProcessState &state)
{ state.clusters.back().elist.back().etype = CHECKBOX; }

/*
	command_logic_dropdown(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: create sublist index linked choices attached to entity (cmd = :dropdown)
	conforming to: void* interpreter_logic
*/
void command_logic_dropdown(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	std::stringstream bfss(cmd.buffer);
	std::string ddoption;
	while (getline(bfss,ddoption,';'))
		state.clusters.back().elist.back().cattribs.push_back(ddoption);
	state.clusters.back().elist.back().etype = DROPDOWN;
}
// TODO: benchmark push back usage against doubled tail process + predefined list size for tail copy
// 	this is also relevant for attributes/floats insertions

/*
	command_logic_slider(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: attach transformable float value to entity (cmd = :slider)
	conforming to: void* interpreter_logic
*/
void command_logic_slider(const ListLanguageCommand &cmd,LDCProcessState &state)
{ state.clusters.back().elist.back().etype = SLIDER; }

/*
	command_logic_return(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: set finalizing entity with set value return (cmd = :return)
	conforming to: void* interpreter_logic
*/
void command_logic_return(const ListLanguageCommand &cmd,LDCProcessState &state)
{
	state.clusters.back().elist.back().tdata = stoi(cmd.tail);
	state.clusters.back().elist.back().etype = RETURN;
}

/*
	command_logic_error(const ListLanguageCommand&,LDCProcessState&) -> void (static,global) !O(1)
	purpose: this method catches invalid syntax/commands and notifies the developer
	conforming to: void* interpreter_logic
*/
void command_logic_syntax_error(const ListLanguageCommand &cmd,LDCProcessState &state)
{ compiler_error_msg(state.fpath,"invalid command syntax",cmd.line_number); }


/*
	compiler_error_msg(const char*,const char*,uint16_t) -> void (static,global) !O(1)
	purpose: print error message for any ldc compiler problems at runtime
	\param path: file path holding the faulty statement
	\param msg: readable message, notifying the user about their mistake
	\param line_number: line number the error occured in for quick navigation
*/
void compiler_error_msg(const char* path,const char* msg,uint16_t line_number)
{ printf("\033[1;31mldc compiler error\033[0m in %s:%i %s\n",path,line_number,msg); }
// FIXME: the way error output works is pure and utter garbage. line_number & path are predictable


/**
 *		Selection Splice Geometry Implementation
 *
 *  the selection splice geometry is a stilistical highlight for the UI.
 *  it not only adds graphical flavour but sometimes also communicates to the user what the current selection is.
 *
 *  	here is some information on how this geometry can be defined:
 *  following, an ascii representation of the splice
 *  			 ___uwe__
 *  			/  uc   /
 *  		       /       /
 *  		      /	      /
 *  		     /	     /		^
 *  		    /	    /		| direction (horizontal = false)
 *  		   /	   /
 *  		  /   lc  /
 *  		 ___lwe___
 *
 *  	explanation:
 *  lc: centerpoint of lower geometry
 *  uc: centerpoint of upper geometry
 *  uwe: full upper width extension at upper centerpoint
 *  lwe: full lower width extension at lower centerpoint
 *  	(!careful! the width extension shown here is double the definition length, def. from centerpoint to edge)
 *  direction: automates the width extensions, so the orientation of the splice has to be predefined
 *
 *  	features of this implementation:
 *  - splices can be created
 *  - animation keys can be created, to transition in-between positional states by an external variable
 *  - geometry can be drawn
*/

/*
	create_splice(vec2,vec2,float,float,vec3,bool,float*) -> uint8_t !O(1)
	purpose: add new splash to geometry complex
	\param l: starting position of lower geometry centerpoint
	\param u: starting position of upper geometry centerpoint
	\param lw: starting width extension from lower geometry centerpoint
	\param uw: starting height extension from upper geometry centerpoint
	\param c: static vertex colour of the created splash
	\param hrz: set to true should the orientation of the splice (not the width extension) be horizontal
	\param tref: pointer to externly modified transition float:
		- will be cast to integer to specify current key & successor key, disregarding float part.
		- the fraction between key & successor key id will be interpreted as transition progress
	\returns memory id the created splice can be referenced by
*/
uint8_t SelectionSpliceGeometry::create_splice(glm::vec2 l,glm::vec2 u,float lw,float uw,glm::vec3 c,
		bool hrz,float* tref)
{
	// write vertices
	verts.push_back({ c,0 }),verts.push_back({ c,3 }),verts.push_back({ c,2 });
	verts.push_back({ c,3 }),verts.push_back({ c,0 }),verts.push_back({ c,1 });

	// set initial geometry transition key
	SelectionSpliceKey t_ssk;
	t_ssk.disp_lower = l,t_ssk.disp_upper = u;
	t_ssk.ext_lower = lw,t_ssk.ext_upper = uw;

	// copy initial information to current state & first transition key
	SelectionSplice splice;
	splice.current = t_ssk;
	splice.ssk.push_back(t_ssk);

	// write additional splice information
	splice.horizontal = hrz;
	splice.transition_ref = tref;
	splices.push_back(splice);

	// return id of created selection splice
	return splices.size()-1;
}

/*
	load() -> void !O(1)
	purpose: upload splice geometry, compile shader & setup coordinate system
*/
void SelectionSpliceGeometry::load()
{
	// upload vertex data to buffer
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	size_t vsize = sizeof(SpliceVertexGeometry);
	shader.compile("./shader/main_menu/vsplash.shader","./shader/main_menu/fsplash.shader");
	shader.def_irregular_attributeF("colour",3,vsize,offsetof(SpliceVertexGeometry,colour));
	shader.def_irregular_attributeI("edge_id",1,vsize,offsetof(SpliceVertexGeometry,edge_id));
	shader.upload_camera(Camera2D(1280.f,720.f));
}

/*
	add_anim_key(uint8_t,vec2,vec2,float,float) -> void !O(1)
	purpose: create transition key information for referenced splice geometry
		this key will always be added at the end of the key list, creation chronology matters
	\param id: memory id given at creation call, referencing the splice to add key to
	\param ld: transition target for lower splice geometry centerpoint
	\param ud: transition target for upper splice geometry centerpoint
	\param le: transition target for lower width extension
	\param ue: transition target for upper width extension
*/
uint8_t SelectionSpliceGeometry::add_anim_key(uint8_t id,glm::vec2 ld,glm::vec2 ud,float le,float ue)
{
	// key creation
	SelectionSpliceKey t_ssk;
	t_ssk.disp_lower = ld,t_ssk.disp_upper = ud;
	t_ssk.ext_lower = le,t_ssk.ext_upper = ue;

	// store key
	splices[id].ssk.push_back(t_ssk);
	return splices[id].ssk.size()-1;
}

/*
	update() -> void !O(n)
	purpose: processing for all elements in splices list:
		calculation of transition animation, uniform upload & geometry drawcalls
*/
void SelectionSpliceGeometry::update()
{
	// setup buffer & shader for draw
	buffer.bind();
	shader.enable();

	// draw created splashes
	for (uint8_t i=0;i<splices.size();i++) {

		// find transition state & load geometry keys
		// successor will only be stored as nkey should there be a successor otherwise ckey=nkey
		// tgeom holds transition in-between ckey and nkey, which is irrelevant if ckey=nkey
		bool sccr = (*splices[i].transition_ref+1)<splices[i].ssk.size();
		float tgeom = fmod(*splices[i].transition_ref,1.f);
		SelectionSpliceKey ckey = splices[i].ssk[*splices[i].transition_ref],
			nkey = splices[i].ssk[*splices[i].transition_ref+sccr];

		// transition uniform variables
		glm::vec2 vmult = glm::vec2(tgeom);
		splices[i].current.disp_lower = ckey.disp_lower+(nkey.disp_lower-ckey.disp_lower)*vmult;
		splices[i].current.disp_upper = ckey.disp_upper+(nkey.disp_upper-ckey.disp_upper)*vmult;
		splices[i].current.ext_lower = ckey.ext_lower+(nkey.ext_lower-ckey.ext_lower)*tgeom;
		splices[i].current.ext_upper = ckey.ext_upper+(nkey.ext_upper-ckey.ext_upper)*tgeom;

		// uniform upload
		shader.upload_vec2("lupos[0]",splices[i].current.disp_lower),
			shader.upload_vec2("lupos[1]",splices[i].current.disp_upper);
		shader.upload_float("luext[0]",splices[i].current.ext_lower),
			shader.upload_float("luext[1]",splices[i].current.ext_upper);
		shader.upload_int("is_hrz",splices[i].horizontal);

		// draw splice
		glDrawArrays(GL_TRIANGLES,i*6,6);
	}
}


/**
 * 		Menu List Implementation
 * 
 * TODO: expand this segment documentation
*/

/*
	TODO
*/
MenuList::MenuList(const char* path)
{
	// execute definition code
	LDCProcessState ldc_result = LDCCompiler::compile(path);
	clusters = ldc_result.clusters;

	// visuals creation
	for (LDCCluster &cluster : clusters) {
		int32_t vscroll = MENU_LIST_SCROLL_START;
		uint8_t i_seg = 0;

		// setup text lists for current cluster
		std::vector<Text> ctx_elist,ctx_slist;

		// write segment information in-between list elements
		for (uint8_t i=0;i<cluster.slist.size();i++) {
			Text stext = Text(st_font);
			stext.add(cluster.slist[i].title.c_str(),glm::vec2(
					MENU_LIST_HEADPOS_X+MENU_LIST_SEGMENT_PUSH_X,
					MENU_LIST_SCROLL_START-(cluster.slist[i].position+i)*MENU_LIST_SCROLL_Y)),
				stext.load();
			ctx_slist.push_back(stext);

		// process cluster entities
		} for (LDCEntity entity : cluster.elist) {
			vscroll -= MENU_LIST_SCROLL_Y*entity.jsegment;
			Text etext = Text(st_font);
			etext.add(entity.head.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),etext.load();
			ctx_elist.push_back(etext);
			vscroll -= MENU_LIST_SCROLL_Y;
		}

		// add created text
		tx_elist.push_back(ctx_elist),tx_slist.push_back(ctx_slist);
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
	int16_t didx = tx_elist[active_cluster_id].size()-(gsel+1);
	grid += didx*(didx<0);
	grid *= grid>0;
	gsel = lscroll+grid;
	// FIXME: don't just update gsel variable. it's bad form i think

	// switching list activation
	if (conf&&clusters[active_cluster_id].elist[gsel].child_name.size())
		active_cluster_id = clusters[active_cluster_id].elist[gsel].tdata;

	// list navigation towards parent
	bool stall_back = active_cluster_id;
	active_cluster_id *= !back;
	back = !stall_back&&back;

	// draw segments & entities
	for (auto txs : tx_slist[active_cluster_id])
		txs.prepare(),txs.set_scroll(glm::vec2(0,lscroll*MENU_LIST_SCROLL_Y)),
			txs.render(1024,glm::vec4(.7f,.7f,.7f,1.f));
	for (auto txe : tx_elist[active_cluster_id])
		txe.prepare(),txe.set_scroll(glm::vec2(0,lscroll*MENU_LIST_SCROLL_Y)),
			txe.render(1024,glm::vec4(1));
}
// TODO: transition between lists (background to foreground animation, tilt shift?)


/**
 *		Start Implementation of Popup Dialogue for Main Menu:
 *
 * TODO: explain
*/

/*
	TODO
*/
uint8_t MenuDialogue::add_dialogue_window(const char* path,glm::vec2 center,float width,float height,
		uint8_t tsize,uint8_t dsize)
{
	// content information extraction
	LDCProcessState ldc_result = LDCCompiler::compile(path);

	// setup dialogue fonts
	Font tfont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",tsize,tsize);
	Font ofont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",dsize,dsize);
	Font dfont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25);
	// FIXME: if my font implementation wouldn't be that ass i could just use variable sized texts

	// process all clusters as dialogues
	uint8_t out = dg_data.size();
	for (LDCCluster cluster : ldc_result.clusters) {

		// store option count of current dialogue window
		size_t opcount = cluster.elist.size();

		// on-demand translation handling through ldc floats command
		glm::vec2 t_center = center;
		if (cluster.elist[0].fattribs.size()>1)
			t_center.x += cluster.elist[0].fattribs[0],t_center.y += cluster.elist[0].fattribs[1];

		// setup dialogue data with list start position
		SingularDialogueData dgd;
		dgd.liststart_y = t_center.y+dsize*(opcount>>1)+(dsize>>1)*(opcount&1);
		dgd.option_size = dsize;

		// precalculation & background vertices
		float hwidth = width*.5f,hheight = height*.5f;
		DialogueBackgroundGeometry t_dbg[] = {
			{ glm::vec2(t_center.x-hwidth,t_center.y-hheight),0 },
			{ glm::vec2(t_center.x+hwidth,t_center.y+hheight),0 },
			{ t_center,1 },
			{ glm::vec2(t_center.x-hwidth,t_center.y-hheight),0 },
			{ t_center,2 },
			{ glm::vec2(t_center.x+hwidth,t_center.y+hheight),0 }
		}; bgr_verts.insert(std::end(bgr_verts),std::begin(t_dbg),std::end(t_dbg));

		// selector vertices
		uint8_t mdos = dsize*.8f,hmdos = mdos>>1;
		float xoffcenter = t_center.x-hwidth;
		glm::vec2 t_sverts[] = {
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y),
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y),
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y)
		}; slc_verts.insert(std::end(slc_verts),std::begin(t_sverts),std::end(t_sverts));
		// TODO: implement prototype selector style
		//	first figure out the idea to make it look nice before blindly implementing
		// TODO: implement choice description print & change list selector to background geometry

		// dialogue title text setup
		dgd.tx_title = Text(tfont),dgd.tx_options = Text(ofont),dgd.tx_descriptions = Text(dfont);
		dgd.tx_title.add(cluster.id.c_str(),t_center+glm::vec2(0,hheight*MENU_DIALOGUE_OFFSET_FACTOR));
		dgd.tx_title.load();

		// setup memory for jump action to subsequent dialogues
		dgd.await.resize(opcount);
		dgd.wait_value.resize(opcount);

		// dialogue option text setup
		for (uint8_t i=0;i<opcount;i++) {

			// action setup
			dgd.await[i] = cluster.elist[i].etype-(uint8_t)LDCEntityType::RETURN;
			dgd.wait_value[i] = cluster.elist[i].tdata;

			// information print setup
			dgd.tx_options.add(
					cluster.elist[i].head.c_str(),
					glm::vec2(
						t_center.x-hwidth*MENU_DIALOGUE_OFFSET_FACTOR,
						dgd.liststart_y-dsize*i));
			dgd.tx_descriptions.add(cluster.elist[i].description.c_str(),glm::vec2(1030,350-720*i),
					200.f,20.f);
			dgd.description_length += cluster.elist[i].description.length();
		} dgd.tx_options.load(),dgd.tx_descriptions.load();

		// additional data
		dgd.max_options = opcount-1;
		dgd.max_width = hwidth,dgd.max_height = hheight;
		dg_data.push_back(dgd);

	// return parent id
	} return out;
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
	size_t vsize = sizeof(DialogueBackgroundGeometry);
	bgr_shader.compile("./shader/main_menu/vdialogue.shader","./shader/main_menu/fdialogue.shader");
	bgr_shader.def_irregular_attributeF("position",2,vsize,offsetof(DialogueBackgroundGeometry,position));
	bgr_shader.def_irregular_attributeI("disp_id",1,vsize,offsetof(DialogueBackgroundGeometry,disp_id));
	bgr_shader.upload_vec2("displace[0]",glm::vec2(0));
	bgr_shader.upload_camera(cam2D);

	// upload selector vertices
	slc_buffer.bind();
	slc_buffer.upload_vertices(slc_verts);

	// selector shader setup
	slc_shader.compile("./shader/main_menu/vdlgselector.shader","./shader/main_menu/fdlgselector.shader");
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
		uint8_t id = active_ids.back();

		// close dialogue on request & skip selection update
		if (back) {
			close_dialogue(id);
			return;
		}

		// update interactions, input will act upon the last element of idle dialogues
		// this can result in a recursive dialogue interaction (filo principle)
		// starting with selection
		SingularDialogueData* csdd = &dg_data[id];
		if (!mperiph) csdd->sindex += imv;
		else csdd->sindex = (mypos-csdd->liststart_y)/csdd->option_size*-1;
		csdd->sindex = (csdd->sindex<0) ? 0 : (csdd->sindex>csdd->max_options)
				? csdd->max_options : csdd->sindex;

		// confirmation handling
		if (conf) {
			switch (dg_data[id].await[csdd->sindex]) {
				case 1: open_dialogue(dg_data[id].wait_value[csdd->sindex]);
					break;
				case 2: close_dialogue(id);
					break;
				default: dg_state = csdd->sindex;
			}
		}

		// write in-dialogue text
		dg_data[id].tx_title.prepare();
		dg_data[id].tx_title.render(128,glm::vec4(DIALOGUE_HEAD_COLOUR,1.f));
		dg_data[id].tx_options.prepare();
		dg_data[id].tx_options.render(128,glm::vec4(DIALOGUE_OPTION_COLOUR,1.f));

		// show description of selected option
		dg_data[id].tx_descriptions.prepare();
		dg_data[id].tx_descriptions.set_scroll(glm::translate(glm::mat4(1.f),
				glm::vec3(.0f,720.f*dg_data[id].sindex,.0f)));
		dg_data[id].tx_descriptions.render(dg_data[id].description_length,glm::vec4(1.f));
	}

	// setup selector draw
	slc_buffer.bind();
	slc_shader.enable();

	// draw idle selectors
	for (uint8_t id : active_ids) {
		slc_shader.upload_float("mv_select",dg_data[id].sindex*dg_data[id].option_size);
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
MainMenu::MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float &progress,float pseq)
	: m_ccbm(ccbm),m_ccbf(ccbf),m_world(world)
{
	// setup logic collection
	interface_behaviour[INTERFACE_LOGIC_MACRO] = interface_behaviour_macro,
	interface_behaviour[INTERFACE_LOGIC_OPTIONS] = interface_behaviour_options,
	interface_behaviour[INTERFACE_LOGIC_EXTRAS] = interface_behaviour_extras,
	interface_behaviour[INTERFACE_LOGIC_PRACTICE] = interface_behaviour_practice,
	interface_behaviour[INTERFACE_LOGIC_LOAD] = interface_behaviour_load,
	interface_behaviour[INTERFACE_LOGIC_CONTINUE] = interface_behaviour_continue,
	interface_behaviour[INTERFACE_LOGIC_NEWGAME] = interface_behaviour_newgame;
	// TODO: a lot of performance and translation testing necessary to analyze the flipsides

	// asset load
	index_ranim = ccbf->r2d->al.size();
	index_rsprite = ccbm->add_lv("lvload/main_menu.ccb");

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

	// setup head splice
	// 1st key: splice disable in case the start screen is displayed
	// 2nd key: underlining the parent menu options selected by the following splice
	// 3rd key: list selection expansion, perfectly fitting list elements, to be modified at change
	// 4th key: tilted splice underlining descriptions for dialogue options if opened
	splice_head_id = splices_geometry.create_splice(
			SPLICE_HEAD_LOWER_START,SPLICE_HEAD_UPPER_START,
			0,0,
			SPLICE_HEAD_COLOUR,true,&tkey_head);
	splices_geometry.add_anim_key(splice_head_id,
			SPLICE_HEAD_LOWER_START,SPLICE_HEAD_UPPER_START,
			SPLICE_HEAD_LOWER_WIDTH,SPLICE_HEAD_UPPER_WIDTH);
	head_mod_id = splices_geometry.add_anim_key(splice_head_id,
			glm::vec2(0,SPLICE_HEAD_ORIGIN_POSITION),glm::vec2(1280,SPLICE_HEAD_ORIGIN_POSITION),
			SPLICE_HEAD_ORIGIN_WIDTH,SPLICE_HEAD_ORIGIN_WIDTH);
	splices_geometry.add_anim_key(splice_head_id,
			glm::vec2(0,SPLICE_HEAD_DLGDESC_QUAD),glm::vec2(SPLICE_HEAD_DLGDESC_QUAD,0),
			SPLICE_HEAD_DLGDESC_WIDTH,SPLICE_HEAD_DLGDESC_WIDTH);

	// setup selection splice
	// 1st key: splice disable, zero'd so the splice gets projected into scene when start is pressed
	// 2nd key: vertical selection mode for main options
	splice_selection_id = splices_geometry.create_splice(
			SPLICE_LOWER_CENTER,glm::vec2(0,720.f),
			0,0,
			SPLICE_SELECTION_COLOUR,false,&tkey_selection);

	// setup title splice
	// 1st key: start request screen stylesplice
	// 2nd key: static title underline splice for the rest of the menu, after start request screen
	uint8_t sid = splices_geometry.create_splice(
			SPLICE_TITLE_LOWER_START,SPLICE_TITLE_UPPER_START,
			SPLICE_TITLE_LOWER_SWIDTH,SPLICE_TITLE_UPPER_SWIDTH,
			SPLICE_TITLE_COLOUR,false,&tkey_title);
	splices_geometry.add_anim_key(sid,
			SPLICE_TITLE_LOWER_MOD,SPLICE_TITLE_UPPER_MOD,
			SPLICE_TITLE_LWIDTH_MOD,SPLICE_TITLE_UWIDTH_MOD);

	// load geometry for splices
	splices_geometry.load();

	// dialogue setup
	dg_diffs = mdialogues.add_dialogue_window("./lvload/challenge.ldc",glm::vec2(670,310),320,140,30,25);
	dg_continue = mdialogues.add_dialogue_window("./lvload/continue.ldc",glm::vec2(640,360),320,250,30,25);
	mdialogues.load();
	// TODO: add second confirmation popup dialogue in case grandmaster or headmaster is selected

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
	inv_ftransition = 1.f-ftransition,inv_mtransition = 1.f-mtransition;
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

	// render & transform main options
	for (uint8_t i=0;i<MENU_MAIN_OPTION_COUNT;i++) {
		tx_mopts[i].prepare();
		glm::mat4 opt_trans = glm::translate(glm::mat4(1.f),
				glm::vec3(mo_cposition[i].x+mo_hwidth[i],mo_cposition[i].y,0));
		glm::vec4 opt_colour = glm::vec4(.5f,1.f,.5f,mtransition);

		// let currently selected option react to the players choice
		if (i==vselect) {
			opt_trans = glm::translate(opt_trans,
					glm::vec3((-mo_cposition[i]+SPLICE_HEAD_TITLE_OFFSET)*ftransition,0));
			opt_trans = glm::scale(opt_trans,glm::vec3(MENU_OPTIONS_SCALE_THRES));
			opt_trans = glm::rotate(opt_trans,st_rot*inv_ftransition,glm::vec3(0,0,1));
			opt_colour.z = 1.f;
		}

		// upload transform & render option text
		tx_mopts[i].set_scroll(opt_trans);
		tx_mopts[i].render(strlen(main_options[i])*(inv_ftransition+(i==vselect)),opt_colour);
	}
	// TODO: differenciate between list- & free mode for head splice displacement
	// TODO: also move active title to upper-left screen position while de-writing non-active
	// FIXME: the splash targeting is not intersecting with the exact center of the option text
	// FIXME: optimize before impending merge, when all the text transitions are done

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

	// curtain animation for main option selector splice
	float curtain_trans = mtransition-ftransition;
	splices_geometry.splices[splice_selection_id].ssk[0].ext_lower = lext_selection*curtain_trans,
	splices_geometry.splices[splice_selection_id].ssk[0].ext_upper = uext_selection*curtain_trans;

	// splash render
	tkey_head = mtransition+ftransition+mdialogues.system_active();
	tkey_title = mtransition;
	splices_geometry.update();
	// TODO: try and break down reference variables to a single float
	// FIXME: splash dimensions to prevent aesthetically unfortunate proportions

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
		SelectionSpliceKey* t_ssk = &splices_geometry.splices[splice_head_id].ssk[head_mod_id];
		float htrans = SPLICE_HEAD_ORIGIN_POSITION-MENU_LIST_SCROLL_Y*vgrid_id;
		t_ssk->disp_lower.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD),
		t_ssk->disp_upper.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD),
		t_ssk->ext_lower = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA),
		t_ssk->ext_upper = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA);
	}
}
// TODO: give each list their own grid id, so selection will be remembered, when going through subsequents
// TODO: with the above mentioned """tech""" produce a geometry shadow of previous list selection in the bgr (mdc)

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

		// change main option selector dimensions based on selected option
		SelectionSpliceKey* t_ssk = &tm.splices_geometry.splices[tm.splice_selection_id].ssk[0];
		glm::vec2 vrt_cpos = tm.mo_cposition[tm.vselect]+glm::vec2(tm.mo_hwidth[tm.vselect],0);
		t_ssk->disp_lower.x = (vrt_cpos.x-SPLICE_LOWER_CENTER.x)*SPLICE_OFFCENTER_MV
				+ SPLICE_LOWER_CENTER.x,
		tm.lext_selection = rand()%(uint16_t)tm.mo_hwidth[tm.vselect],
		tm.uext_selection = rand()%(uint16_t)tm.mo_hwidth[tm.vselect];

		// project upper displacement position based on lower displacement
		glm::vec2 vrt_dir = vrt_cpos-t_ssk->disp_lower;
		float vrt_extend_II = (720.f-vrt_cpos.y)/vrt_dir.y;
		t_ssk->disp_upper.x = vrt_cpos.x+vrt_dir.x*vrt_extend_II;

		// update randomized text rotation
		tm.st_rot = glm::radians((float)(rand()%MENU_OPTIONS_RDEG_THRES)*-((rand()%2)*2-1));
	}

	// reset
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

	// TODO: direction to run change menu list according to user input
	// TODO: actually implement continue functionality

	// closing behaviour
	tm.interface_logic_id *= tm.mdialogues.dg_data[tm.dg_continue].dg_active;
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

	// TODO: preface runcreation with disclaimer that current run will be replaced by the new as main
	// TODO: actually implement functionality of the menu option

	// closing condition
	tm.interface_logic_id *= tm.mdialogues.dg_data[tm.dg_diffs].dg_active;
}
