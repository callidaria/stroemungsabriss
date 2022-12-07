#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

// OHOHO it looks like academia wasn't completely for nothing and worthless...
// i can actually maybe use some knowledge from university here? that's a first!
struct ConversationNode
{
	uint32_t node_id;
	std::string content;
	std::vector<ConversationNode> child_nodes;
};

class Conversation
{

public:

	// construction
	Conversation(const char* mm_path);
	~Conversation() {  }

private:

	ConversationNode compile_node_data(std::vector<std::string> ls,uint32_t si);

private:

	ConversationNode root;

};