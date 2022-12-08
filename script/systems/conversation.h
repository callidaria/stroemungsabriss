#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <cstring>
#include <vector>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "../../ccb/mat/camera2d.h"

#include "../../ccb/gfx/shader.h"

// OHOHO it looks like academia wasn't completely for nothing and worthless...
// i can actually maybe use some knowledge from university here? that's a first!
struct ConversationNode
{
	uint32_t node_id;
	std::string content;
	std::vector<ConversationNode> child_nodes;
	uint32_t jmp_id = 0;
};

class Conversation
{

public:

	// construction
	Conversation(const char* mm_path);
	~Conversation() {  }

	// interaction
	void input();

	// draw
	void render();

private:

	ConversationNode compile_node_data(std::vector<std::string> ls,uint32_t &si);
	std::string grind_raw_node_by_key(std::string raw,std::string key);
	uint32_t convert_rawid(std::string rawid);

private:

	// cascabel
	Buffer bgr_buffer = Buffer();
	Shader bgr_shader = Shader();
	Camera2D cam2D = Camera2D(1280,720);
	Text tspoken;

	ConversationNode croot;

};