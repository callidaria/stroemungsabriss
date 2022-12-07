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
	compile_node_data(lines,0);
}

/*
	TODO
*/
ConversationNode Conversation::compile_node_data(std::vector<std::string> ls,uint32_t si)
{
	ConversationNode cnode;
	cnode.node_id = 0;
	cnode.content = "test";
	std::cout << '\n';
	uint32_t i = si+1;
	while (strcmp(ls[i].c_str(),"</node>")) {
		std::cout << ls[i] << '\n';
		cnode.child_nodes.push_back(compile_node_data(ls,i));
		i++;
	}
	return cnode;
}