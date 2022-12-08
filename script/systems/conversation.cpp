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
	croot = compile_node_data(lines,lidx,0);
}

/*
	TODO
*/
ConversationNode Conversation::compile_node_data(std::vector<std::string> ls,uint32_t &si,int dp)
{
	// node information
	ConversationNode cnode;
	cnode.node_id = convert_rawid(grind_raw_node_by_key(ls[si],"ID"));
	cnode.content = grind_raw_node_by_key(ls[si],"TEXT");
	std::cout << cnode.node_id << ":   " << cnode.content << " -> " << dp << '\n';

	// handle related nodes until closed
	si++;
	while (strcmp(ls[si].c_str(),"</node>")) {

		// recursion for children nodes
		if (!ls[si].rfind("<node")) cnode.child_nodes.push_back(compile_node_data(ls,si,dp+1));

		// add jump links
		else if (!ls[si].rfind("<arrowlink"))
			cnode.jmp_id = convert_rawid(grind_raw_node_by_key(ls[si],"DESTINATION"));

		// move head
		si++;
	} std::cout << '\n' << "jmp: " << cnode.jmp_id << "\n\n";

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