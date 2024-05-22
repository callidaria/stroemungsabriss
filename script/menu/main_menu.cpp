#include "main_menu.h"


/**
 *			Start Implementation of Compiler Logic Pointers switched by Command
 *
 *	The following functions will only be called by LDCCompiler::compile and interpret different commands, which
 *	have been identified beforehand and translated to a memory id pointing to one of those functions.
 *	As a response, these commands change the compiler data and state according to their definition.
 *
 *		How to define a new command
 *
 *	1. insert a command id descriptor in LDCCommandID enumerator in header file.
 *	2. go to interpreter_behaviour definition in source file and insert a function conforming to the typedef.
 *	3. implement that function depending on how the new command should work.
 *	4. finally go to mlcmd definition and insert command syntax.
 *	5. check if id descriptor, interpreter_behaviour function and command syntax have the same id.
 *
 *		Performance Statement
 *
 *	The performance of this solution has been compared against a regular switch and computed gotos.
 *	This approach was decisively faster that both other potential solutions.
*/


typedef void (*interpreter_logic)(LDCProcessState&);

/*
	!O(1)m /+load -> interpreter_logic (local,static)
	purpose: open new cluster definition until next definition is opened. (cmd = :cluster)
*/
void command_logic_cluster(LDCProcessState& state)
{
	LDCCluster cluster;
	cluster.id = state.cmd->tail;
	state.clusters.push_back(cluster);
	state.refs_children.push_back({{},{}});
	state.refs_links.push_back({});
}

/*
	!O(1)m /+load -> interpreter_logic (local,static)
	purpose: add named list entity to currently opened cluster (cmd = :define)
*/
void command_logic_define(LDCProcessState& state)
{
	LDCEntity entity;
	entity.head = state.cmd->tail;
	entity.jsegment = state.jnext, state.jnext = false;
	state.clusters.back().elist.push_back(entity);
}

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: add entity description (cmd = :describe)
*/
void command_logic_describe(LDCProcessState& state)
{ state.clusters.back().elist.back().description = state.cmd->buffer; }

/*
	!O(n)mn .amount of float attributes /+load -> interpreter_logic (local,static)
	purpose: store constant float attributes in entity (cmd = :floats)
*/
void command_logic_fattributes(LDCProcessState& state)
{
	try {
		std::stringstream bfss(state.cmd->tail);
		std::string attrib;
		while (getline(bfss,attrib,' '))
			state.clusters.back().elist.back().fattribs.push_back(stof(attrib));
	} catch (std::invalid_argument const& ex) {
		LDCCompiler::compiler_error_msg(state,"attribute tail should only contain float values");
	} catch (std::out_of_range const& ex) {
		LDCCompiler::compiler_error_msg(state,"at least one attribute is out of float range");
	}
}

/*
	!O(n)mn .amount of string attributes /+load -> interpreter_logic (local,static)
	purpose: store constant string attributes in entity (cmd = :strings)
*/
void command_logic_sattributes(LDCProcessState& state)
{
	std::stringstream bfss(state.cmd->tail);
	std::string attrib;
	while (getline(bfss,attrib,' '))
		state.clusters.back().elist.back().cattribs.push_back(attrib);
}

/*
	!O(1)m /+load -> interpreter_logic (local,static)
	purpose: insert a named segment that seperates the list of entities (cmd = :segment)
*/
void command_logic_segment(LDCProcessState& state)
{
	LDCSegment segment;
	segment.position = state.clusters.back().elist.size();
	segment.title = state.cmd->tail;
	state.clusters.back().slist.push_back(segment);
	state.jnext = true;
}

/*
	!O(n)mn .assigned conditions /+load -> interpreter_logic (local,static)
	purpose: define entity activation boolean reference in logic list by memory id (cmd = :condition)
*/
void command_logic_condition(LDCProcessState& state)
{
	try {
		std::stringstream bfss(state.cmd->tail);
		std::string lid;
		while (getline(bfss,lid,' '))
			state.clusters.back().elist.back().condition_id.push_back(stoi(lid));
	} catch (std::invalid_argument const& ex) {
		LDCCompiler::compiler_error_msg(state,"condition tail does not contain a valid number");
	} catch (std::out_of_range const& ex) {
		LDCCompiler::compiler_error_msg(state,"given number exceeds reasonable range");
	}
}

/*
	!O(1)m /+load -> interpreter_logic (local,static)
	purpose: link a global initialization variable to a list entity (cmd = :link)
*/
void command_logic_link(LDCProcessState& state)
{
	state.clusters.back().linked_ids.push_back(state.clusters.back().elist.size()-1);
	state.refs_links.back().push_back(state.cmd->tail);
}

/*
	!O(1)m /+load -> interpreter_logic (local,static)
	purpose: link subsequent cluster to transition to as entity action (cmd = :subsequent)
*/
void command_logic_subsequent(LDCProcessState& state)
{
	state.refs_children.back().parent_ids.push_back(state.clusters.back().elist.size()-1);
	state.refs_children.back().child_names.push_back(state.cmd->tail);
	state.clusters.back().elist.back().etype = SUBSEQUENT;
}

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: run system command id according to predefinition in ldc documentation (cmd = :system_behaviour)
*/
void command_logic_sysbehaviour(LDCProcessState& state)
{
	try {
		state.clusters.back().elist.back().etype = LDCEntityType::SYSTEM;
		state.clusters.back().elist.back().tdata = stoi(state.cmd->tail);
	} catch (std::invalid_argument const& ex) {
		LDCCompiler::compiler_error_msg(state,"system command id not an interpretable number");
	} catch (std::out_of_range const& ex) {
		LDCCompiler::compiler_error_msg(state,"system command id out of reasonable defrange");
	}
}

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: set boolean on/off functionality as entity action (cmd = :checkbox)
*/
void command_logic_checkbox(LDCProcessState& state)
{
	state.clusters.back().elist.back().etype = LDCEntityType::CHECKBOX;
	state.clusters.back().cnt_checkbox++;
}

/*
	!O(n)bnmn .dropdown options +1 /+load -> interpreter_logic (local,static)
	purpose: create sublist index linked choices attached to entity (cmd = :dropdown)
*/
void command_logic_dropdown(LDCProcessState& state)
{
	std::stringstream bfss(state.cmd->buffer);
	std::string ddoption;
	while (getline(bfss,ddoption,' ')) {
		if (ddoption[0]==';') break;
		state.clusters.back().elist.back().cattribs.push_back(ddoption);
	}
	state.clusters.back().elist.back().etype = LDCEntityType::DROPDOWN;
	state.clusters.back().cnt_dropdown++;
}
// TODO: benchmark push back usage against doubled tail process + predefined list size for tail copy
// 	this is also relevant for attributes/floats insertions

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: attach transformable float value to entity (cmd = :slider)
*/
void command_logic_slider(LDCProcessState& state)
{
	state.clusters.back().elist.back().etype = LDCEntityType::SLIDER;
	state.clusters.back().cnt_slider++;
}

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: define finalizing entity with set value return (cmd = :return)
*/
void command_logic_return(LDCProcessState& state)
{
	state.clusters.back().elist.back().tdata = stoi(state.cmd->tail);
	state.clusters.back().elist.back().etype = LDCEntityType::RETURN;
}

/*
	!O(1) /+load -> interpreter_logic (local,static)
	purpose: this method catches invalid syntax/commands and notifies the developer
*/
void command_logic_syntax_error(LDCProcessState& state)
{ LDCCompiler::compiler_error_msg(state,"invalid command syntax"); }


// instruction handling references
const interpreter_logic interpreter_behaviour[LDCCommandID::COMMAND_COUNT+1] = {

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
const std::string mlcmd[LDCCommandID::COMMAND_COUNT] = {

	// definitions
	"cluster","define","describe",

	// attributes
	"floats","strings","segment","condition","link",

	// behaviours
	"subsequent","system_behaviour","checkbox","dropdown","slider","return"
};


/**
 *			LDC Compiler Implementation
 *
 *	The LDC Compiler reads a menu list definition .ldc file and compiles it as a struct complex.
 *	This resulting complex can be used to write an intern list logic, which will be externally defined and
 *	compiled at load time.
 *
 *		List Definition Code (LDC) Language Design
 *
 *	prefix ':' marks the following string until ' ' or '\n' as command expression within LDC definition below.
 *	following expressions are valid and will be handled by menu list interpreter:
 *	(the definitions are using <> to show where custom naming replaces "<contents>")
 *
 *
 *		GENERAL LANGUAGE FEATURES (WARNING: usage of regex ahead)
 *
 *	 :cluster <cluster_name>
 *	This statement marks the definition of a list, sublist or simply a list entity collection.
 *	Any cluster can be referred to by name within the list definition file.
 *
 *	 :define <entity_name>
 *	Define a new list entity by name.
 *	Definition space of this entity lasts until next :cluster or :define call.
 *
 *	 :describe
 *	 <description>
 *	Add a description to the list entity.
 *	TODO: add possible escape behaviour to mark end of description in buffer ?(char ';')?
 *
 *	 :floats (<float> )*
 *	Attach float constants to the list entity, accessible for any reason, usage defined by logic.
 *
 *	 :strings (<string> )*
 *	Attach string constants to the list entity, accessible for any reason, usage defined by logic.
 *
 *	 :segment <segment_name>
 *	Whereever a segment is called within the list definition file, a list entry will be added, linked to
 *	the position it was created at, but unrelated to the actual entity list.
 *	This feature can be used to segment a list of entities into multiple parts, while not interrupting the list
 *	itself.
 *
 *	 :condition (<condition_id> )*
 *	Specify an amount of conditions to activate list entity.
 *	The condition ids will read the respective booleans from an extern condition array.
 *
 *	 :link <init_variable>
 *	Link attribute value to initialization variable.
 *
 *
 *		BEHAVIOUR DEFINITIONS
 *	(ideally choose ONE of the following commands per entity definition)
 *
 *	 :system_behaviour <behaviour_id>
 *	behaviour_ids:
 *	 0 :<=> no behaviour
 *	 1 :<=> terminate current list
 *	purpose:
 *	This will attach system functionality id to the list entity.
 *	Behaviour ids can be defined differently in custom logic, but it is highly discouraged due to possible
 *	confusion, when standard behaviour differs.
 *	Individual checks into custom logic are generally avoided by programmers due to naïveté.
 *
 *	 :subsequent <cluster_name>
 *	This entity will be linked to a sublist by it's clusters name.
 *
 *	 :checkbox
 *	Sign this entity to be switchable between true or false states.
 *
 *	 :dropdown
 *	 (<sublist_option> )* ;
 *	Add a sublist dropdown to this entity.
 *	Using :dropdown without defining options will still create a sublist and the list can be filled later by
 *	directly writing elements into LDCEntity::cattribs component.
 *	The semicolon (" ;") after a sublist option forcefully ends the option declaration.
 *
 *	 :slider
 *	Attach an adjustable floating point number to this entity.
 *
 *	 :return <output_value>
 *	This entity returns the given value on confirmation.
 *
 *
 *	The following implementation is the static compile function for LDC, holding the language interpreter,
 *	conforming to the above language definitions and statements.
 *	The compiler only has one feature: compile, because this is what it does.
*/


/*
	!O(n)bm .defined entities from all clusters in file combined /function -> (static)
	purpose: load .ldc files, break it up into a command buffer and compile it into a complex
	\param path: path to .ldc file
	\param rClusters: reference to cluster list, the new clusters will be added into
	NOTE: don't kill me for calling it "compile", i just wanted to be cute & it kinda does compile ok
*/
void LDCCompiler::compile(const char* path,std::vector<LDCCluster>& rClusters)
{
	// setup process state
	LDCProcessState state = {
		.fpath = path,
		.clusters = rClusters
	};

	// first pass: extraction - iterate definition file
	// all commands will be written to command list after extraction for further processing
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
			while (llc.id<LDCCommandID::COMMAND_COUNT&&line.substr(1,cmd_split-1)!=mlcmd[llc.id]) llc.id++;

			// handle command attributes
			llc.tail = line.erase(0,cmd_split+1);
			llc.line_number = line_number;

			// assemble command buffer
			cmd_buffer.push_back(llc);
		}
		else cmd_buffer.back().buffer += line+' ';

		// incement line counter & finally close file
		line_number++;
	}
	file.close();

	// second pass: preallocate memory
	// tbd - this will be an optimization measure in the future to prevent push_back usage
	/*uint16_t alloc_cluster = 0;
	for (ListLanguageCommand &cmd : cmd_buffer) {
		alloc_cluster += cmd.id==LDCCommandID::CLUSTER;
	}
	state.clusters.resize(alloc_cluster);
	state.crefs.resize(alloc_cluster);
	state.srefs.resize(alloc_cluster);*/
	// TODO: something like this to reduce push_back usage in third compiler pass

	// third pass: execute commands
	// command list will be iterated and command id will instigate jump to related processes
	for (ListLanguageCommand& cmd : cmd_buffer) {
		state.cmd = &cmd;
		interpreter_behaviour[cmd.id](state);
	}

	// fourth pass: interpretation - temporary instruction data
	for (uint16_t i=0;i<state.clusters.size();i++) {
		LDCCluster &cc = state.clusters[i];

		// correlate child name to cluster id
		// iteration over all stored parent ids, cutting the check for all entities without children
		// then finding iteration over all cluster ids until child of the current parent has been found
		for (uint16_t j=0;j<state.refs_children[i].parent_ids.size();j++) {
			uint16_t k = 0;
			while (state.clusters[k].id!=state.refs_children[i].child_names[j]) k++;
			state.clusters[i].elist[state.refs_children[i].parent_ids[j]].tdata = k;
		}
		// FIXME: isn't this unsafe? would there be a memory leak if no names match the referenced name?

		// correlate initialization variable name to serialization id
		for (uint16_t j=0;j<cc.linked_ids.size();j++) {
			LDCEntity& ce = state.clusters[i].elist[state.clusters[i].linked_ids[j]];

			// find variable and check for errors
			ce.serialization_id = Init::find_iKey(state.refs_links[i][j].c_str());
			if (ce.serialization_id==InitVariable::VARIABLE_ERROR) {
				printf("%s: \033[31;1mvariable linking error.\033[0m referenced as \"\033[34;1m%s\033[0m\"\n",
						state.fpath,state.refs_links[i][j].c_str());
				continue;
			}

			// override held entity data with global variable if comparable
			ce.tdata = Init::iConfig[ce.serialization_id];
		}
	}
}


/**
 *		Selection Splice Geometry Implementation
 *
 *  The selection splice geometry is a stilistical highlight for the UI.
 *  It not only adds graphical flavour, it can also be used to communicate selection to the user.
 *
 *		Here is some information on how this geometry can be defined:
 *  ...following, an ascii representation of the splice.
 *
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
 *  	Explanation:
 *  lc: centerpoint of lower geometry
 *  uc: centerpoint of upper geometry
 *  uwe: full upper width extension at upper centerpoint
 *  lwe: full lower width extension at lower centerpoint
 *  	(!CAREFUL: the width extension shown here is double the definition length. centerpoint to edge)
 *  direction: automates the width extensions, so the orientation of the splice has to be predefined
 *
 *  	Features of this implementation:
 *  - splices can be created
 *  - animation keys can be created, to transition in-between positional states by an external variable
 *  - geometry can be drawn
*/

/*
	!O(1)m /+load -> (public)
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
	size_t vs = verts.size();
	verts.resize(vs+6);
	verts[vs] = { c,0 }, verts[vs+1] = { c,3 }, verts[vs+2] = { c,2 },
	verts[vs+3] = { c,3 }, verts[vs+4] = { c,0 }, verts[vs+5] = { c,1 };

	// write initial splice data
	SelectionSplice splice = {
		.current = {
			.disp_lower = l,
			.disp_upper = u,
			.ext_lower = lw,
			.ext_upper = uw
		},
		.ssk = { splice.current },
		.transition_ref = tref,
		.horizontal = hrz
	};

	// store & return id of created selection splice
	splices.push_back(splice);
	return splices.size()-1;
}
// TODO: system relying on predetermined state "horizontal" is very error prone. find a more adaptive solution
// TODO: aesthetics - pseudoshadow shader for text intersection

/*
	!O(1) /load -> (public)
	purpose: upload splice geometry, compile shader & setup coordinate system
*/
void SelectionSpliceGeometry::load()
{
	// upload vertex data to buffer
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	size_t vsize = sizeof(SpliceVertexGeometry);
	shader.compile("./shader/menu/splash.vs","./shader/menu/splash.fs");
	shader.def_irregular_attributeF("colour",3,vsize,offsetof(SpliceVertexGeometry,colour));
	shader.def_irregular_attributeI("edge_id",1,vsize,offsetof(SpliceVertexGeometry,edge_id));
	shader.upload_camera();
}

/*
	!O(1)m /+load -> (public)
	purpose: create transition key information for referenced splice geometry
		this key will always be added at the end of the key list, creation chronology matters
	\param id: memory id given at creation call, referencing the splice to add key to
	\param ld: transition target for lower splice geometry centerpoint
	\param ud: transition target for upper splice geometry centerpoint
	\param le: transition target for lower width extension
	\param ue: transition target for upper width extension
	\returns memory id the created animation key can be referenced by
*/
uint8_t SelectionSpliceGeometry::add_anim_key(uint8_t id,glm::vec2 ld,glm::vec2 ud,float le,float ue)
{
	// key creation
	SelectionSpliceKey t_ssk = {
		.disp_lower = ld,
		.disp_upper = ud,
		.ext_lower = le,
		.ext_upper = ue
	};

	// store key & return id
	splices[id].ssk.push_back(t_ssk);
	return splices[id].ssk.size()-1;
}

/*
	!O(n) .every splice geometry /update -> (public)
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
// FIXME: single draw calls for each splice is slow, take care of this in the performance update


/**
 *			Start Implementation of Vertical List for Main Menu
 * 
 *	The Menu List transforms .ldc files into displayable lists with a variety of features:
 *	- It is possible to navigate through lists by choosing their linked entity.
 *	- Entity attributes can be linked to variables and modified through multiple stylish representations
 *		-> sliders, dropdowns and checkboxes.
 *	- Loaded savestates are easily represented through Menu List, due to function overload.
 *	- entities can hold their own preview information
 *		-> location on globe map & difficulty prognosis
 *
 *		Metaprogramming of List Behaviour
 *
 *	Through the placement of commands into constant string attributes through :strings in LDC language,
 *	special menu features can be activated to display further information.
 *	Following the definition of additional features the MenuList provides, unrelated to common LDC functionality.
 *
 *		Special definition rules
 *
 *	- define a globe preview by rotation:
 *	 add "grotation" to strings component and add target coordinates (6 floats) to floats component
 *	- define an abnormal text colouration:
 *	 add "colour" to strings component and add target colour vector (4 floats) to floats component
 *	- define a challenge preview banner:
 *	 add "diff_id" to strings component and add difficulty id (1 unsigned 8-bit integer) to floats component
 *	 (difficulty ids: 1-6 student; 7-11 master; 12-14 grandmaster; 15-16 headmaster)
 *
 *	Should commands be required alongside dropdown elements:
 *	 first call :dropdown command and then prefix any further commands with "cmd" in strings component
 *	 NOTE: :dropdown has to be called before :strings cmd
 *
 *		MenuList Features
 *
 *	- First run define_list(...) to load list definition. (can be done through .ldc file or loaded savestates)
 *	- After completing all definitions, load() shall be executed.
 *	- Lists are closed by default. Should the necessity arise call open_list(...) to activate.
 *	- When the List is done being useful, just close it again by calling close_list(...).
 *	- If an entity is supposed to stand out for some reason, it can be recoloured by calling discolour(...).
 *	- Finalize variable modification with write_attributes(...)/reset_attributes(...), depending on circumstance.
 *	- The update works by calling update in the base render stage and the other component in the msaa stage.
 *	- Finally, the last update_overlays(...) belongs right after joining all other framebuffers
 *	- Communication about any list being open at any given time can be requested through .system_active.
 *	- Also the result of linked_variables_changed(...) informs the user about potentially changed attributes.
 *
 *		TODO: features & optimization:
 *
 *	- optimize drawing, geometry and everything else that is very wrong here
 *	- wiggly text shadow
 *	- fading between lists (tilt shift effect, smooth transition between background and foreground)
 *	- add an option to define what will be shown if the condition wasn't met (greyed,???,gfx replacements)
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
	for (LDCCluster& cluster : clusters) {
		mlists[lidx] = {
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
				)), t_segment.text.load();
			mlists[lidx].segments[i] = t_segment;
		}

		// process cluster entities
		uint16_t grid_position = 0;
		uint16_t head_checkbox = 0,head_dropdown = 0,head_slider = 0;
		for (uint16_t i=0;i<cluster.elist.size();i++) {

			// dodge scroll to prevent segment override with list element
			grid_position += cluster.elist[i].jsegment;
			float vscroll = MENU_LIST_SCROLL_START-MENU_LIST_SCROLL_Y*grid_position;

			// create entity
			MenuListEntity t_entity = {
				.grid_position = grid_position,
				.etype = cluster.elist[i].etype,
				.value = cluster.elist[i].tdata,
				.link_id = cluster.elist[i].serialization_id,
				.text = Text(st_font),
				.tlen = cluster.elist[i].head.length()
			};

			// fill in element list information
			t_entity.text.add(cluster.elist[i].head.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),
				t_entity.text.load();
			mlists[lidx].description.add(
					cluster.elist[i].description,
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

				// write dropdown element text
				for (uint8_t di=0;di<cai;di++) {
					Text dd_text = Text(st_font);
					dd_text.add(cluster.elist[i].cattribs[di].c_str(),
							glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,vscroll)),
						dd_text.load();
					t_entity.dd_options[di] = dd_text;
					t_entity.dd_colours[di] = glm::vec4(1.f);
					t_entity.dd_length[di] = cluster.elist[i].cattribs[di].length();
				}
				mlists[lidx].dropdown_ids[head_dropdown++] = i;
				break;

			// load slider geometry
			case LDCEntityType::SLIDER:
				create_slider(vscroll);
				mlists[lidx].slider_ids[head_slider++] = i;
				break;
			};

			// move cursor to write next element & store current information
			grid_position++;
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
		.entities = std::vector<MenuListEntity>(states.saves.size()),
		.description = Text(st_font)
	};

	// iterate save data and create a state list with a proportionally linear relationship
	int32_t vscroll = MENU_LIST_SCROLL_START;
	for (uint16_t i=0;i<states.saves.size();i++) {
		SaveData& state = states.saves[i];
		MenuListEntity mle = {
			.grid_position = i,
			.colour = diff_colours[state.diff],
			.etype = LDCEntityType::RETURN,
			.value = (uint16_t)(i+1),
			.has_rotation = true,
			.grotation = glm::vec2(0),
			.text = Text(st_font),
			.tlen = state.title.length()
		};

		// write save title
		mle.text.add(state.title.c_str(),glm::vec2(MENU_LIST_HEADPOS_X,vscroll)),mle.text.load();
		mlc.description.add(
				state.description,
				MENU_LIST_DESC_POSITION-glm::vec2(0,MATH_CARTESIAN_YRANGE*i),
				MENU_LIST_DESC_BLOCKWIDTH,
				MENU_LIST_DESC_NLINEJMP
			);
		mlc.dtlen += state.description.length();
		// TODO: add correct global preview, when savestate is finally realized and allows for such storage

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
	}

	// output result
	mlc.description.load();
	mlists.push_back(mlc);
	return mlists.size()-1;
}

/*
	!O(1)m /load -> (public)
	purpose: setup vertices & compile shaders for entity attribute visualization
*/
void MenuList::load()
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
	};
	ddbgr_buffer.bind(), ddbgr_buffer.upload_vertices(ddbgr_vertices,sizeof(ddbgr_vertices));

	// dropdown shader setup
	ddbgr_shader.compile("./shader/menu/dropdown.vs","./shader/menu/dropdown.fs");
	ddbgr_shader.def_attributeF("position",2,0,3);
	ddbgr_shader.def_attributeF("dmod",1,2,3);
	ddbgr_shader.upload_int("jmp_dist",MENU_LIST_SCROLL_Y);
	ddbgr_shader.upload_camera();

	// setup checkbox corpi
	checkbox_buffer.bind(),checkbox_buffer.upload_vertices(checkbox_vertices);
	checkbox_shader.compile("./shader/menu/checkbox.vs","./shader/menu/checkbox.fs");
	checkbox_shader.def_attributeF("position",2,0,4);
	checkbox_shader.def_attributeF("drift_mod",2,2,4);
	checkbox_shader.upload_camera();

	// setup slider corpi
	slider_buffer.bind(), slider_buffer.upload_vertices(slider_vertices);
	slider_shader.compile("./shader/menu/slider.vs","./shader/menu/slider.fs");
	slider_shader.def_attributeF("position",2,0,3);
	slider_shader.def_attributeF("bmod",1,2,3);
	slider_shader.upload_float("max_disp",MENU_LIST_ATTRIBUTE_WIDTH);
	slider_shader.upload_camera();

	// difficulty preview spritesheet
	rid_diffs = Core::gRenderer.add_sprite(glm::vec2(950,550),250,50,"./res/menu/diffbanner_colours.png",16,1,30,0);
	Core::gRenderer.add_sprite(glm::vec2(-125,-25),250,50,"./res/menu/diffbanner_annotations.png",16,1,30,0);

	// globe render target
	rid_globe = Core::gR3D.add_physical("./res/terra.obj","./res/terra/albedo.jpg","./res/terra/norm.png",
			"./res/terra/materials.png","./res/none.png",glm::vec3(0),1,glm::vec3(0));
	gb_lights.add_sunlight({ glm::vec3(-50,25,25),glm::vec3(1),10.f });
	globe_target_id = Core::gR3D.add_target();
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
	system_active = true, tf_list_opened = true;
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
	NOTE: initalization write to file has to be performed seperately to avoid repetition, like so:
		for (auto id : list_id) write_attributes(id);
		Init::write_changes();
*/
void MenuList::write_attributes(uint8_t id)
{
	for (uint16_t lid : mlists[id].link_ids) {
		MenuListEntity& e = mlists[id].entities[lid];
		if (e.link_id==InitVariable::VARIABLE_ERROR) continue;
		Init::iConfig[e.link_id] = e.value;
	}
}

/*
	!O(n)bn .list entities /function -> (public)
	purpose: reset changed attributes in list to original state of initialization, when linked successfully
	\param id: id of list complex to reset attributes of
	NOTE: not possible with attributes, that are not linked. reset values rely on initalization memory
*/
void MenuList::reset_attributes(uint8_t id)
{
	for (uint16_t lid : mlists[id].link_ids) {
		MenuListEntity& e = mlists[id].entities[lid];
		if (e.link_id==InitVariable::VARIABLE_ERROR) continue;
		e.value = Init::iConfig[e.link_id];
	}
}

/*
	!O(n)bn .linked attributes /+function -> (public)
	purpose: check if linked variables of desired list have changed compared to initialization
	\param list_id: memory index of list in question
	\param reload: reference, to be overwritten as true, should a changed value require program restart
	\returns true if any linked attributes are not identical with initialization, else false
*/
bool MenuList::linked_variables_changed(uint16_t list_id,bool& reload)
{
	// iterate all linked variables
	bool out = false;
	for (uint16_t id : mlists[list_id].link_ids) {
		MenuListEntity& ce = mlists[list_id].entities[id];
		if (ce.link_id==InitVariable::VARIABLE_ERROR) continue;

		// check for changes against linked initialization variable
		bool changed = ce.value!=Init::iConfig[ce.link_id];
		out = out||changed;
		reload = reload||(iKeys[ce.link_id].restart_system_on_change&&changed);
	}
	return out;
}
// TODO: extract more information to display a list of changes later

/*
	~O(n)b .furthest possible selection difference /update -> (public)
	purpose: update menu list state & selection values
	\param input: processed input for all main menu interactions
	\param rrnd: reference to boolean value, telling the selection splices to rerandomize geometry
	\returns selection mapped to grid position after applying scroll, this determines the splice position
*/
int8_t MenuList::update(ProcessedMenuInput& input,bool& rrnd)
{
	// early exit when no lists active
	system_active = active_ids.size();
	instruction_mod = 0;
	show_globe = false;
	if (!active_ids.size()) return 0;
	MenuListComplex& crr = mlists[active_ids.back()];

	// update most recent list
	int8_t out = 0;
	if (!subfunc_opened) {

		// escape handling
		if (input.hit_b) {
			rrnd = true;
			close_list(active_ids.back());
			if (active_ids.size()) return calculate_grid_position();
			system_active = false;
			return 0;
		}

		// selection by rasterized mouse position
		int32_t selected = crr.select_id;
		uint16_t t_scroll = crr.lscroll;
		if (*input.mouse_preferred_peripheral) {

			// rasterize mouse selection
			int32_t max_scroll = crr.entities.back().grid_position-MENU_LIST_NEUTRAL_GRIDRANGE;
			crr.lscroll -= input.mouse->mw
					* ((crr.lscroll>0||input.mouse->mw<0)&&(crr.lscroll<max_scroll||input.mouse->mw>0));
			int32_t mraster = crr.lscroll+(MENU_LIST_SCROLL_START-input.mouse_cartesian.y)/MENU_LIST_SCROLL_Y;

			// correlate entity with rasterized selection
			while (selected<(crr.entities.size()-1)&&mraster>crr.entities[selected].grid_position) selected++;
			while (selected>0&&mraster<crr.entities[selected].grid_position) selected--;
		}

		// selection by directional input
		else {

			// apply vertical input direction to list selection
			selected += input.dir_vert;
			selected = (selected<0) ? 0 : (selected>=crr.entities.size()) ? crr.entities.size()-1 : selected;

			// adjust list scroll
			int32_t delta = crr.entities[selected].grid_position-crr.lscroll;
			crr.lscroll += std::max(delta-MENU_LIST_NEUTRAL_GRIDRANGE,0)+std::min(delta,0);
		}

		// override selection & check for changes
		rrnd = rrnd||tf_list_opened||(crr.select_id!=selected)||(t_scroll!=crr.lscroll);
		crr.select_id = selected;
		MenuListEntity& ce = crr.entities[selected];

		// slider manipulation by input
		if (ce.etype==LDCEntityType::SLIDER) {
			instruction_mod = 2;
			int8_t nt_hdir = Base::gIMap.request(InputID::RIGHT)-Base::gIMap.request(InputID::LEFT);

			// slider modification
			if (input.mouse->mb[0]
					&& *input.mouse_preferred_peripheral
					&& input.mouse_cartesian.x>MENU_LIST_ATTRIBUTE_COMBINE
					&& input.mouse_cartesian.x<MENU_LIST_ATTRIBUTE_WTARGET
				)
				ce.value = ((input.mouse_cartesian.x-MENU_LIST_ATTRIBUTE_COMBINE)/MENU_LIST_ATTRIBUTE_WIDTH)
						* 100;
			else if (nt_hdir) {
				int16_t nvalue = ce.value+nt_hdir;
				ce.value = (nvalue<0) ? 0 : (nvalue>100) ? 100 : nvalue;
			}
		}

		// confirmation handling
		else if (input.hit_a) {
			switch (ce.etype) {
			case LDCEntityType::DROPDOWN:
				subfunc_opened = true;
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

		else instruction_mod = ce.etype==LDCEntityType::DROPDOWN;

		// selection geometry data manipulation
		out = calculate_grid_position();
		tf_list_opened = false;
		return out;
	}

	// input handling for sublist
	MenuListEntity& e = crr.entities[crr.select_id];
	uint8_t cap_options = e.dd_options.size()-1;

	// handle mouse input
	if (*input.mouse_preferred_peripheral) {
		uint16_t box_start = MENU_LIST_SCROLL_START-MENU_LIST_SCROLL_Y*(e.grid_position-crr.lscroll-e.value);
		int8_t mshover = (box_start-input.mouse_cartesian.y)/MENU_LIST_SCROLL_Y;
		mshover = (mshover<0) ? 0 : (mshover>cap_options) ? cap_options : mshover;
		e.dd_colours[mshover] = TEXT_DIALOGUE_HOVER_COLOUR;
		e.value = input.hit_a ? mshover : e.value;
	}

	// handle directional input
	else {
		int8_t nvalue = e.value+input.dir_vert;
		e.value = (nvalue<0) ? 0 : (nvalue>cap_options) ? cap_options : nvalue;
	}

	// confirmation handling for current selection
	subfunc_opened = !input.hit_a&&!input.hit_b;
	return out;
}
// FIXME: code & processing repitition due to update/render split

/*
	!O(n)b .amount of different list entries /update -> (public)
	purpose: draw menu list entity components: text, dropdown & challenge preview
*/
void MenuList::render()
{
	// skip if no lists active
	if (!system_active) return;
	MenuListComplex& crr = mlists[active_ids.back()];

	// update attribute subfunctionality for dropdown options
	if (subfunc_opened) {
		MenuListEntity& e = crr.entities[crr.select_id];

		// draw dropdown background
		ddbgr_buffer.bind(),ddbgr_shader.enable();
		ddbgr_shader.upload_int("selection",calculate_grid_position()-e.value);
		ddbgr_shader.upload_int("ddsize",e.dd_options.size()-1);
		glDrawArrays(GL_TRIANGLES,0,12);

		// write dropdown elements of selection
		for (uint8_t ddi=0;ddi<e.dd_options.size();ddi++) {
			Text& ddtx = e.dd_options[ddi];
			ddtx.prepare(),ddtx.set_scroll(glm::vec2(0,-MENU_LIST_SCROLL_Y*(ddi-e.value)));
			ddtx.render(e.dd_length[ddi],e.dd_colours[ddi]);
			e.dd_colours[ddi] = glm::vec4(1.f);
		}
	}

	// write dropdown elements
	else {
		for (uint16_t& ddi : crr.dropdown_ids) {
			MenuListEntity& e = crr.entities[ddi];
			e.dd_options[e.value].prepare();
			e.dd_options[e.value].set_scroll(glm::vec2(0,0));
			e.dd_options[e.value].render(e.dd_length[e.value],glm::vec4(1.f));
		}
	}
	// TODO: in DIRE need of optimization once the text implementation is fixed!

	// draw active lists
	crr_scroll = crr.lscroll*MENU_LIST_SCROLL_Y;
	for (MenuListSegment& s : crr.segments)
		s.text.prepare(),s.text.set_scroll(glm::vec2(0,crr_scroll)),s.text.render(s.tlen,TEXT_SEGMENT_COLOUR);
	for (MenuListEntity& e : crr.entities)
		e.text.prepare(),e.text.set_scroll(glm::vec2(0,crr_scroll)),e.text.render(e.tlen,e.colour);

	// description out
	crr.description.prepare(),crr.description.set_scroll(glm::vec2(0,crr.select_id*720.f));
	crr.description.render(crr.dtlen,glm::vec4(1));
	// FIXME: calculating scroll matrix twice due to braindead individual scroll on text

	// globe render setup
	show_globe = crr.entities[crr.select_id].has_rotation;
	globe_rotation = crr.entities[crr.select_id].grotation;

	// difficulty prognosis text animation
	if (!crr.entities[crr.select_id].diff_preview) return;
	anim_prog -= 2*MATH_PI*(anim_prog>2*MATH_PI);
	float diff_scale = 1.f+glm::sin(anim_prog)*.4f;
	float diff_rotation = glm::sin(anim_prog*2.f)*45.f;
	Core::gRenderer.atlas[rid_diffs+1].transform.model = glm::mat4(1.f);
	Core::gRenderer.atlas[rid_diffs+1].transform.translate(glm::vec2(1075,575));
	Core::gRenderer.atlas[rid_diffs+1].transform.scale(diff_scale,diff_scale);
	Core::gRenderer.atlas[rid_diffs+1].transform.rotate(diff_rotation);
	anim_prog += .02f;
	// TODO: this translation pipeline does not make much sense does it?
	//		translation still needs some work for it to be feasable for actual utility

	// difficulty prognosis render
	uint8_t rstate = crr.entities[crr.select_id].diff_preview-1;
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite_frame(rid_diffs,glm::vec2(0,rstate));
	Core::gRenderer.render_sprite_frame(rid_diffs+1,glm::vec2(0,rstate));
}
// TODO: animate dropdown opening

/*
	!O(n) .amount of sliders and checkboxes /update -> (public)
	purpose: draw section of inverted/multisampled background components: sliders & checkboxes
*/
void MenuList::update_background_component()
{
	// prepare checkboxes
	if (!active_ids.size()||subfunc_opened) return;
	MenuListComplex& crr = mlists[active_ids.back()];
	checkbox_buffer.bind(),checkbox_shader.enable();
	checkbox_shader.upload_float("scroll",crr_scroll);

	// iterate checkboxes
	for (uint16_t i=0;i<crr.checkbox_ids.size();i++) {
		MenuListEntity& ce = crr.entities[crr.checkbox_ids[i]];
		Toolbox::transition_float_on_condition(ce.anim_transition,MainMenu::transition_delta,ce.value);
		checkbox_shader.upload_float("aprog",ce.anim_transition);
		glDrawArrays(GL_TRIANGLES,12*i,12);
	}

	// prepare sliders
	slider_buffer.bind(),slider_shader.enable();
	slider_shader.upload_float("scroll",crr_scroll);

	// iterate sliders
	for (uint16_t i=0;i<crr.slider_ids.size();i++) {
		MenuListEntity& ce = crr.entities[crr.slider_ids[i]];
		slider_shader.upload_float("sval",ce.value*.01f);
		glDrawArrays(GL_TRIANGLES,6*i,6);
	}
}
// FIXME: single drawcalls for every slider and checkbox seems like a horrible idea

/*
	!O(1)b /update -> (public)
	purpose: draw location preview on globe
*/
void MenuList::update_overlays()
{
	// early exit if globe render can be skipped
	if (!show_globe) return;

	// transform globe towards preview location
	Core::gR3D.pml[rid_globe].model = glm::rotate(
			glm::rotate(glm::mat4(1),glm::radians(globe_rotation.x),glm::vec3(1,0,0)),
			glm::radians(globe_rotation.y),glm::vec3(0,1,0)
		);

	// render globe to target
	glEnable(GL_DEPTH_TEST), glDisable(GL_BLEND);
	Core::gR3D.start_target(globe_target_id);
	Core::gR3D.prepare_pmesh(gb_cam3D), Core::gR3D.render_pmsh(rid_globe);
	glDisable(GL_DEPTH_TEST), glEnable(GL_BLEND);

	// globe deferred shading
	fb_globe.bind();
	Frame::clear();
	Core::gR3D.render_target(globe_target_id,gb_cam3D,&gb_lights);
	FrameBuffer::close();

	// draw globe buffer
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.sprite_shader.upload_float("vFlip",1.f);
	Core::gRenderer.render_sprite_overwritten(globe_target_id,fb_globe.tex);
	Core::gRenderer.sprite_shader.upload_float("vFlip",.0f);
	// TODO: remove this from acceptable routine to correct corrupt target textures
}

/*
	!O(1)m /+load -> (private)
	purpose: store checkbox information
	\param vscroll: current text scroll to align checkbox with it's corresponding entity
*/
void MenuList::create_checkbox(float vscroll)
{
	// prepare lower-right corner with scroll
	float ledge = vscroll-MENU_LIST_HEAD_SIZE, cedge = vscroll-MENU_LIST_HEAD_HSIZE;
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
	};
	checkbox_vertices.insert(slider_vertices.end(),t_vertices.begin(),t_vertices.end());
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
	float lcorner = vscroll-MENU_LIST_HEAD_SIZE, mcorner = vscroll-MENU_LIST_HEAD_HSIZE;
	std::vector<float> t_vertices = {

		// concave trimming
		MENU_LIST_ATTRIBUTE_COMBINE-MENU_LIST_SLIDER_XPUSH,lcorner,0,
		MENU_LIST_ATTRIBUTE_COMBINE,mcorner,1,
		MENU_LIST_ATTRIBUTE_COMBINE-MENU_LIST_SLIDER_XPUSH,vscroll,0,

		// concave widening
		MENU_LIST_ATTRIBUTE_WTARGET+MENU_LIST_SLIDER_XPUSH,lcorner,0,
		MENU_LIST_ATTRIBUTE_WTARGET+MENU_LIST_SLIDER_XPUSH,vscroll,0,
		MENU_LIST_ATTRIBUTE_COMBINE,mcorner,1
	};
	slider_vertices.insert(slider_vertices.end(),t_vertices.begin(),t_vertices.end());
}
// FIXME: really? we allocate per slider? that is really dumb and lazy!
// FIXME: sliders, checkbox & dropdown boxing is slightly off-center
// TODO: creating the new temporary vertex list and then concat is very slow and simple minded

/*
	!O(1) /+function -> (private)
	purpose: map grid position of selected entity to view space list grid
	\returns position in view space grid
*/
uint8_t MenuList::calculate_grid_position()
{
	const MenuListComplex& crr = mlists[active_ids.back()];
	return crr.entities[crr.select_id].grid_position-crr.lscroll;
}


/**
 *			Start Implementation of Popup Dialogue for Main Menu
 *
 *	The MenuDialogue utilizes the LDC list language to display a dialogue window, prompting the player to
 *	make a choice.
 *	It is a secondary, attention-grabbing way to display a short list of options, that are meant to be
 *	answered as an immediate response to proceed, without the power of a full MenuList.
 *
 *		Following, some explanations about the geometrical behaviour
 *
 *				     __________fw___________
 *									   ____.ur
 *					 ul._____----------   /		|
 *				       /		        /		|
 *				      /      .c       /			| fh
 *				     /            __/.lr		|
 *				 ll./______-------				|
 *
 *		...what we can learn from this beautiful drawing:
 *
 *	The point c marks the parametrical center of the created dialogue. from there, the dimensions are applied.
 *	fw is the full parametrical width and fh is the full height.
 *	The points ur and ll are predictably placed. ur will be at c+(fw/2,fh/2) and ll at c-(fw/2,fh/2).
 *	ul and lr will be spanned over the points c, ur & ll, being placed pseudorandomly within a range for
 *	asthetic reasons.
 *	The dialogue selector scaling is based on the text size of the listed options.
 *
 *		Custom Definition Rules
 *
 *	- Just one rule (so far). To transform an ui component in 2D space use the first two float components
 *	 of the first entity in the cluster as transformation vector.
 *
 *		Features
 *
 *	- Call add_dialogue_window() to create a new dialogue window and receive instance id back.
 *	- After creating all windows run load().
 *	- Dialogues are closed by default. to begin opening/closing process run open_dialogue()/close_dialogue().
 *	- Insert update() into unaltered menu render stage.
 *	- Insert background_component() into intersectionally inverting, anti-aliased menu render stage.
 *	- Return from choice is stored in status while the frame the choice was confirmed in.
 *	- Modification of dialogue data list is possible from outside by directly modifying dg_data.
*/

/*
	!O(n)bnmn .clusters in loaded ldc file /+load -> (public)
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
	uint8_t out = dg_data.size(), cidx = out;
	float hwidth = width*.5f, hheight = height*.5f;
	dg_data.resize(out+clusters.size());
	for (LDCCluster& cluster : clusters) {

		// store option count of current dialogue window
		size_t opcount = cluster.elist.size();

		// on-demand translation handling through ldc floats command
		glm::vec2 t_center = center;
		if (cluster.elist[0].fattribs.size()>1)
			t_center += glm::vec2(cluster.elist[0].fattribs[0],cluster.elist[0].fattribs[1]);

		// setup dialogue data with list start position
		dg_data[cidx] = {
			.liststart_y = t_center.y+dsize*(opcount>>1)+(dsize>>1)*(opcount&1),
			.max_width = hwidth,
			.max_height = hheight,
			.max_options = (uint8_t)(opcount-1),
			.action = std::vector<DialogueActionTuple>(opcount),
			.option_size = dsize,
			.tx_title = Text(tfont),
			.tx_options = Text(ofont),
			.tx_descriptions = Text(dfont),
			.title_length = cluster.id.length()
		};
		SingularDialogueData& dgd = dg_data[cidx++];

		// precalculation & background vertices
		DialogueBackgroundGeometry t_dbg[] = {
			{ glm::vec2(t_center.x-hwidth,t_center.y-hheight),0 },
			{ glm::vec2(t_center.x+hwidth,t_center.y+hheight),0 },
			{ t_center,1 },
			{ glm::vec2(t_center.x-hwidth,t_center.y-hheight),0 },
			{ t_center,2 },
			{ glm::vec2(t_center.x+hwidth,t_center.y+hheight),0 }
		};
		bgr_verts.insert(std::end(bgr_verts),std::begin(t_dbg),std::end(t_dbg));
		// FIXME: performance, repeated insert where predictable

		// selector vertices
		uint8_t mdos = dsize*.8f, hmdos = mdos>>1;
		float xoffcenter = t_center.x-hwidth;
		glm::vec2 t_sverts[] = {
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y),
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y),
			glm::vec2(xoffcenter-hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y-mdos),
			glm::vec2(xoffcenter+hmdos,dgd.liststart_y)
		};
		slc_verts.insert(std::end(slc_verts),std::begin(t_sverts),std::end(t_sverts));
		// TODO: implement prototype selector style
		//	first figure out the idea to make it look nice before blindly implementing
		// TODO: implement choice description print & change list selector to background geometry

		// dialogue title text setup
		dgd.tx_title.add(cluster.id.c_str(),
				t_center+glm::vec2(0,hheight*MENU_DIALOGUE_OFFSET_FACTOR));
		dgd.tx_title.load();

		// dialogue option text setup
		for (uint8_t i=0;i<opcount;i++) {

			// action setup
			dgd.action[i] = {
				.type = cluster.elist[i].etype,
				.value = cluster.elist[i].tdata
			};

			// information print setup
			dgd.tx_options.add(
					cluster.elist[i].head.c_str(),
					glm::vec2(
						t_center.x-hwidth*MENU_DIALOGUE_OFFSET_FACTOR,
						dgd.liststart_y-dsize*i)
				);
			dgd.tx_descriptions.add(cluster.elist[i].description,glm::vec2(1030,350-720*i),
					200.f,20.f);

			// calculate characters to draw for each text
			dgd.option_length += cluster.elist[i].head.length();
			dgd.description_length += cluster.elist[i].description.length();
		}
		dgd.tx_options.load(), dgd.tx_descriptions.load();
	}

	// return parent id
	return out;
}

/*
	!O(1) /load -> (public)
	purpose: compile shaders & setup dialogue buffers for background & selector
*/
void MenuDialogue::load()
{
	// upload background vertices
	bgr_buffer.bind();
	bgr_buffer.upload_vertices(bgr_verts);

	// background shader setup
	size_t vsize = sizeof(DialogueBackgroundGeometry);
	bgr_shader.compile("./shader/menu/dialogue.vs","./shader/menu/dialogue.fs");
	bgr_shader.def_irregular_attributeF("position",2,vsize,offsetof(DialogueBackgroundGeometry,position));
	bgr_shader.def_irregular_attributeI("disp_id",1,vsize,offsetof(DialogueBackgroundGeometry,disp_id));
	bgr_shader.upload_vec2("displace[0]",glm::vec2(0));
	bgr_shader.upload_camera();

	// upload selector vertices
	slc_buffer.bind();
	slc_buffer.upload_vertices(slc_verts);

	// selector shader setup
	slc_shader.compile("./shader/menu/dialogue_selector.vs","./shader/menu/dialogue_selector.fs");
	slc_shader.def_attributeF("position",2,0,2);
	slc_shader.upload_camera();
}
// TODO: store info of defined dialogue background vertices and create only once to avoid multiple allocations

/*
	!O(n)m .active and closing ids /function -> (public)
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

	// system must be active now
	system_active = true;
}

/*
	!O(n)m .active and opening ids /function -> (public)
	purpose: declare referenced dialogue as currently closing and inactive
	\param did: dialogue id
*/
void MenuDialogue::close_dialogue(uint8_t did)
{
	// move dialogue id to closing state
	opening_ids.erase(std::remove(opening_ids.begin(),opening_ids.end(),did),opening_ids.end());
	active_ids.erase(std::remove(active_ids.begin(),active_ids.end(),did),active_ids.end());
	closing_ids.push_back(did);

	// reset selection after close
	dg_data[did].sindex = 0;
}

/*
	!O(1)b /update -> (public)
	purpose: handle inputs, update dialogue state & draw non-background components
	\param input: reference to processed input relevant to main menu
*/
void MenuDialogue::update(ProcessedMenuInput& input)
{
	// reset dialogue state & update if dialogue system is currently active
	status = 0;
	system_active = opening_ids.size()||active_ids.size()||closing_ids.size();

	// check selector update conditions
	if (!active_ids.size()||opening_ids.size()) return;
	uint8_t id = active_ids.back();

	// close dialogue on request & skip selection update
	if (input.hit_b) {
		close_dialogue(id);
		return;
	}

	// update interactions, input will act upon the last element of idle dialogues
	// this can result in a recursive dialogue interaction (filo principle)
	// starting with selection
	SingularDialogueData& dgd = dg_data[id];
	if (!*input.mouse_preferred_peripheral) dgd.sindex += input.dir_vert;
	else dgd.sindex = (input.mouse_cartesian.y-dgd.liststart_y)/dgd.option_size*-1;
	dgd.sindex = (dgd.sindex<0) ? 0 : (dgd.sindex>dgd.max_options) ? dgd.max_options : dgd.sindex;

	// confirmation condition & set dialogue response state
	if (!input.hit_a) return;
	switch (dgd.action[dgd.sindex].type) {
	case LDCEntityType::RETURN: status = dgd.action[dgd.sindex].value;
		break;
	case LDCEntityType::SUBSEQUENT: open_dialogue(dgd.action[dgd.sindex].value);
		break;
	case LDCEntityType::SYSTEM:close_dialogue(id);
		break;
	}
	// TODO: this assumes all system behaviours simply close the dialogue
}
// FIXME: text tech results in ugly edge artifacts around letters after inverting to background (mdc)
// FIXME: avoid constant draw recalling, maybe better performance with triangle crunching (mdc)
// FIXME: (rare) flickering dialogue disappearance when closing dialogue (mdc)

/*
	!O(n)b .active ids /update -> (public)
	purpose: draw menu dialogues with selectors and entity text
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
	SingularDialogueData& dgd = dg_data[active_ids.back()];

	// show description of selected option
	dgd.tx_descriptions.prepare();
	dgd.tx_descriptions.set_scroll(glm::translate(glm::mat4(1.f),glm::vec3(.0f,720.f*dgd.sindex,.0f)));
	dgd.tx_descriptions.render(dgd.description_length,glm::vec4(1.f));

	// write in-dialogue text
	for (uint8_t id : active_ids) {
		dg_data[id].tx_title.prepare();
		dg_data[id].tx_title.render(dg_data[id].title_length,glm::vec4(DIALOGUE_HEAD_COLOUR,1.f));
		dg_data[id].tx_options.prepare();
		dg_data[id].tx_options.render(dg_data[id].option_length,glm::vec4(DIALOGUE_OPTION_COLOUR,1.f));
	}
}

/*
	!O(n)bm .compiled amount of relevant ids in processing lists /update -> (public)
	purpose: draw & update parts of dialogue as inverted background components on intersection with geometry
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
		}
		else i++;
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
		}
		else i++;
	}
}

/*
	!O(1) /update -> (public)
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
 *			Start Implementation of Interface Behaviour
 *
 *	This section defines the static functions the menu update will point to for logic switching.
 *	The variable interface_logic_id will be modified according to the activated OptionLogicID.
 *	Then, due to exact correlation with memory index, this variable will switch to appropriate mode handling.
 *
 *		Modes
 *
 *	A menu mode describes a certain sub-behaviour of the MainMenu.
 *	In default mode (interface_behaviour_macro) at index = 0 the user is supposed to choose a sub-mode to
 *	proceed selection in.
 *	As a result the interface behaviour (now macro) changes into the desired mode for the next update cycle.
 *
 *		Adding / Removing modes
 *
 *	When modifying MainMenu it might be necessary to add or remove menu modes:
 *	1a (addition). create static function among the following implementations, which handles mode logic.
 *	1b (removal). remove function which handles logic of undesired mode.
 *	2. go to interface_behaviour array and add/remove function pointer to list.
 *	3. go to main_options array and add/remove display name.
 *	4. visit OptionLogicID in header file and adjust mode enumeration.
 *	4+. make sure OptionLogicID enumeration, interface_behaviour and main_options are aligned by index.
 *	NOTE: the behaviour at index = 0 has to stay the default behaviour for start menus.
*/


typedef void (*interface_logic)(MainMenu&);

/*
	~O(n)b .amount of horizontal selection if mouse input /+function -> interface_logic (local,static)
	purpose: handle macro menu logic in case no main option was chosen or start menu is displayed
*/
void interface_behaviour_macro(MainMenu& tm)
{
	// menu transition
	bool no_transition = !tm.input.hit_a||tm.menu_action;
	tm.menu_action = (tm.menu_action||tm.input.hit_a)&&!tm.input.hit_b;

	// early exit: processing selection input
	if (!tm.menu_action) return;
	uint8_t t_select = tm.vselect;

	// mouse input
	// iterative approximation of mouse selection
	if (*tm.input.mouse_preferred_peripheral) {
		while (
				t_select>0
				&& tm.input.mouse_cartesian.x<(tm.mo_cposition[t_select].x-tm.mo_prog.x)
			) t_select--;
		while (
				t_select<OptionLogicID::OPTION_CAP
				&& tm.input.mouse_cartesian.x>(tm.mo_cposition[t_select].x+tm.mo_twidth[t_select]+tm.mo_prog.x)
			) t_select++;
	}

	// directional input
	else t_select += tm.input.dir_horz;

	// detect selection change, then write those changes
	bool no_changes = tm.vselect==t_select;
	tm.vselect = t_select;

	// confirmation
	bool confirmation = tm.input.hit_a&&no_transition;
	tm.request_close = (tm.vselect==OptionLogicID::MACRO_EXIT)*confirmation;
	tm.interface_logic_id = tm.vselect*confirmation;

	// early exit: selection splash update calculations
	tm.logic_setup = 0;
	if (no_changes&&no_transition) return;

	// change main option selector dimensions based on selected option
	SelectionSpliceKey& t_ssk = tm.splices_geometry.splices[tm.splice_selection_id].ssk[0];
	glm::vec2 vrt_cpos = tm.mo_cposition[tm.vselect]+glm::vec2(tm.mo_hwidth[tm.vselect],-MENU_OPTIONS_HSIZE);
	t_ssk.disp_lower.x = (vrt_cpos.x-SPLICE_LOWER_CENTER.x)*SPLICE_OFFCENTER_MV+SPLICE_LOWER_CENTER.x;
	tm.lext_selection = rand()%(uint16_t)tm.mo_hwidth[tm.vselect];
	tm.uext_selection = rand()%(uint16_t)tm.mo_hwidth[tm.vselect];

	// project upper displacement position based on lower displacement
	glm::vec2 vrt_dir = vrt_cpos-t_ssk.disp_lower;
	float vrt_extend_II = (MATH_CARTESIAN_YRANGE-vrt_cpos.y)/vrt_dir.y;
	t_ssk.disp_upper.x = vrt_cpos.x+vrt_dir.x*vrt_extend_II;

	// update randomized text rotation
	tm.st_rot = glm::radians((float)(rand()%MENU_OPTIONS_RDEG_THRES)*-((rand()&1)*2-1));
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: handle options menu, check for changes after exit and write or reset based on user input
*/
void interface_behaviour_options(MainMenu& tm)
{
	bool open_conf = false;

	// switch through option editing stages
	// open list
	switch (tm.logic_setup) {
	case 0:
		tm.mlists.open_list(tm.ml_options);
		tm.logic_setup++;
		break;

	// keep alive until list closes, then advance to next stage
	case 1:
		tm.logic_setup += !tm.mlists.system_active;
		break;

	// check for changes & open confirmation dialogue on condition
	case 2:
		for (uint8_t i=tm.ml_options+1;i<tm.ml_options+MENU_OPTIONS_HEADINGS_COUNT;i++)
			open_conf = tm.mlists.linked_variables_changed(i,tm.queued_restart)||open_conf;
		if (open_conf) tm.mdialogues.open_dialogue(tm.dg_optsave);
		tm.logic_setup++;
		break;

	// potential final stage: handle all possible dialogue responses
	default:

		// handle writing changes
		if (tm.mdialogues.status==1&&tm.input.hit_a) {
			for (uint8_t i=tm.ml_options+1;i<tm.ml_options+5;i++) tm.mlists.write_attributes(i);
			Init::write_changes();
			tm.request_restart = tm.queued_restart;
			tm.mdialogues.close_dialogue(tm.dg_optsave);
		}

		// handle settings reset
		else if (tm.mdialogues.status==2&&tm.input.hit_a) {
			for (uint8_t i=tm.ml_options+1;i<tm.ml_options+5;i++) tm.mlists.reset_attributes(i);
			tm.queued_restart = false;
			tm.mdialogues.close_dialogue(tm.dg_optsave);
		}

		// handle universal abort request
		else if (tm.input.hit_a||tm.input.hit_b) {
			tm.logic_setup = 0;
			tm.queued_restart = false;
		}
		tm.interface_logic_id *= tm.mdialogues.system_active;
	}
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: handle extras selection and display in the future
*/
void interface_behaviour_extras(MainMenu& tm)
{
	// open list of extra content
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_extras);
		tm.logic_setup = true;
	}

	// keep alive as long as extras list is active
	tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: handle practice scenario display and selection
*/
void interface_behaviour_practice(MainMenu& tm)
{
	// open list of practice scenarios
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_stages);
		tm.logic_setup = true;
	}

	// TODO: jump to selected pratice board

	// keep alive as long as practice list is active
	tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: handle savestate display and load based on users choice
*/
void interface_behaviour_load(MainMenu& tm)
{
	// open list of savestates
	if (!tm.logic_setup) {
		tm.mlists.open_list(tm.ml_saves);
		tm.logic_setup = true;
	}

	// push selected load instruction on confirm
	if (tm.mlists.status) Core::gLI.push(tm.savestates.saves[tm.mlists.status-1].ld_inst);

	// keep alive as long as save list is active
	tm.interface_logic_id *= tm.mlists.system_active;
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: question certainty of user to continue last load and proceed with reasonable response
*/
void interface_behaviour_continue(MainMenu& tm)
{
	// open dialogue for continuation request
	if (!tm.logic_setup) {
		tm.mdialogues.open_dialogue(tm.dg_continue);
		tm.logic_setup = true;
	}

	// processing continue dialogue response
	// response: continue instruction
	switch (tm.mdialogues.status) {
	case 1:
		Core::gLI.push(/*tm.savestates.saves[0].ld_inst*/LOAD_AREA51);
		break;

	// response: change run, therefore transition to load mode
	case 2:
		tm.mdialogues.close_dialogue(tm.dg_continue);
		tm.interface_logic_id = OptionLogicID::LOAD;
		tm.logic_setup = false;
	}

	// closing behaviour
	tm.interface_logic_id *= tm.mdialogues.system_active;
}

/*
	!O(1)b /+function -> interface_logic (local,static)
	purpose: initiate dialogue sequence to create a new game based on users difficulty preferences
*/
void interface_behaviour_newgame(MainMenu& tm)
{
	switch (tm.logic_setup) {

	// opening difficulty popup dialogue when option first is chosen
	case 0:
		tm.mdialogues.open_dialogue(tm.dg_diffs);
		tm.logic_setup++;

	// switch to confirmation dialogue
	case 1:
		tm.logic_setup += tm.mdialogues.status==1;
		break;

	// open confirmation dialogue
	case 2:
		tm.mdialogues.close_dialogue(tm.dg_diffs);
		tm.mdialogues.close_dialogue(tm.dg_diffs+1);
		tm.mdialogues.open_dialogue(tm.dg_diffs+2);
		tm.logic_setup++;

	// wait for confirmation
	case 3:
		if (tm.mdialogues.status==1) {
			std::cout << "TODO: create new save file\n";
			tm.mdialogues.close_dialogue(tm.dg_diffs+2);
		}
		break;
		// TODO: actually implement functionality of the menu option in the future
	}
	tm.interface_logic_id *= tm.mdialogues.system_active;
}


// menu state references
const interface_logic interface_behaviour[] = {
	interface_behaviour_macro,
	interface_behaviour_options,
	interface_behaviour_extras,
	interface_behaviour_practice,
	interface_behaviour_load,
	interface_behaviour_continue,
	interface_behaviour_newgame
};

// option display names
const char* main_options[OptionLogicID::LOGIC_COUNT] = {
	"exit",
	"options",
	"extras",
	"practice",
	"load",
	"continue",
	"new game"
};


/**
 *			Implementation UI: MainMenu Component
 *
 *	Starting interaction of the user with the program.
 *
 *		Progression
 *
 *	1) start menu, setting the mood, displaying the version and imploring the user to engage.
 *	2) macro selection of straight forward menu options.
 *	3) depending on the selected macro option proceed with user dialogue.
 *
 *		Style Statement
 *
 *	There are a lot of style improvements still waiting to be done, as well as tweaks to the current style.
 *
 *		Performance Statement
 *
 *	Everything here is still quite unoptimized, including most of the used components.
 *	Anyways this will be done later alongside with improving visuals.
*/


/*
	!O(n)bm .logic + display count /load -> UI (public)
	purpose: setup menu environment, populate with lists and define input possibilities
*/
MainMenu::MainMenu(CCBManager* ccbm,World* world,float& progress,float pseq)
	: m_ccbm(ccbm),m_world(world)
{
	// pointers
	input.mouse = &Core::gFrame.mouse;
	input.mouse_preferred_peripheral = &Core::gFrame.mpref_peripheral;
	// TODO: this proves it! mouse has to be part of input mapping at once!

	// asset load
	index_ranim = Core::gRenderer.atlas.size();
	ccbm->add_lv("lvload/main_menu.ccb");

	// version annotation text setup
	std::string vmessage =
			"yomisensei by callidaria. danmaku v"+std::to_string(INFO_VERSION_RELEASE)
			+ '.'+std::to_string(INFO_VERSION_SUBRELEASE)
			+ '.'+std::to_string(INFO_VERSION_DEVSTEP)+INFO_VERSION_MODE_SUFFIX
			+ " - running on cascabel base (OpenGL)";
	tx_version.add(vmessage.c_str(),TEXT_VERSION_POSITION),tx_version.load();
	tcap_version = vmessage.length();

	// peripheral sensitive input request annotations
	update_peripheral_annotations();

	// main menu options text
	// store text widths in lookup & estimate spacing between words
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
		uint32_t wwidth = fnt_mopts.estimate_textwidth(main_options[i]);
		mo_twidth[i] = wwidth, mo_hwidth[i] = wwidth*.5f;
		mo_prog.x -= wwidth;
	}
	mo_prog /= glm::vec2(OptionLogicID::LOGIC_COUNT);

	// write options text and store resulting positions
	glm::vec2 mo_cursor = MENU_OPTIONS_CLEFT+glm::vec2(mo_hwidth[0],0);
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
		tx_mopts[i].add(main_options[i],glm::vec2(-mo_hwidth[i],0)), tx_mopts[i].load();
		mo_cposition[i] = mo_cursor;
		mo_cursor += mo_prog+glm::vec2(mo_twidth[i],0);
	}

	// setup head splice
	// 1st key: splice disable in case the start screen is displayed
	// 2nd key: underlining the parent menu options selected by the following splice
	// 3rd key: list selection expansion, perfectly fitting list elements, to be modified at change
	// 4th key: tilted splice underlining descriptions for dialogue options if opened
	splice_head_id = splices_geometry.create_splice(
			SPLICE_HEAD_LOWER_START,SPLICE_HEAD_UPPER_START,
			0,0,
			SPLICE_HEAD_COLOUR,true,&tkey_head
		);
	splices_geometry.add_anim_key(
			splice_head_id,
			SPLICE_HEAD_LOWER_START,SPLICE_HEAD_UPPER_START,
			SPLICE_HEAD_LOWER_WIDTH,SPLICE_HEAD_UPPER_WIDTH
		);
	head_mod_id = splices_geometry.add_anim_key(
			splice_head_id,
			glm::vec2(0,SPLICE_HEAD_ORIGIN_POSITION),glm::vec2(1280,SPLICE_HEAD_ORIGIN_POSITION),
			SPLICE_HEAD_ORIGIN_WIDTH,SPLICE_HEAD_ORIGIN_WIDTH
		);
	splices_geometry.add_anim_key(
			splice_head_id,
			glm::vec2(0,SPLICE_HEAD_DLGDESC_QUAD),glm::vec2(SPLICE_HEAD_DLGDESC_QUAD,0),
			SPLICE_HEAD_DLGDESC_WIDTH,SPLICE_HEAD_DLGDESC_WIDTH
		);

	// setup selection splice
	// 1st key: splice disable, zero'd so the splice gets projected into scene when start is pressed
	// 2nd key: tilted splice underlining descriptions for list options if selected
	splice_selection_id = splices_geometry.create_splice(
		SPLICE_LOWER_CENTER,glm::vec2(0,MATH_CARTESIAN_YRANGE),
			0,0,
			SPLICE_SELECTION_COLOUR,false,&tkey_selection
		);
	splices_geometry.add_anim_key(
			splice_selection_id,
			glm::vec2(0,-SPLICE_HEAD_DLGDESC_QUAD),glm::vec2(SPLICE_HEAD_DLGDESC_QUAD,720),
			SPLICE_HEAD_DLGDESC_WIDTH,SPLICE_HEAD_DLGDESC_WIDTH
		);

	// setup title splice
	// 1st key: start request screen stylesplice
	// 2nd key: static title underline splice for the rest of the menu, after start request screen
	uint8_t sid = splices_geometry.create_splice(
			SPLICE_TITLE_LOWER_START,SPLICE_TITLE_UPPER_START,
			SPLICE_TITLE_LOWER_SWIDTH,SPLICE_TITLE_UPPER_SWIDTH,
			SPLICE_TITLE_COLOUR,false,&tkey_title
		);
	splices_geometry.add_anim_key(
			sid,
			SPLICE_TITLE_LOWER_MOD,SPLICE_TITLE_UPPER_MOD,
			SPLICE_TITLE_LWIDTH_MOD,SPLICE_TITLE_UWIDTH_MOD
		);
	splices_geometry.load();

	// list setup
	ml_options = mlists.define_list("./lvload/options.ldc");
	ml_extras = mlists.define_list("./lvload/extras.ldc");
	ml_stages = mlists.define_list("./lvload/stages.ldc");
	ml_saves = mlists.define_list(savestates);

	// add options for available screens
	// allocate memory
	Font t_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_LIST_HEAD_SIZE,MENU_LIST_HEAD_SIZE);
	uint8_t max_displays = SDL_GetNumVideoDisplays();
	MenuListEntity& cmle = mlists.mlists[ml_options+2].entities[MENU_OPTIONS_MONITOR_LISTING];
	cmle.dd_options.resize(max_displays);
	cmle.dd_colours.resize(max_displays);
	cmle.dd_length.resize(max_displays);

	// write dropdown text for all found monitors
	for (uint8_t i=0;i<max_displays;i++) {
		Text t_ddo = Text(t_font);
		t_ddo.add(("Monitor "+std::to_string(i)).c_str(),
				glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START-3*MENU_LIST_SCROLL_Y)),
			t_ddo.load();
		cmle.dd_options[i] = t_ddo;
		cmle.dd_colours[i] = glm::vec4(1.f);
		cmle.dd_length[i] = 8+(i>9)+(i>99);
	}

	// avoiding segfault should (for any mystical reason) no monitor be available
	if (!max_displays) {
		cmle.dd_options.push_back(Text(t_font));
		cmle.dd_options.back().add("ERROR!",
				glm::vec2(MENU_LIST_ATTRIBUTE_COMBINE,MENU_LIST_SCROLL_START-3*MENU_LIST_SCROLL_Y)),
			cmle.dd_options.back().load();
		cmle.dd_colours.push_back(TEXT_ERROR_COLOUR);
		cmle.dd_length.push_back(6);
	}
	mlists.load();

	// dialogue setup
	dg_diffs = mdialogues.add_dialogue_window("./lvload/challenge.ldc",glm::vec2(400,420),320,140,30,25);
	mdialogues.add_dialogue_window("./lvload/newgame.ldc",glm::vec2(640,360),320,250,30,25);
	dg_continue = mdialogues.add_dialogue_window("./lvload/continue.ldc",glm::vec2(640,360),320,250,30,25);
	dg_optsave = mdialogues.add_dialogue_window("./lvload/savechanges.ldc",glm::vec2(640,360),320,250,30,25);
	mdialogues.load();

	// buffers
	fb_menu.s.enable();
	fb_menu.s.upload_vec2("ratio",
			glm::vec2(Init::iConfig[FRAME_RESOLUTION_WIDTH],Init::iConfig[FRAME_RESOLUTION_HEIGHT]));
	fb_slice.s.enable();
	fb_slice.s.upload_int("gbuffer_colour",0);
	fb_slice.s.upload_int("gbuffer_normals",1);
	fb_slice.s.upload_int("menu_fb",2);
}

/*
	!O(n)bn .macro logic /update -> UI::render (public)
	purpose: render the main menu, calculate geometry and process interactions
*/
void MainMenu::render(FrameBuffer* game_fb,bool& running,bool& reboot)
{
	// button input
	bool plmb = Core::gFrame.mouse.mb[0]&&!trg_lmb, prmb = Core::gFrame.mouse.mb[2]&&!trg_rmb;
	input.hit_a = 
			(Base::gIMap.get_input_triggered(InputID::PAUSE)&&!menu_action)
			|| Base::gIMap.get_input_triggered(InputID::FOCUS)
			|| Base::gIMap.get_input_triggered(InputID::CONFIRM)
			|| plmb;
	input.hit_b =
			(Base::gIMap.get_input_triggered(InputID::PAUSE)&&menu_action)
			|| Base::gIMap.get_input_triggered(InputID::BOMB)
			|| prmb;

	// directional input
	input.dir_horz =
			((Base::gIMap.get_input_triggered(InputID::RIGHT)&&vselect<OptionLogicID::OPTION_CAP)
			- (Base::gIMap.get_input_triggered(InputID::LEFT)&&vselect>0))
			* menu_action;
	input.dir_vert =
			(Base::gIMap.get_input_triggered(InputID::DOWN)
			- Base::gIMap.get_input_triggered(InputID::UP))
			* menu_action;

	// mouse input
	input.mouse_cartesian = glm::vec2(
			Core::gFrame.mouse.mxfr*MATH_CARTESIAN_XRANGE,
			Core::gFrame.mouse.myfr*MATH_CARTESIAN_YRANGE
		);
	trg_lmb = Core::gFrame.mouse.mb[0], trg_rmb = Core::gFrame.mouse.mb[2];

	// timing
	transition_delta = TRANSITION_SPEED*Core::gFrame.time_delta;
	bool anim_go = anim_timing>ANIMATION_UPDATE_TIMEOUT;
	anim_timing += Core::gFrame.time_delta;

	// speedup animation advancement checking
	dt_tshiftdown += Core::gFrame.time_delta*speedup, dt_tnormalize += Core::gFrame.time_delta*!speedup;
	bool shiftdown_over = dt_tshiftdown>TITLE_SHIFTDOWN_TIMEOUT,
			normalize_over = dt_tnormalize>TITLE_NORMALIZATION_TIMEOUT;
	dt_tshiftdown -= TITLE_SHIFTDOWN_TIMEOUT*shiftdown_over,
			dt_tnormalize -= TITLE_NORMALIZATION_TIMEOUT*normalize_over;
	speedup = (speedup&&!shiftdown_over)||normalize_over;

	// transitions
	Toolbox::transition_float_on_condition(mtransition,transition_delta,menu_action);
	Toolbox::transition_float_on_condition(ftransition,transition_delta,interface_logic_id);
	inv_ftransition = 1.f-ftransition, inv_mtransition = 1.f-mtransition;

	// title animation
	// primitive rattling
	uint8_t rattle_mobility = RATTLE_THRESHOLD+RATTLE_THRESHOLD_RAGEADDR*menu_action,
		rattle_countermove = rattle_mobility>>1;
	glm::vec3 title_action = glm::vec3(
			(rand()%rattle_mobility-rattle_countermove)*anim_go,
			(rand()%rattle_mobility-rattle_countermove)*anim_go,
			0
		);

	// title shiftdown animation
	dt_tshiftdown *= menu_action, dt_tnormalize *= menu_action, speedup = speedup||!menu_action;
	float tshift = 1.f+SHIFTDOWN_ZOOM_INCREASE
			* ((speedup) ? sqrt(sin(dt_tshiftdown*SHIFTDOWN_OCTAPI)) : 1.f-sqrt(dt_tnormalize));

	// combining title animations
	glm::vec3 vrt_position = VRT_TITLE_START+VRT_TITLE_TRANSITION*mtransition+title_action,
		hrz_position = HRZ_TITLE_START+HRZ_TITLE_TRANSITION*mtransition+title_action;
	glm::mat4 vrt_scale = glm::translate(
			glm::scale(
				glm::translate(glm::mat4(1),-VRT_TITLE_SCALESET),
				glm::vec3(tshift)
			),VRT_TITLE_SCALESET
		),
	hrz_scale = glm::translate(
			glm::scale(
				glm::translate(glm::mat4(1),-HRZ_TITLE_SCALESET),
				glm::vec3(tshift)
			),HRZ_TITLE_SCALESET
		);
	Core::gRenderer.atlas[index_ranim].transform.model = glm::translate(glm::mat4(1),vrt_position)*vrt_scale,
	Core::gRenderer.atlas[index_ranim+1].transform.model = glm::translate(glm::mat4(1),hrz_position)*hrz_scale;
	// TODO: rework with the new transform features

	// peripheral switch for input request annotation
	if (input.controller_preferred_peripheral!=Core::gFrame.cpref_peripheral)
		update_peripheral_annotations();

	// component updates before interface behaviour & rendering
	bool rrnd = false;
	int8_t sd_grid = mlists.update(input,rrnd);
	mdialogues.update(input);

	// start render
	// prepare menu buffer
	fb_menu.bind();
	Frame::clear();

	// point towards interface logic
	interface_behaviour[interface_logic_id](*this);

	// render dare message
	tx_dare.prepare();
	tx_dare.set_scroll(glm::vec2(title_action));
	tx_dare.render(tcap_dare*inv_mtransition,TEXT_DARE_COLOUR);
	tx_version.prepare();
	tx_version.render(tcap_version*inv_mtransition,TEXT_VERSION_COLOUR);

	// render input instructions
	tx_instr.prepare();
	tx_instr.set_scroll(glm::vec2(0,-(!!ftransition+mlists.instruction_mod)*MATH_CARTESIAN_YRANGE));
	tx_instr.render(tcap_instr*mtransition,TEXT_INSTRUCTION_COLOUR);

	// render & transform main options
	for (uint8_t i=0;i<OptionLogicID::LOGIC_COUNT;i++) {
		tx_mopts[i].prepare();
		glm::mat4 opt_trans = glm::translate(glm::mat4(1.f),
				glm::vec3(mo_cposition[i].x+mo_hwidth[i],mo_cposition[i].y,0));
		glm::vec4 opt_colour = glm::vec4(TEXT_OPTIONS_COLOUR,mtransition);

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
	Core::gRenderer.atlas[index_ranim+1].transform.translate(glm::vec2(0,150*ftransition));
	Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite_frame(index_ranim,glm::vec2(3,0));
	Core::gRenderer.render_sprite_frame(index_ranim+1,glm::vec2(0,0));

	// update dialogues & lists
	mlists.render();
	mdialogues.render();
	// FIXME: request does not time input validity, work on input timing safety for unlocked update mode

	// multisampling
	// prepare msaa framebuffer for geometry section
	FrameBuffer::close();
	msaa.bind();
	Frame::clear();

	// curtain animation for main option selector splice
	float curtain_transition = mtransition-ftransition;
	SelectionSplice& css = splices_geometry.splices[splice_selection_id];
	css.ssk[0].ext_lower = lext_selection*curtain_transition;
	css.ssk[0].ext_upper = uext_selection*curtain_transition;

	// update selection splash geometry
	if (rrnd) {
		SelectionSpliceKey& csk = splices_geometry.splices[splice_head_id].ssk[head_mod_id];
		float htrans = SPLICE_HEAD_ORIGIN_POSITION-MENU_LIST_SCROLL_Y*sd_grid;
		csk.disp_lower.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD);
		csk.disp_upper.y = htrans+(rand()%SPLICE_HEAD_TILT_DBTHRESHOLD-SPLICE_HEAD_TILT_THRESHOLD);
		csk.ext_lower = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA);
		csk.ext_upper = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_DELTA);
	}
	// FIXME: splash dimensions to prevent aesthetically unfortunate proportions

	// splash render
	tkey_head = mtransition+ftransition+mdialogues.system_active;
	Toolbox::transition_float_on_condition(tkey_selection,transition_delta,mlists.system_active);
	tkey_title = mtransition;
	splices_geometry.update();

	// draw dialogue & list background components
	mlists.update_background_component();
	mdialogues.update_background_component();

	// finalize multisampling
	msaa.blit();

	// render menu
	fb_menu.prepare();
	fb_menu.s.upload_float("vignette",.44f+(float)(rand()%21)*.001f);
	fb_menu.s.upload_float("mtransition",mtransition);
	fb_menu.render();
	// TODO: this always happens when transitioning into any menu, do this outside
	//		this assumes the main menu utility to be rendered to scene instead of everything being ui
	//		decide how this should be handled and combine into most reduced implementation

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

	// framerate irrelevance through animation stall
	anim_timing -= ANIMATION_UPDATE_TIMEOUT*anim_go;

	// system
	reboot = request_restart;
	running = running&&!request_close&&!request_restart;
}

/*
	!O(1)bm /function -> (private)
	purpose: change instruction annotation for user when input peripheral has been changed, while playing
*/
void MainMenu::update_peripheral_annotations()
{
	// update shown preferred peripheral
	input.controller_preferred_peripheral = Core::gFrame.cpref_peripheral;

	// clear instruction text
	tx_dare.clear();
	tx_instr.clear();

	// assemble input instructions
	//	instr[0]: instructions for main horizontal selection
	//	instr[1]: instructions for main vertical selection
	//	instr[2]: instructions for dropdown expansion when selected, confirmation leads to instr[1]
	//	instr[3]: instructions for slider adjustment when selected
	std::string dmessage;
	std::string instr[TEXT_INSTRUCTION_COUNT];
	if (input.controller_preferred_peripheral) {

		// write messages for controller input
		dmessage = "press ["+Base::gIMap.cnt_name[InputID::PAUSE]+"] if you DARE";
		instr[0] =
				"confirm ["+Base::gIMap.cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.cnt_name[InputID::LEFT]
				+ '/'+Base::gIMap.cnt_name[InputID::RIGHT]+"]"
				+ "  go back ["+Base::gIMap.cnt_name[InputID::BOMB]+"]";
		instr[1] =
				"confirm ["+Base::gIMap.cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.cnt_name[InputID::UP]
				+ '/'+Base::gIMap.cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.cnt_name[InputID::BOMB]+"]";
		instr[2] =
				"see options ["+Base::gIMap.cnt_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.cnt_name[InputID::UP]
				+ '/'+Base::gIMap.cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.cnt_name[InputID::BOMB]+"]";
		instr[3] =
				"adjust slider ["+Base::gIMap.cnt_name[InputID::LEFT]
				+ '/'+Base::gIMap.cnt_name[InputID::RIGHT]+"]"
				+ "  select ["+Base::gIMap.cnt_name[InputID::UP]
				+ '/'+Base::gIMap.cnt_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.cnt_name[InputID::BOMB]+"]";
	} else {

		// write message for keyboard input
		dmessage = "press ["+Base::gIMap.key_name[InputID::CONFIRM]+"] if you DARE";
		instr[0] =
				"confirm ["+Base::gIMap.key_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.key_name[InputID::LEFT]
				+ '/'+Base::gIMap.key_name[InputID::RIGHT]+"]"
				+ "  go back ["+Base::gIMap.key_name[InputID::BOMB]+"]";
		instr[1] =
				"confirm ["+Base::gIMap.key_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.key_name[InputID::UP]
				+ '/'+Base::gIMap.key_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.key_name[InputID::BOMB]+"]";
		instr[2] =
				"see options ["+Base::gIMap.key_name[InputID::FOCUS]+"]"
				+ "  select ["+Base::gIMap.key_name[InputID::UP]
				+ '/'+Base::gIMap.key_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.key_name[InputID::BOMB]+"]";
		instr[3] =
				"adjust slider ["+Base::gIMap.key_name[InputID::LEFT]
				+ '/'+Base::gIMap.key_name[InputID::RIGHT]+"]"
				+ "  select ["+Base::gIMap.key_name[InputID::UP]
				+ '/'+Base::gIMap.key_name[InputID::DOWN]+"]"
				+ "  go back ["+Base::gIMap.key_name[InputID::BOMB]+"]";
	}

	// write dare message
	tx_dare.add(dmessage.c_str(),TEXT_DARE_POSITION);
	tcap_dare = dmessage.length();
	tx_dare.load();

	// write input instructions
	for (uint8_t i=0;i<TEXT_INSTRUCTION_COUNT;i++) {
		uint16_t estm = fnt_reqt.estimate_textwidth(instr[i].c_str())>>1;
		tx_instr.add(instr[i].c_str(),glm::vec2(MATH_CENTER_GOLDEN-estm,30+i*MATH_CARTESIAN_YRANGE));
		tcap_instr += instr[i].length();
	}
	tx_instr.load();
}
