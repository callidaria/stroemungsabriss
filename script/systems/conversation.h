#ifndef SCR_SYSTEMS_CONVERSATION
#define SCR_SYSTEMS_CONVERSATION

#include <fstream>
#include <cstring>

#include "../../ccb/core.h"
#include "../../ccb/fcn/buffer.h"
#include "../../ccb/gfx/shader.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "character_manager.h"

// screen position specifications
constexpr float CONVERSATION_SPOKEN_TEXT_X = 720;
constexpr float CONVERSATION_CHOICE_ORIGIN_X = 50;
constexpr float CONVERSATION_CHOICE_ORIGIN_Y = 600;
constexpr float CONVERSATION_CHOICE_OFFSET = 25;
constexpr float CNV_BGR_ORIGIN_X = 705;
constexpr float CNV_BGR_WIDTH = 400;
constexpr float CNV_BORDER_NEWLINE = 380;
constexpr float CNV_BGR_DESTINATION_X = CNV_BGR_ORIGIN_X+CNV_BGR_WIDTH;
constexpr float CNV_SELECTOR_HEIGHT = 20;
constexpr float CNV_CREQUEST_WQUAD = 50;

// timing
constexpr float CNV_DISENGAGE_WAIT_FRAMES = 45;
constexpr float CNV_CONFIRMATION_COOLDOWN = 45;
// TODO: find the sweetspot for input framedropping

// i can actually maybe use some knowledge from university here? that's a first!
struct ConversationNode
{
	uint32_t node_id;
	std::string content = "";
	std::vector<ConversationNode> child_nodes;
	uint32_t jmp_id = 0;
	bool valueless = false;
	bool end_node = false;
	uint16_t char_id = 0;
	uint16_t mood_id = 0;
	uint16_t condition_id = 0;
	uint16_t condition_set = 0;
};

// yes, the naming is very dramatic... it's on purpose.
class Conversation
{
public:

	// construction
	Conversation(CharacterManager* cm,const char* mm_path);
	~Conversation() {  }

	// interaction
	void engage(std::string tree_path,std::vector<bool> cnd);
	void disengage();
	void input(bool cnf,bool up,bool down);

	// draw
	void render_to_scene();
	void render(GLuint scene_tex);

private:

	// tools
	ConversationNode rc_compile_node_data(std::vector<std::string> ls,uint32_t &si);
	std::string grind_raw_node_by_key(std::string raw,std::string key);
	uint32_t convert_rawid(std::string rawid);
	ConversationNode rc_depthsearch(ConversationNode root,uint32_t id);
	uint16_t count_instances(std::string text);
	void manipulate_background_edges();

	// loader
	void load_text();
	void load_choice();
	void jmp_successor();
	void mv_decision(uint8_t i);

	// write
	void log_speaker(std::string name,glm::vec4 colour);
	void log_content(std::string content);

private:

	// cascabel
	Buffer slct_buffer = Buffer(),bgr_buffer = Buffer(),mood_buffer = Buffer();
	Shader slct_shader = Shader(),bgr_shader = Shader(),mood_shader = Shader();
	CharacterManager* charManager;

	// text
	Font bgrfont = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",20,20);
	Text tdecide = Text(bgrfont),tname = Text(bgrfont);
	std::vector<Text> tspoken,tspoken_names;
	std::vector<glm::vec4> tcolour;
	glm::vec4 name_colour;
	CharacterData protag;

	// tree
	ConversationNode croot,ctemp;	// root & head nodes
	std::vector<ConversationNode> choices;
	std::vector<bool> cnd_list;

	// input
	uint16_t btn_rindex;
	glm::vec2 btn_position;
	bool input_blocked = false;

	// animation
	uint16_t sltr_count = 0,dltr_count = 0;		// count of displayed letters
	uint16_t sltr_target = 0;					// target to count displayed letters towards
	float tscroll = 0;							// defines scroll advancement of backlog
	uint8_t decision_id = 0;					// index of selected decision
	bool chlfr = false;							// conserve if input happened last frame
	float sEdges[4] = { 0 };					// choice selection edge modification
	uint16_t chosen_mood = 0;					// protagonist's mood index
	uint16_t curr_char,curr_cols,curr_mood;		// currently shown opposing character
	float cursor_y = 655;						// current cursor height
	GLuint curr_ctex;							// current character mood visualization texture
	uint8_t dwait = 0,iwait = 0;				// frame cooldown counter for disengage & input
};

#endif

// TODO: only read multi branch after conditional exclusion
// TODO: text colour picker for choices & speaker annotations
// TODO: protagonist portrait for chosen line mood visualization