#include "conversation.h"

/*
	TODO
*/
Conversation::Conversation(const char* mm_path)
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
	croot = compile_node_data(lines,lidx);

	// background vertices
	float tfield_verts[] = { 425,25, 425,150, 1200,150, 1200,150, 1200,25, 425,25, };
	bgr_buffer.bind();
	bgr_buffer.upload_vertices(tfield_verts,sizeof(tfield_verts));

	// background shader
	bgr_shader.compile("./shader/cnv_background_vertex.shader",
			"./shader/cnv_background_fragment.shader");
	bgr_shader.def_attributeF("position",2,0,2);

	// 2D background projection
	Camera2D cam2D = Camera2D(1280,720);
	bgr_shader.upload_matrix("view",cam2D.view2D);
	bgr_shader.upload_matrix("proj",cam2D.proj2D);

	// create text
	tspoken.add(croot.content.c_str(),glm::vec2(450,125));
	tspoken.load(&cam2D);
}

/*
	TODO
*/
void Conversation::render()
{
	// draw background for spoken text
	bgr_shader.enable();
	bgr_buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6);

	// draw spoken text contents
	tspoken.prepare();
	tspoken.render(2048,glm::vec4(.8f,.2f,0,1));
}

/*
	TODO
*/
ConversationNode Conversation::compile_node_data(std::vector<std::string> ls,uint32_t &si)
{
	// node information
	ConversationNode cnode;
	cnode.node_id = convert_rawid(grind_raw_node_by_key(ls[si],"ID"));
	cnode.content = grind_raw_node_by_key(ls[si],"TEXT");

	// handle related nodes until closed
	si++;
	while (strcmp(ls[si].c_str(),"</node>")) {

		// recursion for children nodes
		if (!ls[si].rfind("<node")) cnode.child_nodes.push_back(compile_node_data(ls,si));

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
	for (auto cproc : raw) {
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