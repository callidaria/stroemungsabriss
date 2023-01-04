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

// screen position specifications
constexpr float CONVERSATION_SPOKEN_TEXT_X = 450;
constexpr float CONVERSATION_SPOKEN_TEXT_Y = 125;
constexpr float CONVERSATION_CHOICE_ORIGIN_X = 50;
constexpr float CONVERSATION_CHOICE_ORIGIN_Y = 600;
constexpr float CONVERSATION_CHOICE_OFFSET = 30;

// OHOHO it looks like academia wasn't completely for nothing and worthless...
// i can actually maybe use some knowledge from university here? that's a first!
struct ConversationNode
{
	uint32_t node_id;
	std::string content;
	std::vector<ConversationNode> child_nodes;
	uint32_t jmp_id = 0;
};

// yes, the naming is very dramatic... it's on purpose.
class Conversation
{
public:

	// construction
	Conversation(const char* mm_path);
	~Conversation() {  }

	// interaction
	void engage(std::string tree_path);
	void input(bool cnf,bool up,bool down);

	// draw
	void render();

private:

	// tools
	ConversationNode rc_compile_node_data(std::vector<std::string> ls,uint32_t &si);
	std::string grind_raw_node_by_key(std::string raw,std::string key);
	uint32_t convert_rawid(std::string rawid);
	ConversationNode rc_depthsearch(ConversationNode root,uint32_t id);

	// loader
	void load_text();
	void load_choice();
	ConversationNode get_successor(ConversationNode node);

private:

	// cascabel
	Buffer bgr_buffer = Buffer();
	Shader bgr_shader = Shader();

	// text
	Camera2D cam2D = Camera2D(1280,720);
	Font bgrfont = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",25,25);
	Text tspoken = Text(&bgrfont),tdecide = Text(&bgrfont);

	// tree
	ConversationNode croot,ctemp;	// root & head nodes

	// animation
	uint16_t sltr_count = 0,dltr_count = 0;		// count of displayed letters
	uint8_t decision_id = 0;					// index of selected decision
	bool chlfr = false;							// conserve if input happened last frame
};