#include "conversation.h"

/*
	TODO
*/
Conversation::Conversation(Renderer2D* r2D,CharacterManager* cm,const char* mm_path,std::string pname)
	: m_r2D(r2D),charManager(cm),protag_name(pname)
{
	// extract lines from conversation file
	std::ifstream mm_file(mm_path,std::ios::in);
	std::vector<std::string> lines;
	while (!mm_file.eof()) {
		std::string raw_node;
		getline(mm_file,raw_node);
		lines.push_back(raw_node);
	}

	// extract nodes from raw content
	uint32_t lidx = 2;
	croot = rc_compile_node_data(lines,lidx);

	// selector vertices
	float sfield_verts[] = { 0,-20,0, 0,0,1, 1280,0,2, 1280,0,2, 1280,-20,3, 0,-20,0, };
	slct_buffer.bind();
	slct_buffer.upload_vertices(sfield_verts,sizeof(sfield_verts));
	slct_shader.compile("./shader/fbv_lselect.shader","./shader/fbf_lselect.shader");
	slct_shader.def_attributeF("position",2,0,3);
	slct_shader.def_attributeF("index",1,2,3);
	slct_shader.upload_camera(cam2D);

	// conversation opponent visualizations
	std::vector<float> oppconv_verts
			= Toolbox::create_sprite_canvas_triangled(glm::vec2(1100,0),180,720);
	opps_buffer.bind();
	opps_buffer.upload_vertices(oppconv_verts);
	opps_shader.compile("./shader/vertex_mood.shader","./shader/fragment_mood.shader");
	opps_shader.def_attributeF("position",2,0,4);
	opps_shader.def_attributeF("texCoords",2,2,4);
	opps_shader.upload_camera(cam2D);
	// FIXME: isn't this compile2d() worthy?

	// load character spritesheets
	charManager->load_spritesheets();

	// background vertices
	float tfield_verts[] = { 705,0,0, 705,720,1, 1105,720,2, 1105,720,2, 1105,0,3, 705,0,0, };
	bgr_buffer.bind();
	bgr_buffer.upload_vertices(tfield_verts,sizeof(tfield_verts));

	// background shader
	bgr_shader.compile("./shader/cnv_background_vertex.shader",
			"./shader/cnv_background_fragment.shader");
	bgr_shader.def_attributeF("position",2,0,3);
	bgr_shader.def_attributeF("edge_id",1,2,3);
	bgr_shader.upload_camera(cam2D);
	manipulate_background_edges();

	// load continue button animation
	btn_rindex = m_r2D->add(glm::vec2(CONVERSATION_SPOKEN_TEXT_X,-50),50,50,
			"./res/continue_dialogue.png",2,3,5,30);
}

/*
	TODO
*/
void Conversation::engage(std::string tree_path)
{
	// setup
	std::string node_name = "";
	ctemp = croot;

	// follow tree path
	for (auto cproc : tree_path) {

		// check for node naming end
		if (cproc=='/') {
			for (auto child:ctemp.child_nodes) {

				// check content equivalence for all children linked in temporary node
				if (child.content==node_name) {
					ctemp = child;
					node_name = "";
				}
			}
		} // FIXME: i'll gouge my fucking eyes out looking at this

		// assemble node name from current character
		else node_name += cproc;
	}

	// move into immediate content node
	jmp_successor();
}

/*
	TODO
*/
void Conversation::input(bool cnf,bool up,bool down)
{
	// block input & complete filling when text not ready
	bool t_chlfr = chlfr;
	chlfr = (sltr_count<sltr_target)||chlfr;
	sltr_count += (sltr_target-sltr_count)*(cnf&&!t_chlfr);

	// in case of branch decision
	if (cnf&&!chlfr) {

		// load initial child from selected branch & reset
		if (dltr_count) mv_decision(decision_id);
		jmp_successor();

		// refresh edge manipulation
		bgr_shader.enable();
		manipulate_background_edges();
	}

	// in case of choosing
	else if ((up||down)&&!chlfr) {

		// change decision based on input, respective to minimum & maximum
		decision_id += down*(decision_id<(ctemp.child_nodes.size()-1))-up*(decision_id>0);

		// recalculate random selector edge changes
		for (uint8_t i=0;i<4;i++) sEdges[i] = rand()%15-10;
	}

	// set input trigger
	chlfr = cnf||up||down;
}

/*
	TODO
*/
void Conversation::render()
{
	// update letter count
	bool filling = sltr_count<sltr_target;
	sltr_count += filling;

	// upload selection splash modifications
	uint16_t strans = CONVERSATION_CHOICE_ORIGIN_Y-CONVERSATION_CHOICE_OFFSET*decision_id;
	slct_shader.enable();
	slct_shader.upload_vec2("idx_mod[0]",glm::vec2(0,strans+(rand()%10-5)-sEdges[0]));
	slct_shader.upload_vec2("idx_mod[1]",glm::vec2(0,strans+(rand()%10-5)+sEdges[1]));
	slct_shader.upload_vec2("idx_mod[2]",glm::vec2(0,strans+(rand()%10-5)+sEdges[2]));
	slct_shader.upload_vec2("idx_mod[3]",glm::vec2(0,strans+(rand()%10-5)-sEdges[3]));

	// draw selection indicator visuals
	slct_buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6*(dltr_count&&!filling));

	// draw opponent's mood visualization
	opps_shader.enable();
	opps_shader.upload_int("col_max",curr_cols);
	opps_shader.upload_float("col_index",curr_mood);
	opps_buffer.bind();
	glBindTexture(GL_TEXTURE_2D,curr_ctex);
	glDrawArrays(GL_TRIANGLES,0,6);

	// draw background for spoken text
	bgr_shader.enable();
	bgr_buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6);

	// draw speaker labels
	for (auto name : tspoken_names) {
		name.prepare();
		name.render(1024,glm::vec4(0,0,0,1));
	}

	// draw spoken text contents
	for (uint16_t i=0;i<tspoken.size();i++) {
		tspoken[i].prepare();
		tspoken[i].render(sltr_count+1024*(i!=tspoken.size()-1),tcolour[i]);
	}

	// draw decision list text contents
	tdecide.prepare();
	tdecide.render(dltr_count*!filling,glm::vec4(1,1,1,1));

	// show conversing character's name
	tname.prepare();
	tname.render(1024,name_colour);

	// animate continue request
	m_r2D->prepare();
	m_r2D->al[btn_rindex].model = glm::translate(glm::mat4(1.0f),
			glm::vec3(btn_position.x+1280*(sltr_count<sltr_target||dltr_count),
			btn_position.y+tscroll,0));
	m_r2D->render_anim(btn_rindex);
}

/*
	TODO
*/
ConversationNode Conversation::rc_compile_node_data(std::vector<std::string> ls,uint32_t &si)
{
	// node information
	ConversationNode cnode;
	cnode.node_id = convert_rawid(grind_raw_node_by_key(ls[si],"ID"));
	std::string raw_content = grind_raw_node_by_key(ls[si],"TEXT");

	// interpret prefix conditions & attributes
	while (raw_content[0]==':') {
		uint16_t prefix_value = uint16_t(raw_content[2]-'0');
		cnode.char_id = prefix_value*(raw_content[1]=='c')+cnode.char_id;
		cnode.mood_id = prefix_value*(raw_content[1]=='m')+cnode.mood_id;
		cnode.condition_id = prefix_value*(raw_content[1]=='b')+cnode.condition_id;
		raw_content.erase(0,4);
	}

	// convert content to be more readable & save to node
	bool rp_mode = false;		// is replace mode activated
	std::string idf = "";		// placeholder identifier
	bool brc_mode = false;		// bracket command mode
	std::string bracket = "";	// string to write bracket command to
	for (auto tmp:raw_content) {

		// enable/disable replace mode
		bool replace_off = tmp==';';
		rp_mode = ((!rp_mode&&tmp=='&')||rp_mode)&&(!replace_off);

		// accumulate replacement identifier
		if (rp_mode) idf += tmp;

		// identify placeholder equivalent
		else if (replace_off) {

			// replace based on placeholder pattern
			if (idf=="&apos") cnode.content += '\'';
			else if (idf=="&quot") cnode.content += '\"';

			// read & interpret bracket commands
			else if (idf=="&lt") brc_mode = true;
			else if (idf=="&gt") {

				// interpret bracket command
				cnode.valueless = cnode.valueless||(bracket=="null");

				// reset bracket reader values
				brc_mode = false;
				bracket = "";
			}

			// accumulate innocent node content characters
			else cnode.content += idf;
			// FIXME: foolish ifelsing

			// reset identifier
			idf = "";
		}

		// accumulate bracket command
		else if (brc_mode&&!rp_mode) bracket += tmp;

		// add innocent characters to content string
		else cnode.content += tmp;
	}

	// handle related nodes until closed
	si++;
	while (strcmp(ls[si].c_str(),"</node>")) {

		// recursion for children nodes
		if (!ls[si].rfind("<node")) cnode.child_nodes.push_back(rc_compile_node_data(ls,si));

		// add jump links
		else if (!ls[si].rfind("<arrowlink"))
			cnode.jmp_id = convert_rawid(grind_raw_node_by_key(ls[si],"DESTINATION"));

		// move head
		si++;
	}

	// result
	return cnode;
}

/*
	TODO
*/
std::string Conversation::grind_raw_node_by_key(std::string raw,std::string key)
{
	// setup
	uint8_t emode = 0;			// grind mode identifier. switches interpretation modes
	std::string ex_key = "";	// holding current extracted key to compare to target when '='
	std::string out = "";		// variable to hold extracted value to return

	// grind raw data
	for (auto cproc:raw) {
		switch (emode) {

		// find keystart
		case 0:
			emode = cproc==' ';
			break;

		// read key
		case 1:
			emode += cproc=='=';
			ex_key += cproc;
			break;

		// compare keys
		case 2:
			emode++;
			emode += !strcmp(ex_key.c_str(),(key+'=').c_str());
			ex_key = "";
			break;

		// discard until information ends
		case 3:
			emode *= cproc!='"';
			break;

		// extract value
		case 4:
			if (cproc=='"') return out;
			out += cproc;
			break;

		}
	} return "no such key in raw node data";
}

/*
	TODO
*/
uint32_t Conversation::convert_rawid(std::string rawid)
{ return stoi(std::string(rawid,3)); }

/*
	TODO
*/
ConversationNode Conversation::rc_depthsearch(ConversationNode root,uint32_t id)
{
	// check id equivalence
	if (root.node_id==id) return root;

	// recursive depth search for all children
	for (auto child:root.child_nodes) {

		// get branch result over entire depth & return if valid
		ConversationNode fnode = rc_depthsearch(child,id);
		if (fnode.node_id) return fnode;
	}

	// if nothing has been found return error node
	return { 0,"error: successor not found, jump disconnected",{},0 };
}

/*
	TODO
*/
uint16_t Conversation::count_instances(std::string text)
{
	uint16_t out = 0;
	for (auto chr : text) out += chr!=' ';
	return out;
}

/*
	TODO
	NOTE: background shader has to be enabled beforehand
*/
void Conversation::manipulate_background_edges()
{
	bgr_shader.upload_int("edge_offset[0]",rand()%37-30);
	bgr_shader.upload_int("edge_offset[1]",rand()%37-30);
	bgr_shader.upload_int("edge_offset[2]",rand()%37-7);
	bgr_shader.upload_int("edge_offset[3]",rand()%37-7);
}

/*
	TODO
*/
void Conversation::load_text()
{
	// load character name display
	if (ctemp.char_id) {
		curr_char = ctemp.char_id;
		std::string speaker_name = charManager->get_character(curr_char).name;

		// write name to spoken text output
		log_speaker(speaker_name,charManager->get_character(curr_char).text_colour);

		// update lower name display
		tname.clear();
		tname.add(speaker_name.c_str(),glm::vec2(905,33));
		tname.load(&cam2D);
	}

	// write text content
	log_content(ctemp.content);

	// advance scroll if y-axis border has been violated
	float scr_cursor = cursor_y+tscroll;
	tscroll -= (scr_cursor-50)*(scr_cursor<50);
	glm::mat4 tmodel = glm::translate(glm::mat4(1.0f),glm::vec3(0,tscroll,0));
	for (auto ltxt : tspoken_names) {
		ltxt.enable_shader();
		ltxt.set_scroll(tmodel);
	} for (auto stxt : tspoken) {
		stxt.enable_shader();
		stxt.set_scroll(tmodel);
	}
	// FIXME: scrolling gets more expensive in a linear fashion in relation to text
	// TODO: ??add a text instance for every participant in conversation and add upon those

	// load characters mood
	if (ctemp.mood_id) {
		CharacterData tmp = charManager->get_character(curr_char);

		// upload uniform variables
		curr_cols = tmp.cols;
		curr_mood = ctemp.mood_id-1;

		// preload opponent's spritesheet
		curr_ctex = tmp.tex_sheet;
	}

	// advance target count
	sltr_count = 0;
	sltr_target = count_instances(ctemp.content);
}

/*
	TODO
*/
void Conversation::load_choice()
{
	// reset previous branch data
	tdecide.clear();
	decision_id = 0;

	// write conversation branching lines
	for (uint16_t i=0;i<ctemp.child_nodes.size();i++) {

		// write line
		tdecide.add(ctemp.child_nodes[i].content.c_str(),
				glm::vec2(CONVERSATION_CHOICE_ORIGIN_X,
				CONVERSATION_CHOICE_ORIGIN_Y-i*CONVERSATION_CHOICE_OFFSET));

		// calculate decision letter capacity
		dltr_count += ctemp.child_nodes[i].content.length();
	}

	// load text instances & and reset letter count
	tdecide.load(&cam2D);
}

/*
	TODO
*/
void Conversation::jmp_successor()
{
	// proceed with next child if no jump address. need that adrenochrome
	bool multi_branch = false;
	do {
		if (ctemp.jmp_id) ctemp = rc_depthsearch(croot,ctemp.jmp_id);
		else {
			ConversationNode swp = ctemp.child_nodes[0];
			ctemp = swp;
		} multi_branch = ctemp.child_nodes.size()>1;
	}

	// run until conversation tree head has valued content or branches
	while (ctemp.valueless&&!multi_branch);

	// load text or choices based on value break
	if (!ctemp.valueless) load_text();
	if (multi_branch) load_choice();
	else dltr_count = 0;
}

/*
	TODO
*/
void Conversation::mv_decision(uint8_t i)
{
	// swap to chosen child node
	ConversationNode swp = ctemp.child_nodes[i];
	ctemp = swp;

	// write choice to conversation log
	log_speaker(protag_name,glm::vec4(1,.7f,0,1));
	log_content(swp.content);
}

/*
	TODO
*/
void Conversation::log_speaker(std::string name,glm::vec4 colour)
{
	// write name to spoken text output
	Text proc = Text(&bgrfont);
	proc.add((name+':').c_str(),glm::vec2(CONVERSATION_SPOKEN_TEXT_X,cursor_y));
	proc.load(&cam2D);
	tspoken_names.push_back(proc);

	// newline & load speaker's colour
	cursor_y -= CONVERSATION_CHOICE_OFFSET;
	name_colour = colour;
}

/*
	TODO
*/
void Conversation::log_content(std::string content)
{
	// write content to spoken text output
	Text proc = Text(&bgrfont);
	glm::vec2 cpos = proc.add(content,glm::vec2(CONVERSATION_SPOKEN_TEXT_X,cursor_y),380,
			CONVERSATION_CHOICE_OFFSET);
	proc.load(&cam2D);
	tspoken.push_back(proc);
	tcolour.push_back(name_colour);

	// update confirmation request & newline
	btn_position = cpos;
	cursor_y = cpos.y-CONVERSATION_CHOICE_OFFSET;
}