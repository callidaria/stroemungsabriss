#include "main_menu.h"


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


typedef void (*interpreter_logic)(LDCProcessState&);

/*
	command_logic_cluster(LDCProcessState&) -> (static,global) void !O(1)
	purpose: open new cluster definition until next definition is opened. (cmd = :cluster)
	conforming to: void* interpreter_logic
*/
void command_logic_cluster(LDCProcessState &state)
{
	LDCCluster cluster;
	cluster.id = state.cmd->tail;
	state.clusters.push_back(cluster);
	state.crefs.push_back({{},{}});
	state.srefs.push_back({});
}

/*
	command_logic_define(LDCProcessState&) -> void (static,global) !O(1)
	purpose: add named list entity to currently opened cluster (cmd = :define)
	conforming to: void* interpreter_logic
*/
void command_logic_define(LDCProcessState &state)
{
	LDCEntity entity;
	entity.head = state.cmd->tail;
	state.clusters.back().elist.push_back(entity);
}

/*
	command_logic_describe(LDCProcessState&) -> void (static,global) !O(1)
	purpose: add entity description (cmd = :describe)
	conforming to: void* interpreter_logic
*/
void command_logic_describe(LDCProcessState &state)
{ state.clusters.back().elist.back().description = state.cmd->buffer; }

/*
	command_logic_fattributes(LDCProcessState&) -> void (static,global) !O(1)
	purpose: store constant attributes in entity
	conforming to: void* interpreter_logic
*/
void command_logic_fattributes(LDCProcessState &state)
{
	try {
		std::stringstream bfss(state.cmd->tail);
		std::string attrib;
		while (getline(bfss,attrib,' '))
			state.clusters.back().elist.back().fattribs.push_back(stof(attrib));
	} catch (std::invalid_argument const &ex) {
		LDCCompiler::compiler_error_msg(state,"attribute tail should only contain float values");
	} catch (std::out_of_range const &ex) {
		LDCCompiler::compiler_error_msg(state,"at least one attribute is out of float range");
	}
}

/*
	TODO
*/
void command_logic_sattributes(LDCProcessState &state)
{
	std::stringstream bfss(state.cmd->tail);
	std::string attrib;
	while (getline(bfss,attrib,' '))
		state.clusters.back().elist.back().cattribs.push_back(attrib);
}

/*
	command_logic_segment(LDCProcessState&) -> void (static,global) !O(1)
	purpose: add a horrible segment that will cause problems in your logic later (cmd = :segment)
	conforming to: void* interpreter_logic
*/
void command_logic_segment(LDCProcessState &state)
{
	LDCSegment segment;
	segment.position = state.clusters.back().elist.size();
	segment.title = state.cmd->tail;
	state.clusters.back().slist.push_back(segment);
}

/*
	command_logic_condition(LDCProcessState&) -> void (static,global) !O(1)
	purpose: define entity activation boolean reference in logic list by memory id (cmd = :condition)
	conforming to: void* interpreter_logic
*/
void command_logic_condition(LDCProcessState &state)
{
	try {
		std::stringstream bfss(state.cmd->tail);
		std::string lid;
		while (getline(bfss,lid,' '))
			state.clusters.back().elist.back().condition_id.push_back(stoi(lid));
	} catch (std::invalid_argument const &ex) {
		LDCCompiler::compiler_error_msg(state,"condition tail does not contain a valid number");
	} catch (std::out_of_range const &ex) {
		LDCCompiler::compiler_error_msg(state,"given number exceeds reasonable range");
	}
}

/*
	TODO
*/
void command_logic_link(LDCProcessState &state)
{
	state.clusters.back().linked_ids.push_back(state.clusters.back().elist.size()-1);
	state.srefs.back().push_back(state.cmd->tail);
}

/*
	command_logic_subsequent(LDCProcessState&) -> void (static,global) !O(1)
	purpose: link subsequent cluster to transition to as entity action (cmd = :subsequent)
	conforming to: void* interpreter_logic
*/
void command_logic_subsequent(LDCProcessState &state)
{
	state.crefs.back().parent_ids.push_back(state.clusters.back().elist.size()-1);
	state.crefs.back().child_names.push_back(state.cmd->tail);
	state.clusters.back().elist.back().etype = SUBSEQUENT;
}

/*
	command_logic_sysbehaviour(LDCProcessState&) -> void (static,global) !O(1)
	purpose: read system command id according to predefinition in ldc documentation above when confirmed
	conforming to: void* interpreter_logic
*/
void command_logic_sysbehaviour(LDCProcessState &state)
{
	try {
		state.clusters.back().elist.back().etype = SYSTEM;
		state.clusters.back().elist.back().tdata = stoi(state.cmd->tail);
	} catch (std::invalid_argument const &ex) {
		LDCCompiler::compiler_error_msg(state,"system command id not an interpretable number");
	} catch (std::out_of_range const &ex) {
		LDCCompiler::compiler_error_msg(state,"system command id out of reasonable defrange");
	}
}

/*
	command_logic_checkbox(LDCProcessState&) -> void (static,global) !O(1)
	purpose: set boolean on/off functionality as entity action (cmd = :checkbox)
	conforming to: void* interpreter_logic
*/
void command_logic_checkbox(LDCProcessState &state)
{
	state.clusters.back().elist.back().etype = CHECKBOX;
	state.clusters.back().cnt_checkbox++;
}

/*
	command_logic_dropdown(LDCProcessState&) -> void (static,global) !O(1)
	purpose: create sublist index linked choices attached to entity (cmd = :dropdown)
	conforming to: void* interpreter_logic
*/
void command_logic_dropdown(LDCProcessState &state)
{
	std::stringstream bfss(state.cmd->buffer);
	std::string ddoption;
	while (getline(bfss,ddoption,' ')) {
		if (ddoption[0]==';') break;
		state.clusters.back().elist.back().cattribs.push_back(ddoption);
	}
	state.clusters.back().elist.back().etype = DROPDOWN;
	state.clusters.back().cnt_dropdown++;
}
// TODO: benchmark push back usage against doubled tail process + predefined list size for tail copy
// 	this is also relevant for attributes/floats insertions

/*
	command_logic_slider(LDCProcessState&) -> void (static,global) !O(1)
	purpose: attach transformable float value to entity (cmd = :slider)
	conforming to: void* interpreter_logic
*/
void command_logic_slider(LDCProcessState &state)
{
	state.clusters.back().elist.back().etype = SLIDER;
	state.clusters.back().cnt_slider++;
}

/*
	command_logic_return(LDCProcessState&) -> void (static,global) !O(1)
	purpose: set finalizing entity with set value return (cmd = :return)
	conforming to: void* interpreter_logic
*/
void command_logic_return(LDCProcessState &state)
{
	state.clusters.back().elist.back().tdata = stoi(state.cmd->tail);
	state.clusters.back().elist.back().etype = RETURN;
}

/*
	command_logic_error(LDCProcessState&) -> void (static,global) !O(1)
	purpose: this method catches invalid syntax/commands and notifies the developer
	conforming to: void* interpreter_logic
*/
void command_logic_syntax_error(LDCProcessState &state)
{ LDCCompiler::compiler_error_msg(state,"invalid command syntax"); }


// instruction handling references
static interpreter_logic interpreter_behaviour[LIST_LANGUAGE_COMMAND_COUNT+1] = {

	// definitions
	command_logic_cluster,command_logic_define,command_logic_describe,

	// attributes
	command_logic_fattributes,command_logic_sattributes,command_logic_segment,
	command_logic_condition,command_logic_link,

	// behaviours
	command_logic_subsequent,command_logic_sysbehaviour,command_logic_checkbox,command_logic_dropdown,
	command_logic_slider,command_logic_return,

	// fault
	command_logic_syntax_error
};

// command syntax
static std::string mlcmd[LIST_LANGUAGE_COMMAND_COUNT] = {

	// definitions
	"cluster","define","describe",

	// attributes
	"floats","strings","segment","condition","link",

	// behaviours
	"subsequent","system_behaviour","checkbox","dropdown","slider","return"
};


/**
 *		LDC Compiler Implementation
 *
 * The LDC Compiler reads a menu list definition .ldc file and compiles it to a struct complex.
 * This resulting complex can be used to write an intern list logic, which will be externally defined and
 * compiled at load time.
 *
 * 		List Definition Code Language Design
 * 
 * prefix ':' marks a definition statement in ldc, the following expression until ' ' or '\n'
 * following expressions are valid and will be handled by menu list interpreter:
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
 * 	:define <entity_name>
 * define the name of the list entity
 * 
 * 	:describe
 * 	<description>
 * to add a description to the list entity, that will be shown when currently selected
 *
 * 	:floats (<float><space>)*
 * add float constants to list entity, accessible for any reason, usage defined by logic
 *
 *	:strings (<string><space>)*
 * add string constant to list entity, accessible for any reason, usage defined by logic
 *
 *
 * 	:segment <segment_name>
 * whereever a segment is called within the list definition file, a list entry will be added, linked to
 * the position it was created in, but unrelated to the actual entity list.
 * this feature can be used to segment a list of entities into multiple parts, while not interrupting the list
 * itself.
 * 
 * 	:condition (<condition_id><space>)*
 * specify an amount of conditions to activate list entity.
 * the condition ids will read the respective booleans from an extern condition list.
 *
 *	:link <init_variable>
 * link attribute value to initialization variable.
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
 * this will attach a system functionality to the list entity, which will be executed when it is selected &
 * confirmed.
 * behaviour ids can be defined differently in custom logic, but it is highly discouraged due to possible
 * confusion, when standard behaviour differs. individual checks into custom logic are generally avoided.
 * 
 * 	:subsequent <cluster_name>
 * this entity will be linked to a sublist by it's clusters name.
 * 
 * 	:checkbox
 * sign this entity to mark it switchable between true or false states
 * 
 * 	:dropdown
 * 	<fist_option> <second_option> <third_option> ;
 * add a sublist/dropdown option to this entity, holding selectable options divided by a ' '
 * using :dropdown without defining options will still create a dropdown and the list can be
 * assembled later by directly writing elements into cattribs.
 * the semicolon (;) after a sublist options forcefully ends the option write.
 * if the last semicolon is missing, the last dropdown option definition lasts until next ldc command.
 * 
 * 	:slider
 * make this entity contain a changable floating point number to be adjusted by e.g. a slider
 * 
 * 	:return <output_value>
 * this entity returns given value on confirmation
 * 
 * 
 * The following implementation is the static compile function for LDC, holding the language interpreter
 * conforming to the above language definitions and statements.
 * The compiler only has one feature: compile, because this is what it does.
*/


/*
	!O(n)bm .defined entities from all clusters in file combined /function -> (static)
	purpose: load .ldc files, break it up into a command buffer and compile it into a complex
	\param path: path to .ldc file
	\param rClusters: reference to cluster list, the new clusters will be added into
	NOTE: don't kill me for calling it "compile", i just wanted to be cute & it kinda does compile ok
*/
void LDCCompiler::compile(const char* path,std::vector<LDCCluster> &rClusters)
{
	// setup process state
	LDCProcessState state = {
		.fpath = path,
		.clusters = rClusters
	};

	// first pass: extraction - iterate definition file
	std::vector<ListLanguageCommand> cmd_buffer;
	std::ifstream file(path,std::ios::in);
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

	// second pass: commands - execute extracted commands
	for (ListLanguageCommand &cmd : cmd_buffer) {
		state.cmd = &cmd;
		interpreter_behaviour[cmd.id](state);
	}

	// third pass: interpretation - temporary instruction data
	for (uint16_t i=0;i<state.clusters.size();i++) {
		LDCCluster &cc = state.clusters[i];

		// correlate child name to cluster id
		for (uint16_t j=0;j<state.crefs[i].parent_ids.size();j++) {
			uint16_t k = 0;
			while (state.clusters[k].id!=state.crefs[i].child_names[j]) k++;
			state.clusters[i].elist[state.crefs[i].parent_ids[j]].tdata = k;
		}

		// correlate initialization variable name to serialization id
		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
			uint32_t ivar = Init::find_iKey(state.srefs[i][j].c_str());
			if (ivar==InitVariable::VARIABLE_ERROR) {
				printf("%s: \033[31;1mvariable linking error.\033[0m referenced as \"\033[34;1m%s\033[0m\"\n",
						state.fpath,state.srefs[i][j].c_str());
				continue;
			}

			// store id and original values for writing purposes & override held entity data
			LDCEntity &ce = state.clusters[i].elist[state.clusters[i].linked_ids[j]];
			ce.vlink = ivar;
			ce.tdata = Init::iConfig[ivar];
		}

		// set segment jump write
		for (LDCSegment &segment : state.clusters[i].slist)
			state.clusters[i].elist[segment.position].jsegment = true;
	}
	// FIXME: the angry valgrind output should give the programmer some ideas what is to be fixed
}


/**
 *		Selection Splice Geometry Implementation
 *
 *  the selection splice geometry is a stilistical highlight for the UI.
 *  it not only adds graphical flavour, it can also be used to communicate selection to the user.
 *
 *  	here is some information on how this geometry can be defined:
 *  following, an ascii representation of the splice
 *					 ___uwe__
 *					/  uc   /
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
	shader.upload_camera(gCamera2D);
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
// TODO: head rotation transition between dialogue and main mode is snapping around and it looks terrible


/**
 *			Start Implementation of Vertical List for Main Menu:
 * 
 *	The Menu List interprets .ldc files and converts them into displayable lists with a variety of features.
 *
 *		The following enjoyable features are included:
 *	It is possible to navigate through lists by choosing their linked entity.
 *	Entity attributes can be linked to variables and modified through multiple stylish representations:
 *	-> sliders, dropdowns and checkboxes.
 *	Loaded savestates are easily represented through Menu List, due to function overload.
 *
 *		Special definition rules:
 *	To define a globe preview by rotation:
 *		add "grotation" to strings component and add target coordinates (6 floats) to floats component
 *	To define an abnormal text colouration:
 *		add "colour" to strings component and add target colour vector (4 floats) to floats component
 *	To define a challenge preview banner:
 *		add "diff_id" to strings component and add difficulty id (1 unsigned 8-bit integer) to floats component
 *		(difficulty ids: 1-6 student; 7-11 master; 12-14 grandmaster; 15-16 headmaster)
 *	Should commands be required alongside dropdown elements:
 *		first call :dropdown command and then prefix any further commands with "cmd" in strings component
 *		NOTE: :dropdown has to be called before :strings cmd
 *
 *		How 2 use:
 *	First run define_list(...) to load list definition. (can be done through .ldc file or loaded savestates)
 *	After completing all definitions, load() shall be executed.
 *	Lists are closed by default. Should the necessity arise call open_list(...) to activate.
 *	When the List is done being useful, just close it again by calling close_list(...).
 *	If an entity is supposed to stand out for some reason, it can be recoloured by calling discolour(...).
 *	Finalize variable modification with write_attributes(...)/reset_attributes(...), depending on circumstance.
 *	The update works by calling update in the base render stage and the other component in the msaa stage.
 *	Finally, the last update_overlays(...) belongs right after joining all other framebuffers
 *	Communication about any list being open at any given time can be requested through .system_active.
 *	Also the result of linked_variables_changed(...) informs the user about potentially changed attributes.
 *
 *		TODO: features & optimization
 *	- optimize drawing, geometry and everything else that is very wrong here
 *	- wiggly text shadow
 *	- fading between lists (tilt shift effect, smooth transition between background and foreground)
 *
 * TODO: add an option to define what will be shown if the condition wasn't met (greyed,???,gfx replacements)
*/

/*
	!O(n)bm .list entities /+load -> (public)
	purpose: add lists to menu list complex collection based on .ldc definition file at given path
	\param path: path to .ldc definition file
	\returns memory index of first list added to menu list complex collection
*/
uint8_t MenuList::define_list(const char* path)
{
	// execute definition code
	std::vector<LDCCluster> clusters;
	LDCCompiler::compile(path,clusters);
	uint8_t out = mlists.size();

	// entity creation
	uint8_t lidx = out;
	mlists.resize(out+clusters.size());
	for (LDCCluster &cluster : clusters) {
		mlists[lidx] = {
			.full_range = cluster.elist.size()+cluster.slist.size()-1,
			.entities = std::vector<MenuListEntity>(cluster.elist.size()),
			.segments = std::vector<MenuListSegment>(cluster.slist.size()),
			.description = Text(de_font),
			.checkbox_ids = std::vector<uint16_t>(cluster.cnt_checkbox),
			.dropdown_ids = std::vector<uint16_t>(cluster.cnt_dropdown),
			.slider_ids = std::vector<uint16_t>(cluster.cnt_slider),
			.link_ids = cluster.linked_ids
		};

		// write segment information in-between list elements
		for (uint8_t i=0;i<cluster.slist.size();i++) {
			MenuListSegment t_segment = {
				.position = cluster.slist[i].position,
				.text = Text(st_font),
				.tlen = cluster.slist[i].title.length()
			};

			// setup segment render
			t_segment.text.add(
					cluster.slist[i].title.c_str(),
					glm::vec2(
						MENU_LIST_HEADPOS_X+MENU_LIST_SEGMENT_PUSH_X,
						MENU_LIST_SCROLL_START-(cluster.slist[i].position+i)*MENU_LIST_SCROLL_Y
				)),t_segment.text.load();
			mlists[lidx].segments[i] = t_segment;
		}

		// process cluster entities
		uint16_t head_checkbox = 0,head_dropdown = 0,head_slider = 0;
		float vscroll = MENU_LIST_SCROLL_START;
		for (uint16_t i=0;i<cluster.elist.size();i++) {

			// dodge scroll to prevent segment override with list element
			vscroll -= MENU_LIST_SCROLL_Y*cluster.elist[i].jsegment;

			// create entity
			MenuListEntity t_entity = {
				.etype = cluster.elist[i].etype,
				.value = cluster.elist[i].tdata,
				.link_id = cluster.elist[i].vlink,
				.text = Text(st_font),
				.tlen = cluster.elist[i].head.length()
			};

			// fill in element list information
			t_entity.text.add(cluster.elist[i].head.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),
				t_entity.text.load();
			mlists[lidx].description.add(
					cluster.elist[i].description.c_str(),
					MENU_LIST_DESC_POSITION-glm::vec2(0,MATH_CARTESIAN_YRANGE*i),
					MENU_LIST_DESC_BLOCKWIDTH,
					MENU_LIST_DESC_NLINEJMP
				);
			mlists[lidx].dtlen += cluster.elist[i].description.length();

			// iterate through possible dropdown elements in character attribute space
			bool element_mode = cluster.elist[i].etype==LDCEntityType::DROPDOWN;
			uint8_t cai = 0;
			while (element_mode&&cai<cluster.elist[i].cattribs.size())
				element_mode = cluster.elist[i].cattribs[cai++]!="cmd";

			// iterate additional commands in character attribute space
			uint8_t fai = 0;
			for (uint8_t j=cai;j<cluster.elist[i].cattribs.size();j++) {
				if (cluster.elist[i].cattribs[j]=="colour") {
					t_entity.colour = glm::vec4(
							cluster.elist[i].fattribs[fai],
							cluster.elist[i].fattribs[fai+1],
							cluster.elist[i].fattribs[fai+2],
							cluster.elist[i].fattribs[fai+3]
						);
					fai += 4;
				}
				else if (cluster.elist[i].cattribs[j]=="diff_id") {
					t_entity.diff_preview = cluster.elist[i].fattribs[fai++];
				}
				else if (cluster.elist[i].cattribs[j]=="grotation") {
					t_entity.has_rotation = true;
					t_entity.grotation = glm::vec2(
							cluster.elist[i].fattribs[fai],
							cluster.elist[i].fattribs[fai+3]
						);
					fai += 6;
					// TODO: improve rotation precision beyond simple degree reading
				}
				else {
					printf("syntax error in character attribute commands: %s\n",
						   cluster.elist[i].cattribs[j].c_str());
				}
			}

			// load checkbox geometry
			switch (cluster.elist[i].etype) {
			case LDCEntityType::CHECKBOX:
				create_checkbox(vscroll);
				mlists[lidx].checkbox_ids[head_checkbox++] = i;
				break;

			// load dropdown options
			case LDCEntityType::DROPDOWN:
				t_entity.dd_options.resize(cluster.elist[i].cattribs.size());
				t_entity.dd_colours.resize(cluster.elist[i].cattribs.size());
				t_entity.dd_length.resize(cluster.elist[i].cattribs.size());
				for (uint8_t di=0;di<cai;di++) {
					Text dd_text = Text(st_font);
					dd_text.add(cluster.elist[i].cattribs[di].c_str(),
							glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,vscroll)),
						dd_text.load();
					t_entity.dd_options[di] = dd_text;
					t_entity.dd_colours[di] = glm::vec4(1.f);
					t_entity.dd_length[di] = cluster.elist[i].cattribs[di].length();
				} mlists[lidx].dropdown_ids[head_dropdown++] = i;
				break;

			// load slider geometry
			case LDCEntityType::SLIDER:
				create_slider(vscroll);
				mlists[lidx].slider_ids[head_slider++] = i;
			};

			// move cursor to write next element & store current information
			vscroll -= MENU_LIST_SCROLL_Y;
			mlists[lidx].entities[i] = t_entity;
		}

		// store resulting cluster
		mlists[lidx].description.load();
		lidx++;
	} return out;
}

/*
	!O(n)bm .save states /load -> (public)
	purpose: define a single new list based on savestate data. one entry per savestate
	\param states: savestate format, already loaded. not a path to the save file
	\returns memory index of savestate list in menu list collection
*/
uint8_t MenuList::define_list(SaveStates states)
{
	// create singular complex without segments for a simple state list
	MenuListComplex mlc = {
		.full_range = states.saves.size()-1,
		.entities = std::vector<MenuListEntity>(states.saves.size()),
		.description = Text(st_font)
	};

	// iterate save data and create a state list with a proportionally linear relationship
	int32_t vscroll = MENU_LIST_SCROLL_START;
	for (uint16_t i=0;i<states.saves.size();i++) {
		SaveData &state = states.saves[i];
		MenuListEntity mle = {
			.colour = diff_colours[state.diff],
			.etype = LDCEntityType::RETURN,
			.value = i,
			.has_rotation = true,
			.grotation = glm::vec2(0),
			.text = Text(st_font),
			.tlen = state.title.length()
		};

		// write save title
		mle.text.add(state.title.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),mle.text.load();
		mlc.description.add(
				state.description.c_str(),
				MENU_LIST_DESC_POSITION-glm::vec2(0,MATH_CARTESIAN_YRANGE*i),
				MENU_LIST_DESC_BLOCKWIDTH,
				MENU_LIST_DESC_NLINEJMP
			);
		mlc.dtlen += state.description.length();
		// TODO: add correct global preview, when savestate is finally realized and allows such storage

		// scroll list cursor
		vscroll -= MENU_LIST_SCROLL_Y;
		mlc.entities[i] = mle;
	}

	// communicate possiblilty of empty save data
	if (!states.saves.size()) {
		std::string err_message = "no save data";
		MenuListEntity decoy = {
			.colour = TEXT_ERROR_COLOUR,
			.etype = LDCEntityType::UNDEFINED,
			.text = Text(st_font),
			.tlen = err_message.length()
		};
		decoy.text.add(err_message.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),decoy.text.load();
		mlc.entities.push_back(decoy);
		mlc.full_range = 0;
	}

	// output result
	mlc.description.load();
	mlists.push_back(mlc);
	return mlists.size()-1;
}

/*
	!O(1) /load -> (public)
	purpose: setup vertices & compile shaders for entity attribute visualization
	\param r2d: reference to 2D renderer
*/
void MenuList::load(CascabelBaseFeature* ccbf,uint16_t wsid)
{
	// setup dropdown background
	float ddbgr_vertices[] = {

		// setup spanning
		MENU_LIST_ATTRIBUTE_HTARGET,0,0,
		MENU_LIST_ATTRIBUTE_WTARGET,MENU_LIST_ATTRIBUTE_THEIGHT,2,
		MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_ATTRIBUTE_THEIGHT,2,
		MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START,1,
		MENU_LIST_ATTRIBUTE_WTARGET,MENU_LIST_SCROLL_START,1,
		MENU_LIST_ATTRIBUTE_HTARGET,720,0,

		// content box
		MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START,1,
		MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_ATTRIBUTE_THEIGHT,2,
		MENU_LIST_ATTRIBUTE_WTARGET,MENU_LIST_SCROLL_START,1,
		MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_ATTRIBUTE_THEIGHT,2,
		MENU_LIST_ATTRIBUTE_WTARGET,MENU_LIST_ATTRIBUTE_THEIGHT,2,
		MENU_LIST_ATTRIBUTE_WTARGET,MENU_LIST_SCROLL_START,1
	}; ddbgr_buffer.bind(),ddbgr_buffer.upload_vertices(ddbgr_vertices,sizeof(ddbgr_vertices));

	// dropdown shader setup
	ddbgr_shader.compile("./shader/main_menu/vddbgr.shader","./shader/main_menu/fddbgr.shader");
	ddbgr_shader.def_attributeF("position",2,0,3);
	ddbgr_shader.def_attributeF("dmod",1,2,3);
	ddbgr_shader.upload_int("jmp_dist",MENU_LIST_SCROLL_Y);
	ddbgr_shader.upload_camera(gCamera2D);

	// setup checkbox corpi
	checkbox_buffer.bind(),checkbox_buffer.upload_vertices(checkbox_vertices);
	checkbox_shader.compile("./shader/main_menu/vcheckbox.shader","./shader/main_menu/fcheckbox.shader");
	checkbox_shader.def_attributeF("position",2,0,4);
	checkbox_shader.def_attributeF("drift_mod",2,2,4);
	checkbox_shader.upload_camera(gCamera2D);

	// setup slider corpi
	slider_buffer.bind(),slider_buffer.upload_vertices(slider_vertices);
	slider_shader.compile("./shader/main_menu/vslider.shader","./shader/main_menu/fslider.shader");
	slider_shader.def_attributeF("position",2,0,3);
	slider_shader.def_attributeF("bmod",1,2,3);
	slider_shader.upload_float("max_disp",MENU_LIST_ATTRIBUTE_WIDTH);
	slider_shader.upload_camera(gCamera2D);

	// difficulty preview spritesheet
	m_ccbf = ccbf;
	rid_diffs = m_ccbf->r2d->add(glm::vec2(950,550),250,50,"./res/menu/est_diff.png",16,2,30,0);
	m_ccbf->r2d->add(glm::vec2(-125,-25),250,50,"./res/menu/est_diff.png",16,2,30,0);

	// globe render target
	rid_globe = m_ccbf->r3d->add_physical("./res/terra.obj","./res/terra/albedo.jpg","./res/terra/norm.png",
			"./res/terra/materials.png","./res/none.png",glm::vec3(0),1,glm::vec3(0));
	gb_lights.add_sunlight({ glm::vec3(-50,25,25),glm::vec3(1),10.f });
	globe_target_id = m_ccbf->r3d->add_target(m_ccbf->frame);
	fb_globe = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/fbf_standard.shader",false);
	rid_window_sprite = wsid;
	// TODO: replace emission map with nighttime extrapolation in terra directory
}

/*
	!O(1)m /function -> (public)
	purpose: transfer list referenced by given memory index into opening state
	\param id: list index
*/
void MenuList::open_list(uint8_t id)
{
	active_ids.push_back(id);
	system_active = true,tf_list_opened = true;
}
// TODO: this will increase in complexity and also will be a recurring pattern. etc, do what must be done kuro

/*
	!O(n)m .amount of stored ids /function -> (public)
	purpose: transfer list referenced by given memory index into closing state
	\param id: list index
*/
void MenuList::close_list(uint8_t id)
{
	active_ids.erase(std::remove(active_ids.begin(),active_ids.end(),id),active_ids.end());
	// this will become more complex in the future it belongs in the cpp
}
// TODO: recurring pattern, this also happens in renderer btw, write a shortcut for that kind of id processing

/*
	!O(n) .linked attributes /function -> (public)
	purpose: write changed attributes in list to initialization, when linked successfully
	\param id: id of list complex to write attributes of
	NOTE: initalization write to file has to be performed seperately
*/
void MenuList::write_attributes(uint8_t id)
{
	for (uint16_t lid : mlists[id].link_ids) {
		MenuListEntity &e = mlists[id].entities[lid];
		Init::iConfig[e.link_id] = e.value;
	}
}

/*
	!O(n2)bn .list entities .dropdown entities /function -> (public)
	purpose: reset changed attributes in list to original state of initialization, when linked successfully
	\param id: id of list complex to reset attributes of
	NOTE: not possible with attributes, that are not linked. reset values rely on initalization memory
*/
void MenuList::reset_attributes(uint8_t id)
{
	for (uint16_t lid : mlists[id].link_ids) {
		MenuListEntity &e = mlists[id].entities[lid];
		e.value = Init::iConfig[e.link_id];
	}
}

/*
	TODO
*/
uint8_t MenuList::update(int8_t vdir,int8_t hdir,glm::vec2 mpos,int8_t mscroll,bool conf,bool ntconf,bool back,
		bool mperiph,bool &rrnd)
{
	// early exit when no lists active
	system_active = active_ids.size();
	instruction_mod = 0;
	show_globe = false;
	if (!active_ids.size()) return 0;

	// update most recent list
	uint8_t out = 0;
	if (!subfunc_opened) {
		crr_select = 0;
		MenuListComplex &crr = mlists[active_ids.back()];

		// escape handling
		if (back) {
			rrnd = true;
			close_list(active_ids.back());
			if (active_ids.size()) return mlists[active_ids.back()].lselect;
			return 0;
		}

		// set selection by rasterized mouse position
		uint8_t cmp_select = crr.lselect,cmp_scroll = crr.lscroll;
		if (mperiph) {
			int16_t nselect = (MENU_LIST_SCROLL_START-mpos.y)/MENU_LIST_SCROLL_Y;
			crr.lselect = (nselect<7) ? nselect*(nselect>0) : 7;
			crr.lscroll -= mscroll*((crr.lscroll>0||mscroll<0)
					&& (crr.lscroll<((int16_t)crr.full_range-7)||mscroll>0));
		}

		// update selection by directional input
		else {
			int8_t dt_select = crr.lselect+vdir;
			int8_t dt_scroll = crr.lscroll+dt_select*(dt_select<0)+(dt_select-7)*(dt_select>7);
			crr.lselect = (dt_select<0) ? 0 : (dt_select>7) ? 7 : dt_select;
			crr.lscroll = (dt_scroll<0) ? 0 : dt_scroll;
		}

		// clamp selection to last element
		uint16_t eindex = crr.lscroll+crr.lselect;
		crr.lselect -= (eindex-crr.full_range)*(eindex>crr.full_range);

		// find index of selected entity & check if selection intersects segment
		crr_select = crr.lscroll+crr.lselect;
		seg_passed = 0;
		bool seg_select = false;
		for (MenuListSegment &seg : crr.segments) {
			if (crr_select<=seg.position) {
				seg_select = crr_select==seg.position;
				break;
			} seg_passed++,crr_select--;
		}
		MenuListEntity &ce = crr.entities[crr_select];
		instruction_mod = (ce.etype-LDCEntityType::CHECKBOX)*(ce.etype<LDCEntityType::RETURN);

		// protection for selected segments
		if (seg_select) {
			int8_t dt_select = -1+2*(vdir>0||mperiph||!seg_passed);
			crr.lselect += dt_select,crr_select -= dt_select<0;
		}
		// FIXME: one too many scroll, when selection tries to exceed last entity.

		// slider manipulation by input
		if (ce.etype==LDCEntityType::SLIDER) {
			if (ntconf&&mperiph&&mpos.x>MENU_LIST_ATTRIBUTE_COMBINE&&mpos.x<MENU_LIST_ATTRIBUTE_WTARGET)
				ce.value = ((mpos.x-MENU_LIST_ATTRIBUTE_COMBINE)/MENU_LIST_ATTRIBUTE_WIDTH)*100;
			else if (hdir) {
				int16_t nvalue = ce.value+hdir;
				ce.value = (nvalue<0) ? 0 : (nvalue>100) ? 100 : nvalue;
			}
		}

		// confirmation handling
		else if (conf) {
			switch (ce.etype) {
			case LDCEntityType::DROPDOWN:
				subfunc_opened = true;
				conf = false;
				break;
			case LDCEntityType::SUBSEQUENT:
				open_list(ce.value);
				rrnd = true;
				break;
			case LDCEntityType::CHECKBOX: ce.value = !ce.value;
				break;
			default:
				status = ce.value*(ce.etype==LDCEntityType::RETURN);
			};
		}

		// selection geometry data manipulation
		out = mlists[active_ids.back()].lselect;
		rrnd = (crr.lselect!=cmp_select)||(crr.lscroll!=cmp_scroll)||rrnd||tf_list_opened;
		tf_list_opened = false;
	}

	// update attribute subfunctionality for dropdown options if obsoletion check fails
	uint8_t id = active_ids.back();
	if (!subfunc_opened) return out;

	// setup
	MenuListEntity &e = mlists[id].entities[crr_select];
	uint8_t cap_options = e.dd_options.size()-1;
	instruction_mod = 0;

	// input handling for sublist
	if (mperiph) {
		uint16_t box_start = MENU_LIST_SCROLL_START-MENU_LIST_SCROLL_Y*(crr_select-e.value);
		int8_t mshover = (box_start-mpos.y)/MENU_LIST_SCROLL_Y-seg_passed;
		mshover = (mshover<0) ? 0 : (mshover>cap_options) ? cap_options : mshover;
		e.dd_colours[mshover] = glm::vec4(0,.7f,.7f,1.f);
		e.value = conf ? mshover : e.value;
	} else {
		int8_t nvalue = e.value+vdir;
		e.value = (nvalue<0) ? 0 : (nvalue>cap_options) ? cap_options : nvalue;
	}

	// confirmation handling for current selection
	subfunc_opened = !back&&!conf;
	return out;
}
// FIXME: bad dog, no bisquits!
// FIXME: code & processing repitition due to update/render split

/*
	TODO
*/
void MenuList::render()
{
	// skip if no lists active
	if (!system_active) return;
	uint8_t id = active_ids.back();

	// update attribute subfunctionality for dropdown options
	if (subfunc_opened) {
		MenuListEntity &e = mlists[id].entities[crr_select];

		// draw dropdown background
		ddbgr_buffer.bind(),ddbgr_shader.enable();
		ddbgr_shader.upload_int("selection",mlists[id].lselect-e.value);
		ddbgr_shader.upload_int("ddsize",e.dd_options.size()-1);
		glDrawArrays(GL_TRIANGLES,0,12);

		// write dropdown elements of selection
		for (uint8_t ddi=0;ddi<e.dd_options.size();ddi++) {
			Text &ddtx = e.dd_options[ddi];
			ddtx.prepare(),ddtx.set_scroll(glm::vec2(0,-MENU_LIST_SCROLL_Y*(ddi-e.value)));
			ddtx.render(e.dd_length[ddi],e.dd_colours[ddi]);
			e.dd_colours[ddi] = glm::vec4(1.f);
		}
	}

	// write dropdown elements
	else {
		for (uint16_t &ddi : mlists[id].dropdown_ids) {
			MenuListEntity &e = mlists[id].entities[ddi];
			e.dd_options[e.value].prepare();
			e.dd_options[e.value].set_scroll(glm::vec2(0,0));
			e.dd_options[e.value].render(e.dd_length[e.value],glm::vec4(1.f));
		}
	}
	// TODO: in DIRE need of optimization once the text implementation is fixed!

	// draw active lists
	crr_scroll = mlists[id].lscroll*MENU_LIST_SCROLL_Y;
	for (MenuListSegment &s : mlists[id].segments)
		s.text.prepare(),s.text.set_scroll(glm::vec2(0,crr_scroll)),s.text.render(s.tlen,TEXT_SEGMENT_COLOUR);
	for (MenuListEntity &e : mlists[id].entities)
		e.text.prepare(),e.text.set_scroll(glm::vec2(0,crr_scroll)),e.text.render(e.tlen,e.colour);

	// description out
	mlists[id].description.prepare(),mlists[id].description.set_scroll(glm::vec2(0,crr_select*720.f)),
		mlists[id].description.render(mlists[id].dtlen,glm::vec4(1));
	// FIXME: calculating scroll matrix twice due to braindead individual scroll on text

	// globe render setup
	show_globe = mlists[id].entities[crr_select].has_rotation;
	globe_rotation = mlists[id].entities[crr_select].grotation;

	// difficulty prognosis text animation
	if (!mlists[id].entities[crr_select].diff_preview) return;
	anim_prog -= 2*MATH_PI*(anim_prog>2*MATH_PI);
	float diff_scale = 1.f+glm::sin(anim_prog)*.4f;
	float diff_rotation = glm::sin(anim_prog*2.f)*45.f;
	glm::mat4 diff_model = glm::translate(glm::mat4(1.0f),glm::vec3(1075,575,0));
	diff_model = glm::scale(diff_model,glm::vec3(diff_scale,diff_scale,0));
	diff_model = glm::rotate(diff_model,glm::radians(diff_rotation),glm::vec3(0,0,1));
	m_ccbf->r2d->al[rid_diffs+1].model = diff_model;
	anim_prog += .02f;

	// difficulty prognosis render
	uint8_t rstate = mlists[id].entities[crr_select].diff_preview-1;
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(rid_diffs,glm::vec2(0,rstate));
	m_ccbf->r2d->render_state(rid_diffs+1,glm::vec2(1,rstate));
	// TODO: break apart difficulty prognosis text and belt colours
}

/*
	!O(n)e .amount of sliders and checkboxes /update -> (public)
	purpose: draw section of inverted/multisampled background components
	\param anim_delta: predefined and commonly used animation speed for all menu transitions
*/
void MenuList::update_background_component()
{
	// prepare checkboxes
	if (!active_ids.size()||subfunc_opened) return;
	uint16_t id = active_ids.back();
	checkbox_buffer.bind(),checkbox_shader.enable();
	checkbox_shader.upload_float("scroll",crr_scroll);

	// iterate checkboxes
	for (uint16_t i=0;i<mlists[id].checkbox_ids.size();i++) {
		MenuListEntity &ce = mlists[id].entities[mlists[id].checkbox_ids[i]];
		Toolbox::transition_float_on_condition(ce.anim_transition,MainMenu::transition_delta,ce.value);
		checkbox_shader.upload_float("aprog",ce.anim_transition);
		glDrawArrays(GL_TRIANGLES,12*i,12);
	}

	// prepare sliders
	slider_buffer.bind(),slider_shader.enable();
	slider_shader.upload_float("scroll",crr_scroll);

	// iterate sliders
	for (uint16_t i=0;i<mlists[id].slider_ids.size();i++) {
		MenuListEntity &ce = mlists[id].entities[mlists[id].slider_ids[i]];
		slider_shader.upload_float("sval",ce.value*.01f);
		glDrawArrays(GL_TRIANGLES,6*i,6);
	}
}
// FIXME: single drawcalls for every slider and checkbox seems like a horrible idea

/*
	TODO
*/
void MenuList::update_overlays()
{
	// early exit if globe render can be skipped
	if (!show_globe) return;

	// transform globe towards preview location
	m_ccbf->r3d->pml[rid_globe].model = glm::rotate(
			glm::rotate(glm::mat4(1),glm::radians(globe_rotation.x),glm::vec3(1,0,0)),
			glm::radians(globe_rotation.y),glm::vec3(0,1,0)
		);

	// render globe to target
	glEnable(GL_DEPTH_TEST),glDisable(GL_BLEND);
	m_ccbf->r3d->start_target(globe_target_id);
	m_ccbf->r3d->prepare_pmesh(gb_cam3D),m_ccbf->r3d->render_pmsh(rid_globe);
	glDisable(GL_DEPTH_TEST),glEnable(GL_BLEND);

	// globe deferred shading
	fb_globe.bind();
	Frame::clear();
	m_ccbf->r3d->render_target(globe_target_id,gb_cam3D,&gb_lights);
	FrameBuffer::close();

	// draw globe buffer
	if (show_globe) {
		m_ccbf->r2d->prepare();
		m_ccbf->r2d->s2d.upload_float("vFlip",1.f);
		m_ccbf->r2d->render_sprite(globe_target_id,globe_target_id+1,fb_globe.tex);
		m_ccbf->r2d->s2d.upload_float("vFlip",.0f);
	}
}

/*
	!O(n) .linked attributes /+function -> (public)
	purpose: check if linked variables of desired list have changed compared to initialization
	\param list_id: memory index of list in question
	\returns true if any linked attributes are not identical with initialization, else false
*/
bool MenuList::linked_variables_changed(uint16_t list_id,bool& reload)
{
	bool out = false;
	for (uint16_t id : mlists[list_id].link_ids) {
		MenuListEntity &ce = mlists[list_id].entities[id];
		bool changed = ce.value!=Init::iConfig[ce.link_id];
		out = out||changed;
		reload = reload||(iKeys[ce.link_id].restart_system_on_change&&changed);
	} return out;
}
// TODO: extract more information to display a list of changes later

/*
	!O(1)m /+load -> (private)
	purpose: store checkbox information
	\param vscroll: current text scroll to align checkbox with it's corresponding entity
*/
void MenuList::create_checkbox(float vscroll)
{
	// prepare lower-right corner with scroll
	float ledge = vscroll-MENU_LIST_HEAD_SIZE;
	float cedge = vscroll-MENU_LIST_HEAD_HSIZE;
	std::vector<float> t_vertices = {

		// upper triangle
		MENU_LIST_ATTRIBUTE_COMBINE,vscroll,0,MENU_LIST_CHECKBOX_DRIFT_DIST,
		MENU_LIST_ATTRIBUTE_HQUADRATIC,cedge,0,MENU_LIST_CHECKBOX_DRIFT_DIST,
		MENU_LIST_ATTRIBUTE_QUADRATIC,vscroll,0,MENU_LIST_CHECKBOX_DRIFT_DIST,

		// right triangle
		MENU_LIST_ATTRIBUTE_QUADRATIC,ledge,MENU_LIST_CHECKBOX_DRIFT_DIST,0,
		MENU_LIST_ATTRIBUTE_QUADRATIC,vscroll,MENU_LIST_CHECKBOX_DRIFT_DIST,0,
		MENU_LIST_ATTRIBUTE_HQUADRATIC,cedge,MENU_LIST_CHECKBOX_DRIFT_DIST,0,

		// lower triangle
		MENU_LIST_ATTRIBUTE_COMBINE,ledge,0,-MENU_LIST_CHECKBOX_DRIFT_DIST,
		MENU_LIST_ATTRIBUTE_QUADRATIC,ledge,0,-MENU_LIST_CHECKBOX_DRIFT_DIST,
		MENU_LIST_ATTRIBUTE_HQUADRATIC,cedge,0,-MENU_LIST_CHECKBOX_DRIFT_DIST,

		// left triangle
		MENU_LIST_ATTRIBUTE_COMBINE,vscroll,-MENU_LIST_CHECKBOX_DRIFT_DIST,0,
		MENU_LIST_ATTRIBUTE_COMBINE,ledge,-MENU_LIST_CHECKBOX_DRIFT_DIST,0,
		MENU_LIST_ATTRIBUTE_HQUADRATIC,cedge,-MENU_LIST_CHECKBOX_DRIFT_DIST,0
	}; checkbox_vertices.insert(slider_vertices.end(),t_vertices.begin(),t_vertices.end());
}
// FIXME: similar issues apply as for slider implementation
// FIXME: 6 float width is too heavy for this purpose

/*
	!O(1)m /+load -> (private)
	purpose: store slider information
	\param vscroll: current text scroll to align slider with it's corresponding entity
*/
void MenuList::create_slider(float vscroll)
{
	float lcorner = vscroll-MENU_LIST_HEAD_SIZE;
	float mcorner = vscroll-MENU_LIST_HEAD_HSIZE;
	std::vector<float> t_vertices = {

		// concave trimming
		MENU_LIST_ATTRIBUTE_COMBINE-MENU_LIST_SLIDER_XPUSH,lcorner,0,
		MENU_LIST_ATTRIBUTE_COMBINE,mcorner,1,
		MENU_LIST_ATTRIBUTE_COMBINE-MENU_LIST_SLIDER_XPUSH,vscroll,0,

		// concave widening
		MENU_LIST_ATTRIBUTE_WTARGET+MENU_LIST_SLIDER_XPUSH,lcorner,0,
		MENU_LIST_ATTRIBUTE_WTARGET+MENU_LIST_SLIDER_XPUSH,vscroll,0,
		MENU_LIST_ATTRIBUTE_COMBINE,mcorner,1
	}; slider_vertices.insert(slider_vertices.end(),t_vertices.begin(),t_vertices.end());
}
// FIXME: really? we allocate per slider? that is really dumb and lazy!
// FIXME: sliders, checkbox & dropdown boxing is slightly off-center
// TODO: creating the new temporary vertex list and then concat is very slow and simple minded


/**
 *		Start Implementation of Popup Dialogue for Main Menu:
 *
 *	the MenuDialogue utilizes the ldc list language to display a dialogue window, prompting the player to
 *	make a choice. it is a secondary, attention-grabbing way to display a short list of options, that are meant
 *	to be answered as an immediate response to proceed, without the power of a full MenuList.
 *
 *		following, some explanations about the geometrical behaviour:
 *
 *				     __________fw___________
 *									   ____.ur		
 *					 ul._____----------   /		|
 *				       /		        /		|
 *				      /      .c       /			| fh
 *				     /            __/.lr		|
 *				 ll./______-------				|
 *
 *		what to learn from this beautiful drawing:
 *	the point c marks the parametrical center of the created dialogue. from there, the dimensions are applied.
 *	fw is the full parametrical width and fh is the full height.
 *	the points ur and ll are predictably placed. ur will be at c+(fw/2,fh/2) and ll at c-(fw/2,fh/2).
 *	ul and lr will be spanned over the points c, ur & ll, being placed pseudorandomly within a range for
 *	asthetic reasons.
 *	the dialogue selector scaling is based on the text size of the listed options.
 *
 *		Custom Definition Rules:
 *	Just one rule (so far). to transform an ui component in 2D space:
 *		use the first two float components of the first entity in the cluster as transformation vector
 *
 *		how this feature works:
 *	Call add_dialogue_window() to create a new dialogue window and receive instance id back.
 *	After creating all windows run load().
 *	Dialogues are closed by default. to begin opening/closing process run open_dialogue()/close_dialogue().
 *	Insert update() into unaltered menu render stage.
 *	Insert background_component() into intersectionally inverting, anti-aliased menu render stage.
 *	Return from choice is stored in dg_state while the frame the choice was confirmed in.
 *	Modification of dialogue data list is possible from outside by directly modifying dg_data.
*/

/*
	add_dialogue_window(const char*,vec2,float,float,uint8_t,uint8_t) -> uint8_t !O(n)bm
	purpose: create new callable dialogue window
	\param path: path to list definition file written in ldc script language (as defined above)
	\param center: central point of dialogue background, dialogue geometry will be spread from there
	\param width: dialogue target spanning width (will not be reached on both sides)
	\param height: dialogue target spanning height (will not be reached on both sides)
	\param tsize: text size for dialogue options
	\param dsize: text size for dialogue descriptions
	\returns memory reference id for !root! dialogue. subsequents will always be stored in-order next
*/
uint8_t MenuDialogue::add_dialogue_window(const char* path,glm::vec2 center,float width,float height,
		uint8_t tsize,uint8_t dsize)
{
	// content information extraction
	std::vector<LDCCluster> clusters;
	LDCCompiler::compile(path,clusters);

	// setup dialogue fonts
	Font tfont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",tsize,tsize);
	Font ofont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",dsize,dsize);
	Font dfont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25);
	// FIXME: if my font implementation wouldn't be that ass i could just use variable sized texts (mdc)

	// process all clusters as dialogues
	uint8_t out = dg_data.size(),cidx = out;
	dg_data.resize(out+clusters.size());
	for (LDCCluster &cluster : clusters) {

		// store option count of current dialogue window
		size_t opcount = cluster.elist.size();

		// on-demand translation handling through ldc floats command
		glm::vec2 t_center = center;
		if (cluster.elist[0].fattribs.size()>1)
			t_center += glm::vec2(cluster.elist[0].fattribs[0],cluster.elist[0].fattribs[1]);

		// setup dialogue data with list start position
		SingularDialogueData &dgd = dg_data[cidx++];
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
		dgd.title_length = cluster.id.length();

		// setup memory for jump action to subsequent dialogues
		dgd.action_id.resize(opcount);
		dgd.action_value.resize(opcount);

		// dialogue option text setup
		for (uint8_t i=0;i<opcount;i++) {

			// action setup
			dgd.action_id[i] = cluster.elist[i].etype-(uint8_t)LDCEntityType::RETURN;
			dgd.action_value[i] = cluster.elist[i].tdata;

			// information print setup
			dgd.tx_options.add(
					cluster.elist[i].head.c_str(),
					glm::vec2(
						t_center.x-hwidth*MENU_DIALOGUE_OFFSET_FACTOR,
						dgd.liststart_y-dsize*i));
			dgd.tx_descriptions.add(cluster.elist[i].description.c_str(),glm::vec2(1030,350-720*i),
					200.f,20.f);

			// calculate characters to draw for each text
			dgd.option_length += cluster.elist[i].head.length();
			dgd.description_length += cluster.elist[i].description.length();
		} dgd.tx_options.load(),dgd.tx_descriptions.load();

		// additional data
		dgd.max_options = opcount-1;
		dgd.max_width = hwidth,dgd.max_height = hheight;

	// return parent id
	} return out;
}

/*
	load() -> void !O(1)
	purpose: compile shaders & setup dialogue buffers for background & selector
*/
void MenuDialogue::load()
{
	// upload background vertices
	bgr_buffer.bind();
	bgr_buffer.upload_vertices(bgr_verts);

	// background shader setup
	size_t vsize = sizeof(DialogueBackgroundGeometry);
	bgr_shader.compile("./shader/main_menu/vdialogue.shader","./shader/main_menu/fdialogue.shader");
	bgr_shader.def_irregular_attributeF("position",2,vsize,offsetof(DialogueBackgroundGeometry,position));
	bgr_shader.def_irregular_attributeI("disp_id",1,vsize,offsetof(DialogueBackgroundGeometry,disp_id));
	bgr_shader.upload_vec2("displace[0]",glm::vec2(0));
	bgr_shader.upload_camera(gCamera2D);

	// upload selector vertices
	slc_buffer.bind();
	slc_buffer.upload_vertices(slc_verts);

	// selector shader setup
	slc_shader.compile("./shader/main_menu/vdlgselector.shader","./shader/main_menu/fdlgselector.shader");
	slc_shader.def_attributeF("position",2,0,2);
	slc_shader.upload_camera(gCamera2D);
}
// TODO: store info of defined dialogue background vertices and create only once to avoid multiple allocations

/*
	open_dialogue(uint8_t) -> void !O(1)m
	purpose: declare referenced dialogue as currently opening
	\param did: dialogue id
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

	// set dialogue info active & reset state
	dg_data[did].dg_active = true;
}

/*
	close_dialogue(uint8_t) -> void !O(1)m
	purpose: declare referenced dialogue as currently closing and inactive
	\param did: dialogue id
*/
void MenuDialogue::close_dialogue(uint8_t did)
{
	// move dialogue id to closing state
	opening_ids.erase(std::remove(opening_ids.begin(),opening_ids.end(),did),opening_ids.end());
	active_ids.erase(std::remove(active_ids.begin(),active_ids.end(),did),active_ids.end());
	closing_ids.push_back(did);

	// set dialogue info closed
	dg_data[did].dg_active = false;
	dg_data[did].sindex = 0;
}

/*
	update(int8_t,float,bool,bool,bool) -> void !O(n)b
	purpose: handle inputs, update dialogue state & draw non-background components
	\param imv: vertical key/button input to navigate list
	\param mypos: y-axis component of mouse position for pointed selection
	\param mperiph: preferred peripheral: true for mouse; false for keyboard/controller
	\param conf: confirmation request
	\param back: closing request
*/
void MenuDialogue::update(int8_t imv,float mypos,bool mperiph,bool conf,bool back)
{
	// reset dialogue state & update if dialogue system is currently active
	dg_state = 0;
	system_active = opening_ids.size()||active_ids.size()||closing_ids.size();

	// check selector update conditions
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

		// confirmation condition & set dialogue response state
		if (conf) {
			dg_state = csdd->sindex;

			// handle possible system behaviour attached to confirmed option
			switch (dg_data[id].action_id[csdd->sindex]) {
				case 1: open_dialogue(dg_data[id].action_value[csdd->sindex]);
					break;
				case 2: close_dialogue(id);
					break;
			}
		}
	}
}
// FIXME: text tech results in ugly edge artifacts around letters after inverting to background (mdc)
// FIXME: avoid constant draw recalling, maybe better performance with triangle crunching (mdc)
// FIXME: (rare) flickering dialogue disappearance when closing dialogue (mdc)

/*
	TODO
*/
void MenuDialogue::render()
{
	// requirement, dialogue rendering can be skipped if system is currently inactive
	if (!system_active) return;

	// setup selector draw
	slc_buffer.bind();
	slc_shader.enable();

	// draw idle selectors
	for (uint8_t id : active_ids) {
		slc_shader.upload_float("mv_select",dg_data[id].sindex*dg_data[id].option_size);
		glDrawArrays(GL_TRIANGLES,id*6,6);
	}

	// requirement for title & description draw, any dialogue currently has to be in active selection
	if (!active_ids.size()) return;
	uint8_t id = active_ids.back();

	// show description of selected option
	dg_data[id].tx_descriptions.prepare();
	dg_data[id].tx_descriptions.set_scroll(glm::translate(glm::mat4(1.f),
			glm::vec3(.0f,720.f*dg_data[id].sindex,.0f)));
	dg_data[id].tx_descriptions.render(dg_data[id].description_length,glm::vec4(1.f));

	// write in-dialogue text
	for (uint8_t id : active_ids) {
		dg_data[id].tx_title.prepare();
		dg_data[id].tx_title.render(dg_data[id].title_length,glm::vec4(DIALOGUE_HEAD_COLOUR,1.f));
		dg_data[id].tx_options.prepare();
		dg_data[id].tx_options.render(dg_data[id].option_length,glm::vec4(DIALOGUE_OPTION_COLOUR,1.f));
	}
}

/*
	!O(n)b .compiled amount of relevant ids in processing lists /update -> (public)
	purpose: draw parts of dialogue as inverted background components on intersection with geometry
*/
void MenuDialogue::update_background_component()
{
	// skip if no dialogue active
	if (!system_active) return;

	// setup draw
	bgr_buffer.bind();
	bgr_shader.enable();

	// draw closing dialogues
	uint8_t i = 0;
	while (i<closing_ids.size()) {
		uint8_t id = closing_ids[i];
		draw_dialogue(id);

		// transition closing dialogues
		dg_data[id].dgtrans -= MainMenu::transition_delta;
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
		dg_data[id].dgtrans += MainMenu::transition_delta;
		if (dg_data[id].dgtrans>1.f) {
			dg_data[id].dgtrans = 1.f;
			active_ids.push_back(id);
			opening_ids.erase(opening_ids.begin()+i);
		} else i++;
	}
}

/*
	draw_dialogue(uint8_t) -> void (private) !O(1)
	purpose: generalization of upload & render call for any dialogue
	\param id: dialogue id
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
	interface_behaviour[OptionLogicID::MACRO_EXIT] = interface_behaviour_macro,
	interface_behaviour[OptionLogicID::OPTIONS] = interface_behaviour_options,
	interface_behaviour[OptionLogicID::EXTRAS] = interface_behaviour_extras,
	interface_behaviour[OptionLogicID::PRACTICE] = interface_behaviour_practice,
	interface_behaviour[OptionLogicID::LOAD] = interface_behaviour_load,
	interface_behaviour[OptionLogicID::CONTINUE] = interface_behaviour_continue,
	interface_behaviour[OptionLogicID::NEWGAME] = interface_behaviour_newgame;
	// TODO: a lot of performance and translation testing necessary to analyze the flipsides

	// asset load
	uint16_t rid_window_sprite = ccbf->r2d->sl.size();
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
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
		uint32_t wwidth = fnt_mopts.estimate_textwidth(main_options[i]);
		mo_twidth[i] = wwidth,mo_hwidth[i] = wwidth*.5f;
		mo_prog.x -= wwidth;
	} mo_prog /= glm::vec2(OptionLogicID::LOGIC_COUNT);
	glm::vec2 mo_cursor = MENU_OPTIONS_CLEFT+glm::vec2(mo_hwidth[0],0);
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
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
	// 2nd key: tilted splice underlining descriptions for list options if selected
	splice_selection_id = splices_geometry.create_splice(
			SPLICE_LOWER_CENTER,glm::vec2(0,720.f),
			0,0,
			SPLICE_SELECTION_COLOUR,false,&tkey_selection);
	splices_geometry.add_anim_key(splice_selection_id,
			glm::vec2(0,-SPLICE_HEAD_DLGDESC_QUAD),glm::vec2(SPLICE_HEAD_DLGDESC_QUAD,720),
			SPLICE_HEAD_DLGDESC_WIDTH,SPLICE_HEAD_DLGDESC_WIDTH);

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

	// list setup
	ml_options = mlists.define_list("./lvload/options.ldc");
	ml_extras = mlists.define_list("./lvload/extras.ldc");
	ml_stages = mlists.define_list("./lvload/stages.ldc");
	ml_saves = mlists.define_list(savestates);

	// test goto solution
	/*DebugLogData dld;
	Toolbox::start_debug_logging(dld,"compiler runtime delta");
	for (uint16_t i=0;i<100;i++) {
		mlists.define_list("./lvload/extras.ldc");
		mlists.define_list("./lvload/stages.ldc");
	}
	Toolbox::add_timekey(dld,"computational goto timing");
	for (uint16_t i=0;i<100;i++) {
		mlists.define_list("./lvload/extras.ldc");
		mlists.define_list("./lvload/stages.ldc");
	}
	Toolbox::add_timekey(dld,"function pointer timing");
	Toolbox::flush_debug_logging(dld);*/

	// add options for available screens
	Font t_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_LIST_HEAD_SIZE,MENU_LIST_HEAD_SIZE);
	uint8_t max_displays = SDL_GetNumVideoDisplays(),flist = ml_options+2;
	mlists.mlists[flist].entities[2].dd_options.resize(max_displays);
	mlists.mlists[flist].entities[2].dd_colours.resize(max_displays);
	mlists.mlists[flist].entities[2].dd_length.resize(max_displays);
	for (uint8_t i=0;i<max_displays;i++) {
		Text t_ddo = Text(t_font);
		t_ddo.add(
				("Monitor "+std::to_string(i)).c_str(),
				glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START-3*MENU_LIST_SCROLL_Y)
			),t_ddo.load();
		mlists.mlists[flist].entities[2].dd_options[i] = t_ddo;
		mlists.mlists[flist].entities[2].dd_colours[i] = glm::vec4(1.f);
		mlists.mlists[flist].entities[2].dd_length[i] = 8+(i>9)+(i>99);
	}

	// avoiding segfault should (for any mystical reason) no monitor be available
	if (!max_displays) {
		mlists.mlists[flist].entities[2].dd_options.push_back(Text(t_font));
		mlists.mlists[flist].entities[2].dd_options.back().add(
				"ERROR!",
				glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START-3*MENU_LIST_SCROLL_Y)
			),mlists.mlists[flist].entities[2].dd_options.back().load();
		mlists.mlists[flist].entities[2].dd_colours.push_back(TEXT_ERROR_COLOUR);
		mlists.mlists[flist].entities[2].dd_length.push_back(6);
	}
	mlists.load(m_ccbf,rid_window_sprite);

	// dialogue setup
	dg_diffs = mdialogues.add_dialogue_window("./lvload/challenge.ldc",glm::vec2(400,420),320,140,30,25);
	dg_continue = mdialogues.add_dialogue_window("./lvload/continue.ldc",glm::vec2(640,360),320,250,30,25);
	dg_optsave = mdialogues.add_dialogue_window("./lvload/savechanges.ldc",glm::vec2(640,360),320,250,30,25);
	mdialogues.load();

	// buffers
	msaa = MSAA("./shader/fbv_standard.shader","./shader/fbf_standard.shader",
			m_ccbf->frame->w_res,m_ccbf->frame->h_res,8);
	fb_nslice = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/fbf_standard.shader");
	fb_menu = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_mainmenu.shader");
	fb_menu.s.upload_vec2(
			"ratio",
			glm::vec2(Init::iConfig[FRAME_RESOLUTION_WIDTH],Init::iConfig[FRAME_RESOLUTION_HEIGHT])
		);
	fb_slice = FrameBuffer(m_ccbf->frame->w_res,m_ccbf->frame->h_res,
			"./shader/fbv_standard.shader","./shader/main_menu/fbf_splash.shader");
	fb_slice.s.upload_int("gbuffer_colour",0);
	fb_slice.s.upload_int("gbuffer_normals",1);
	fb_slice.s.upload_int("menu_fb",2);
}
// FIXME: when using mouse & keyboard input simultaneously the transition can be stuck between states

/*
	render(FrameBuffer*,bool&,bool&) -> void (virtual) !O(1)
	purpose: render the main menu, calculate geometry and process interactions
	\param game_fb: deferred shaded game scene framebuffer
	\param running: is application still running?
	\param reboot: will be rebooting with new settings after game closed?
*/
void MainMenu::render(FrameBuffer* game_fb,bool &running,bool &reboot)
{
	// button input
	bool plmb = m_ccbf->frame->mouse.mb[0]&&!trg_lmb,prmb = m_ccbf->frame->mouse.mb[2]&&!trg_rmb;
	hit_a = (m_ccbf->iMap->get_input_triggered(InputID::PAUSE)&&!menu_action)
			|| m_ccbf->iMap->get_input_triggered(InputID::FOCUS)
			|| m_ccbf->iMap->get_input_triggered(InputID::CONFIRM)
			|| plmb;
	hit_b = (m_ccbf->iMap->get_input_triggered(InputID::PAUSE)&&menu_action)
			|| m_ccbf->iMap->get_input_triggered(InputID::BOMB)
			|| prmb;

	// directional input
	lrmv = ((m_ccbf->iMap->get_input_triggered(InputID::RIGHT)&&vselect<OptionLogicID::OPTION_CAP)
			- (m_ccbf->iMap->get_input_triggered(InputID::LEFT)&&vselect>0))
			* menu_action;
	udmv = (m_ccbf->iMap->get_input_triggered(InputID::DOWN)
			- m_ccbf->iMap->get_input_triggered(InputID::UP))
			* menu_action;

	// mouse input
	crd_mouse = glm::vec2(
			m_ccbf->frame->mouse.mxfr*MATH_CARTESIAN_XRANGE,
			m_ccbf->frame->mouse.myfr*MATH_CARTESIAN_YRANGE
		);
	trg_lmb = m_ccbf->frame->mouse.mb[0],trg_rmb = m_ccbf->frame->mouse.mb[2];

	// timing
	transition_delta = TRANSITION_SPEED*m_ccbf->frame->time_delta;
	bool anim_go = anim_timing>ANIMATION_UPDATE_TIMEOUT;
	anim_timing += m_ccbf->frame->time_delta;
	dt_tshiftdown += m_ccbf->frame->time_delta*speedup,
			dt_tnormalize += m_ccbf->frame->time_delta*!speedup;

	// focus transition
	Toolbox::transition_float_on_condition(ftransition,transition_delta,interface_logic_id);
	inv_ftransition = 1.f-ftransition,inv_mtransition = 1.f-mtransition;

	// title rattle animation
	uint8_t rattle_mobility = RATTLE_THRESHOLD+RATTLE_THRESHOLD_RAGEADDR*menu_action,
		rattle_countermove = rattle_mobility/2;
	glm::vec3 title_action = glm::vec3((rand()%rattle_mobility-rattle_countermove)*anim_go,
			(rand()%rattle_mobility-rattle_countermove)*anim_go,0);

	// title shiftdown animation
	dt_tshiftdown *= menu_action,dt_tnormalize *= menu_action,speedup = speedup||!menu_action;
	float tshift = 1.f+SHIFTDOWN_ZOOM_INCREASE*((speedup) ? sqrt(sin(dt_tshiftdown*SHIFTDOWN_OCTAPI))
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

	// component updates before interface behaviour & rendering
	bool rrnd = false;
	uint8_t sd_grid = mlists.update(
			udmv,m_ccbf->iMap->request(InputID::RIGHT)-m_ccbf->iMap->request(InputID::LEFT),
			crd_mouse,m_ccbf->frame->mouse.mw,
			hit_a,m_ccbf->frame->mouse.mb[0],hit_b,
			m_ccbf->frame->mpref_peripheral,rrnd);
	mdialogues.update(udmv,crd_mouse.y,m_ccbf->frame->mpref_peripheral,hit_a,hit_b);

	// START RENDER MENU BUFFER
	fb_menu.bind();
	Frame::clear();

	// interface logic
	interface_behaviour[interface_logic_id](*this);

	// render dare message
	tx_dare.prepare();
	tx_dare.set_scroll(glm::vec2(title_action));
	tx_dare.render(tcap_dare*inv_mtransition,TEXT_DARE_COLOUR);
	tx_version.prepare();
	tx_version.render(tcap_version*inv_mtransition,TEXT_VERSION_COLOUR);

	// render input instructions
	tx_instr.prepare();
	tx_instr.set_scroll(glm::vec2(0,-(!!ftransition+mlists.instruction_mod)*720));
	tx_instr.render(tcap_instr*mtransition,glm::vec4(1,.6f,0,1));

	// render & transform main options
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
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
	// FIXME: optimize when all the text transitions are done

	// render titles
	m_ccbf->r2d->al[index_ranim+1].model = glm::translate(m_ccbf->r2d->al[index_ranim+1].model,
			glm::vec3(0,150*ftransition,0));
	m_ccbf->r2d->prepare();
	m_ccbf->r2d->render_state(index_ranim,glm::vec2(3,0));
	m_ccbf->r2d->render_state(index_ranim+1,glm::vec2(0,0));

	// update dialogues & lists
	mlists.render();
	mdialogues.render();
	// FIXME: request does not time input validity, work on input timing safety for unlocked update mode

	// START MULTISAMPLED RENDER
	FrameBuffer::close();
	msaa.bind();
	Frame::clear();

	// curtain animation for main option selector splice
	float curtain_trans = mtransition-ftransition;
	splices_geometry.splices[splice_selection_id].ssk[0].ext_lower = lext_selection*curtain_trans,
	splices_geometry.splices[splice_selection_id].ssk[0].ext_upper = uext_selection*curtain_trans;

	// update selection splash geometry
	if (rrnd) {
		SelectionSpliceKey* t_ssk = &splices_geometry.splices[splice_head_id].ssk[head_mod_id];
		float htrans = SPLICE_HEAD_ORIGIN_POSITION-MENU_LIST_SCROLL_Y*sd_grid;
		t_ssk->disp_lower.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD),
		t_ssk->disp_upper.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD),
		t_ssk->ext_lower = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA),
		t_ssk->ext_upper = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA);
	}

	// splash render
	tkey_head = mtransition+ftransition+mdialogues.system_active;
	Toolbox::transition_float_on_condition(tkey_selection,transition_delta,mlists.system_active);
	tkey_title = mtransition;
	splices_geometry.update();
	// FIXME: splash dimensions to prevent aesthetically unfortunate proportions

	// draw dialogue & list background components
	mlists.update_background_component();
	mdialogues.update_background_component();

	// STOP MULTISAMPLED RENDER
	msaa.blit();

	// render menu
	fb_menu.prepare();
	fb_menu.s.upload_float("vignette",.44f+(float)(rand()%21)*.001f);
	fb_menu.s.upload_float("mtransition",mtransition);
	fb_menu.render();

	// render anti-aliased splices
	fb_slice.bind();
	Frame::clear();
	msaa.prepare();
	MSAA::render();
	FrameBuffer::close();

	// render layers
	fb_slice.prepare();
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,fb_nslice.tex);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,fb_menu.tex);
	glActiveTexture(GL_TEXTURE0);
	fb_slice.s.upload_float("mtrans",mtransition);
	fb_slice.render();
	mlists.update_overlays();

	// finishing
	bool shiftdown_over = dt_tshiftdown>TITLE_SHIFTDOWN_TIMEOUT,
			normalize_over = dt_tnormalize>TITLE_NORMALIZATION_TIMEOUT;
	anim_timing -= ANIMATION_UPDATE_TIMEOUT*anim_go;
	dt_tshiftdown -= TITLE_SHIFTDOWN_TIMEOUT*shiftdown_over,
			dt_tnormalize -= TITLE_NORMALIZATION_TIMEOUT*normalize_over;
	speedup = (speedup&&!shiftdown_over)||normalize_over;

	// system
	reboot = request_restart;
	running = !request_close&&!request_restart;
}

/*
	TODO
*/
void MainMenu::update_peripheral_annotations()
{
	// update shown preferred peripheral
	cpref_peripheral = m_ccbf->frame->cpref_peripheral;

	// clear instruction text
	tx_dare.clear();
	tx_instr.clear();

	// assemble input instructions
	std::string dmessage;
	std::string instr[TEXT_INSTRUCTION_COUNT];
	if (cpref_peripheral) {

		// write messages for controller input
		dmessage = "press ["+m_ccbf->iMap->cnt_name[InputID::PAUSE]+"] if you DARE";
		instr[0] = "confirm ["+m_ccbf->iMap->cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->cnt_name[InputID::LEFT]
				+ '/'+m_ccbf->iMap->cnt_name[InputID::RIGHT]+"]"
				+ "  go back ["+m_ccbf->iMap->cnt_name[InputID::BOMB]+"]";
		instr[1] = "confirm ["+m_ccbf->iMap->cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->cnt_name[InputID::UP]
				+ '/'+m_ccbf->iMap->cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->cnt_name[InputID::BOMB]+"]";
		instr[2] = "see options ["+m_ccbf->iMap->cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->cnt_name[InputID::UP]
				+ '/'+m_ccbf->iMap->cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->cnt_name[InputID::BOMB]+"]";
		instr[3] = "adjust slider ["+m_ccbf->iMap->cnt_name[InputID::LEFT]
				+ '/'+m_ccbf->iMap->cnt_name[InputID::RIGHT]+"]"
				+ "  select ["+m_ccbf->iMap->cnt_name[InputID::UP]
				+ '/'+m_ccbf->iMap->cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->cnt_name[InputID::BOMB]+"]";
	} else {

		// write message for keyboard input
		dmessage = "press ["+m_ccbf->iMap->key_name[InputID::CONFIRM]+"] if you DARE";
		instr[0] = "confirm ["+m_ccbf->iMap->key_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->key_name[InputID::LEFT]
				+ '/'+m_ccbf->iMap->key_name[InputID::RIGHT]+"]"
				+ "  go back ["+m_ccbf->iMap->key_name[InputID::BOMB]+"]";
		instr[1] = "confirm ["+m_ccbf->iMap->key_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->key_name[InputID::UP]
				+ '/'+m_ccbf->iMap->key_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->key_name[InputID::BOMB]+"]";
		instr[2] = "see options ["+m_ccbf->iMap->key_name[InputID::FOCUS]+"]"
				+ "  select ["+m_ccbf->iMap->key_name[InputID::UP]
				+ '/'+m_ccbf->iMap->key_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->key_name[InputID::BOMB]+"]";
		instr[3] = "adjust slider ["+m_ccbf->iMap->key_name[InputID::LEFT]
				+ '/'+m_ccbf->iMap->key_name[InputID::RIGHT]+"]"
				+ "  select ["+m_ccbf->iMap->key_name[InputID::UP]
				+ '/'+m_ccbf->iMap->key_name[InputID::DOWN]+"]"
				+ "  go back ["+m_ccbf->iMap->key_name[InputID::BOMB]+"]";
	}

	// write dare message
	tx_dare.add(dmessage.c_str(),TEXT_DARE_POSITION);
	tcap_dare = dmessage.length();
	tx_dare.load();

	// write input instructions
	for (uint8_t i=0;i<TEXT_INSTRUCTION_COUNT;i++) {
		uint16_t estm = fnt_reqt.estimate_textwidth(instr[i].c_str())>>1;
		tx_instr.add(instr[i].c_str(),glm::vec2(MATH_CENTER_GOLDEN-estm,30+i*720));
		tcap_instr += instr[i].length();
	}
	tx_instr.load();
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
	tm.request_close = (tm.vselect==OptionLogicID::MACRO_EXIT)*action_request;
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
				&& out_id<OptionLogicID::OPTION_CAP) out_id++;
		ch_select = ch_select||(out_id!=tm.vselect);
		tm.vselect = out_id;
	}

	// selection splash update calculations
	if (ch_select||req_transition) {

		// change main option selector dimensions based on selected option
		SelectionSpliceKey* t_ssk = &tm.splices_geometry.splices[tm.splice_selection_id].ssk[0];
		glm::vec2 vrt_cpos = tm.mo_cposition[tm.vselect]
				+ glm::vec2(tm.mo_hwidth[tm.vselect],-MENU_OPTIONS_HSIZE);
		t_ssk->disp_lower.x = (vrt_cpos.x-SPLICE_LOWER_CENTER.x)*SPLICE_OFFCENTER_MV+SPLICE_LOWER_CENTER.x,
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
	tm.logic_setup = 0;
}

/*
	TODO
*/
void interface_behaviour_options(MainMenu &tm)
{
	bool open_conf = false;
	switch (tm.logic_setup) {

	// setup stage
	case 0:
		tm.mlists.open_list(tm.ml_options);
		tm.logic_setup++;
		break;

	// listing stage
	case 1:
		tm.logic_setup += !tm.mlists.system_active;
		break;

	// checking for changes & open confirmation dialogue on condition
	case 2:
		for (uint8_t i=tm.ml_options+1;i<tm.ml_options+5;i++)
			open_conf = open_conf||tm.mlists.linked_variables_changed(i,tm.queued_restart);
		if (open_conf) tm.mdialogues.open_dialogue(tm.dg_optsave);
		tm.logic_setup++;
		break;

	// confirmation input handling
	default:
		if (tm.mdialogues.dg_state==0&&tm.hit_a) {
			for (uint8_t i=tm.ml_options+1;i<tm.ml_options+5;i++)
				tm.mlists.write_attributes(i);
			Init::write_changes();
			tm.request_restart = tm.queued_restart;
		} else if (tm.mdialogues.dg_state==1&&tm.hit_a) {
			for (uint8_t i=tm.ml_options+1;i<tm.ml_options+5;i++)
				tm.mlists.reset_attributes(i);
			tm.queued_restart = false;
		} else if (tm.hit_a||tm.hit_b) tm.logic_setup = 0;
		tm.interface_logic_id *= tm.mdialogues.system_active;
	}
}

/*
	TODO
*/
void interface_behaviour_extras(MainMenu &tm)
{
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_extras);
		tm.logic_setup = true;
	} tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	TODO
*/
void interface_behaviour_practice(MainMenu &tm)
{
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_stages);
		tm.logic_setup = true;
	} tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	TODO
*/
void interface_behaviour_load(MainMenu &tm)
{
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_saves);
		tm.logic_setup = true;
	} tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	// TODO
*/
void interface_behaviour_continue(MainMenu &tm)
{
	// open dialogue for continuation request
	if (!tm.logic_setup) {
		tm.mdialogues.open_dialogue(tm.dg_continue);
		tm.logic_setup = true;
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
	if (!tm.logic_setup) {
		tm.mdialogues.open_dialogue(tm.dg_diffs);
		tm.logic_setup = true;
	}

	// TODO: preface runcreation with disclaimer that current run will be replaced by the new as main
	// TODO: actually implement functionality of the menu option

	// closing condition
	tm.interface_logic_id *= tm.mdialogues.dg_data[tm.dg_diffs].dg_active;
}

// new issues:
//	- input request annotation is not displayed for practice and extras listing, but works for all others?!?
//	- refusal of difficulty should automatically close confirmation prompt
//	- automatic restart is prevented when changing both restartable and non-restartable options?
//		-> probably has something todo with the refusal to write but still storing visual changes?
//	- when opening a list for the first time a memory leak occurs
