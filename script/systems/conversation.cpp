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
	cnode.node_id = si;
	cnode.content = ls[si];
	std::cout << si << ":   " << ls[si] << " -> " << dp << '\n';

	// recursion for children nodes
	si++;
	while (strcmp(ls[si].c_str(),"</node>")) {
		if (!ls[si].rfind("<node")) cnode.child_nodes.push_back(compile_node_data(ls,si,dp+1));
		si++;
	} std::cout << '\n';

	// result
	return cnode;
}