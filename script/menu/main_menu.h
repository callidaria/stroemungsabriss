#ifndef SRC_MENU_MAIN
#define SRC_MENU_MAIN

#include "../../ccb/core.h"
#include "../../ccb/fcn/init.h"
#include "../../ccb/fcn/ccb_manager.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "../../ccb/frm/framebuffer.h"
#include "../../ccb/ppe/msaa.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"

#include "../systems/savestates.h"
#include "../world.h"

// information
constexpr uint8_t INFO_VERSION_RELEASE = 0;
constexpr uint8_t INFO_VERSION_SUBRELEASE = 0;
constexpr uint8_t INFO_VERSION_DEVSTEP = 6;
constexpr char INFO_VERSION_MODE_SUFFIX = 'c';
// MODI: c = "development", t = "QA build", p = "polishing", R = "release"

// menu list positioning
constexpr uint8_t MENU_LIST_NEUTRAL_GRIDRANGE = 7;
constexpr uint16_t MENU_LIST_HEADPOS_X = 250;
constexpr uint16_t MENU_LIST_SCROLL_START = 515;
constexpr uint16_t MENU_LIST_SCROLL_Y = 45;
constexpr uint16_t MENU_LIST_HEAD_SIZE = 30;
constexpr uint16_t MENU_LIST_HEAD_HSIZE = MENU_LIST_HEAD_SIZE>>1;
constexpr uint16_t MENU_LIST_SEGMENT_PUSH_X = 100;
constexpr uint16_t MENU_LIST_ATTRIBUTE_OFFSET = 500;
constexpr uint16_t MENU_LIST_ATTRIBUTE_COMBINE = MENU_LIST_HEADPOS_X+MENU_LIST_ATTRIBUTE_OFFSET;
constexpr uint16_t MENU_LIST_ATTRIBUTE_WIDTH = 200;
constexpr uint16_t MENU_LIST_ATTRIBUTE_THEIGHT = MENU_LIST_SCROLL_START-MENU_LIST_HEAD_SIZE;
constexpr uint16_t MENU_LIST_ATTRIBUTE_HWIDTH = MENU_LIST_ATTRIBUTE_WIDTH>>1;
constexpr uint16_t MENU_LIST_ATTRIBUTE_WTARGET = MENU_LIST_ATTRIBUTE_COMBINE+MENU_LIST_ATTRIBUTE_WIDTH;
constexpr uint16_t MENU_LIST_ATTRIBUTE_HTARGET = MENU_LIST_ATTRIBUTE_COMBINE+MENU_LIST_ATTRIBUTE_HWIDTH;
constexpr uint16_t MENU_LIST_ATTRIBUTE_QUADRATIC = MENU_LIST_ATTRIBUTE_COMBINE+MENU_LIST_HEAD_SIZE;
constexpr uint16_t MENU_LIST_ATTRIBUTE_HQUADRATIC = MENU_LIST_ATTRIBUTE_COMBINE+MENU_LIST_HEAD_HSIZE;
constexpr uint8_t MENU_LIST_CHECKBOX_DRIFT_DIST = 2;
constexpr uint8_t MENU_LIST_SLIDER_XPUSH = 7;
constexpr glm::vec2 MENU_LIST_DESC_POSITION = glm::vec2(1030,350);
constexpr uint16_t MENU_LIST_DESC_BLOCKWIDTH = 200;
constexpr uint8_t MENU_LIST_DESC_NLINEJMP = 20;

// menu dialogue positioning
constexpr float MENU_DIALOGUE_OFFSET_FACTOR = .9f;

// title position & transition destination
constexpr glm::vec3 VRT_TITLE_START = glm::vec3(300,300,0);
constexpr glm::vec3 VRT_TITLE_END = glm::vec3(100,250,0);
constexpr glm::vec3 HRZ_TITLE_START = glm::vec3(590,800,0);
constexpr glm::vec3 HRZ_TITLE_END = glm::vec3(590,642,0);
constexpr glm::vec3 VRT_TITLE_TRANSITION = VRT_TITLE_END-VRT_TITLE_START;
constexpr glm::vec3 HRZ_TITLE_TRANSITION = HRZ_TITLE_END-HRZ_TITLE_START;
constexpr glm::vec3 VRT_TITLE_SCALESET = VRT_TITLE_END-glm::vec3(640,360,0);
constexpr glm::vec3 HRZ_TITLE_SCALESET = HRZ_TITLE_END-glm::vec3(640,360,0);

// head splice
constexpr float SPLICE_HEAD_ORIGIN_POSITION = MENU_LIST_SCROLL_START-MENU_LIST_HEAD_SIZE*.5f;
constexpr float SPLICE_HEAD_MINIMUM_WIDTH = 4.f;
constexpr float SPLICE_HEAD_ORIGIN_WIDTH = 15.f;
constexpr float SPLICE_HEAD_ORIGIN_DELTA = SPLICE_HEAD_ORIGIN_WIDTH-SPLICE_HEAD_MINIMUM_WIDTH;
constexpr glm::vec2 SPLICE_HEAD_LOWER_START = glm::vec2(0,520.f);
constexpr glm::vec2 SPLICE_HEAD_UPPER_START = glm::vec2(1280,470.f);
constexpr float SPLICE_HEAD_LOWER_WIDTH = 42.f-SPLICE_HEAD_ORIGIN_WIDTH;
constexpr float SPLICE_HEAD_UPPER_WIDTH = 50.f-SPLICE_HEAD_ORIGIN_WIDTH;
constexpr uint8_t SPLICE_HEAD_TILT_THRESHOLD = 10;
constexpr uint8_t SPLICE_HEAD_TILT_DBTHRESHOLD = SPLICE_HEAD_TILT_THRESHOLD<<1;
constexpr glm::vec2 SPLICE_HEAD_TITLE_OFFSET = glm::vec2(100,685);
constexpr float SPLICE_HEAD_DLGDESC_QUAD = 1440.f;
constexpr float SPLICE_HEAD_DLGDESC_WIDTH = 125.f;

// selection splice
constexpr glm::vec2 SPLICE_LOWER_CENTER = glm::vec2(1280.f*.5f,0);
constexpr float SPLICE_OFFCENTER_MV = .33f;

// title splice
constexpr glm::vec2 SPLICE_TITLE_LOWER_START = glm::vec2(12.5f,0);
constexpr glm::vec2 SPLICE_TITLE_UPPER_START = glm::vec2(510.f,720);
constexpr float SPLICE_TITLE_LOWER_SWIDTH = 10.f;
constexpr float SPLICE_TITLE_UPPER_SWIDTH = 100.f;
constexpr glm::vec2 SPLICE_TITLE_LOWER_MOD = SPLICE_TITLE_LOWER_START+glm::vec2(150.f,0);
constexpr glm::vec2 SPLICE_TITLE_UPPER_MOD = SPLICE_TITLE_UPPER_START-glm::vec2(485.f,0);
constexpr float SPLICE_TITLE_LWIDTH_MOD = SPLICE_TITLE_LOWER_SWIDTH+40.f;
constexpr float SPLICE_TITLE_UWIDTH_MOD = SPLICE_TITLE_UPPER_SWIDTH-85.f;

// splice colours
constexpr glm::vec3 SPLICE_TITLE_COLOUR = glm::vec3(.5f,0,0);
constexpr glm::vec3 SPLICE_HEAD_COLOUR = glm::vec3(.75f,.4125f,0);
constexpr glm::vec3 SPLICE_SELECTION_COLOUR = glm::vec3(.0985f,.270f,.037f);

// dialogue constants
constexpr glm::vec3 DIALOGUE_HEAD_COLOUR = glm::vec3(.75f,.75f,.0f);
constexpr glm::vec3 DIALOGUE_OPTION_COLOUR = glm::vec3(.75f,.75f,.0f);

// text attributes
constexpr uint8_t TEXT_INSTRUCTION_COUNT = 4;
constexpr glm::vec2 TEXT_DARE_POSITION = glm::vec2(450,250);
constexpr glm::vec4 TEXT_DARE_COLOUR = glm::vec4(1,0,0,1);
constexpr glm::vec2 TEXT_VERSION_POSITION = glm::vec2(650,20);
constexpr glm::vec4 TEXT_VERSION_COLOUR = glm::vec4(.25f,0,.75f,1);
constexpr glm::vec3 TEXT_OPTIONS_COLOUR = glm::vec3(.5f,1,.5f);
constexpr glm::vec4 TEXT_INSTRUCTION_COLOUR = glm::vec4(1,.6f,0,1);
constexpr glm::vec4 TEXT_SEGMENT_COLOUR = glm::vec4(.7f,.7f,.7f,1.f);
constexpr glm::vec4 TEXT_ERROR_COLOUR = glm::vec4(1,0,0,1);
constexpr glm::vec4 TEXT_DIALOGUE_HOVER_COLOUR = glm::vec4(0,.7f,.7f,1.f);

// menu option text translation scope
constexpr uint8_t MENU_OPTIONS_HEADINGS_COUNT = 5;
constexpr uint8_t MENU_OPTIONS_MONITOR_LISTING = 2;
constexpr float MENU_OPTIONS_TSIZE = 35.f;
constexpr float MENU_OPTIONS_HSIZE = MENU_OPTIONS_TSIZE*.5f;
constexpr glm::vec2 MENU_OPTIONS_CLEFT = glm::vec2(120,510)+glm::vec2(0,MENU_OPTIONS_HSIZE);
constexpr glm::vec2 MENU_OPTIONS_CRIGHT = glm::vec2(1240,470)+glm::vec2(0,MENU_OPTIONS_HSIZE);
constexpr glm::vec2 MENU_OPTIONS_CADDR = MENU_OPTIONS_CRIGHT-MENU_OPTIONS_CLEFT;
constexpr float MENU_OPTIONS_SCALE_THRES = 1.2f;
constexpr uint8_t MENU_OPTIONS_RDEG_THRES = 16;

// animation timing
constexpr float TRANSITION_SPEED = 11.5f;
constexpr float TITLE_SHIFTDOWN_TIMEOUT = 4.f;
constexpr float TITLE_NORMALIZATION_TIMEOUT = .5f;
constexpr float ANIMATION_UPDATE_TIMEOUT = .01f;

// animation attributes
constexpr uint8_t RATTLE_THRESHOLD = 2;
constexpr uint8_t RATTLE_THRESHOLD_RAGEADDR = 2;
constexpr float SHIFTDOWN_ZOOM_INCREASE = .075f;
constexpr double SHIFTDOWN_OCTAPI = MATH_PI/(2.0*TITLE_SHIFTDOWN_TIMEOUT);


/**
 *		Utility
*/

struct ProcessedMenuInput
{

	// processed input
	bool hit_a = false,hit_b = false;
	int8_t dir_horz = 0,dir_vert = 0;
	glm::vec2 mouse_cartesian = glm::vec2(0);
	Mouse* mouse;

	// preferred peripherals
	bool* mouse_preferred_peripheral,controller_preferred_peripheral = false;
};
// TODO: make those feature a base functionality of input mapping.
//	triggered & cartesian mouse clicks are BASIC FUNCTIONALITY!
//	also triggered directional input is a MUST HAVE! this will NOT be the last time we need user input!


/**
 *		LDC Compiler Definition
*/

// mapped enumeration of all possible LDC language commands
enum LDCCommandID
{

	// definitions
	CLUSTER,ENTITY,DESCRIPTION,

	// attributes
	FLOATS,STRINGS,SEGMENT,CONDITION,LINK,

	// behaviours
	CHILD,SYSBEHAVIOUR,CMD_CHECKBOX,CMD_DROPDOWN,CMD_SLIDER,CMD_RETURN,

	// fault
	SYNTAX_ERROR,
	COMMAND_COUNT = SYNTAX_ERROR
};

// enumeration of possible list entity states as defined by LDC language
enum LDCEntityType
{
	UNDEFINED,
	CHECKBOX,
	DROPDOWN,
	SLIDER,
	RETURN,
	SUBSEQUENT,
	SYSTEM
};

// extracted command data from .ldc file
struct ListLanguageCommand
{
	uint8_t id = 0;
	std::string tail,buffer;
	uint16_t line_number;
};

// sublist references, linking child id per parent
struct LDCSubsequentReferences
{
	std::vector<uint16_t> parent_ids;
	std::vector<std::string> child_names;
};

// list entity data
struct LDCEntity
{
	// basic element data & availability state
	std::string head,description;
	uint16_t serialization_id = 0;
	std::vector<uint8_t> condition_id;

	// element behaviour type, value of tdata switches meaning based on etype
	LDCEntityType etype = LDCEntityType::UNDEFINED;
	uint16_t tdata = 0;

	// storage for additional attributes
	std::vector<float> fattribs;
	std::vector<std::string> cattribs;

	// segment behaviour
	bool jsegment = false;
};

// list segmentation data
struct LDCSegment
{
	uint16_t position;
	std::string title;
};

// parent structure for list content, forming the list itself
struct LDCCluster
{
	// info
	std::string id = "";

	// list content
	std::vector<LDCEntity> elist;
	std::vector<LDCSegment> slist;

	// precalculation
	std::vector<uint16_t> linked_ids;
	uint16_t cnt_checkbox = 0,cnt_dropdown = 0,cnt_slider = 0;
};

// compiler data
struct LDCProcessState
{
	// info
	const char* fpath;

	// processing
	ListLanguageCommand* cmd;
	bool jnext = false;

	// working data
	std::vector<LDCSubsequentReferences> refs_children;
	std::vector<std::vector<std::string>> refs_links;

	// resulting data
	std::vector<LDCCluster>& clusters;
};

// static compiler component
class LDCCompiler
{
public:

	// code processing
	static void compile(const char* path,std::vector<LDCCluster>& rClusters);
	static inline void compiler_error_msg(LDCProcessState& state,const char* msg)
		{ printf("\033[1;31mldc compiler error\033[0m in %s:%i %s\n",state.fpath,state.cmd->line_number,msg); }
};


/**
 *		Selection Splice Geometry Definition
*/

// vertex pattern for GPU upload
struct SpliceVertexGeometry
{
	glm::vec3 colour;
	uint32_t edge_id;
};

// animation target key for splice transition
struct SelectionSpliceKey
{
	glm::vec2 disp_lower,disp_upper;
	float ext_lower,ext_upper;
};

// splice data
struct SelectionSplice
{
	// uniform & animation keys
	SelectionSpliceKey current;
	std::vector<SelectionSpliceKey> ssk;

	// data
	float* transition_ref;
	bool horizontal;
};

// collective selection splice component
class SelectionSpliceGeometry
{
public:

	// construction
	SelectionSpliceGeometry() {  }
	~SelectionSpliceGeometry() {  }

	// creation
	uint8_t create_splice(glm::vec2 l,glm::vec2 u,float lw,float uw,glm::vec3 c,bool hrz,float* tref);
	void load();

	// modification
	uint8_t add_anim_key(uint8_t id,glm::vec2 ld,glm::vec2 ud,float le,float ue);

	// draw
	void update();

public:

	// data
	std::vector<SelectionSplice> splices;

private:

	// engine
	Buffer buffer = Buffer();
	Shader shader = Shader();

	// data
	std::vector<SpliceVertexGeometry> verts;
};


/**
 * 		MenuList Definiton
*/

// data for each list entity
struct MenuListEntity
{
	// function
	uint16_t grid_position = 0;
	glm::vec4 colour = glm::vec4(1.f);					// TODO: obsoletion through text feature
	LDCEntityType etype = LDCEntityType::UNDEFINED;
	uint16_t value = 0;
	uint16_t link_id = 0;

	// visuals
	float anim_transition = .0f;
	uint8_t diff_preview = 0;
	bool has_rotation = false;
	glm::vec2 grotation = glm::vec2(0);

	// text
	Text text;
	size_t tlen = 0;									// TODO: obsoletion through text feature

	// attribute component
	std::vector<Text> dd_options;
	std::vector<glm::vec4> dd_colours;
	std::vector<size_t> dd_length;
};

// data for visual list segmentation
struct MenuListSegment
{
	uint16_t position = 0;
	Text text;
	size_t tlen = 0;
};

// list structure
struct MenuListComplex
{
	// navigation
	uint16_t lscroll = 0,select_id = 0;

	// data
	std::vector<MenuListEntity> entities;
	std::vector<MenuListSegment> segments;

	// text
	Text description;
	size_t dtlen = 0;

	// index
	std::vector<uint16_t> checkbox_ids,dropdown_ids,slider_ids,link_ids;
};

// collective menu list component
class MenuList
{
public:

	// construction
	MenuList() {  }
	~MenuList() {  }

	// invokation
	uint8_t define_list(const char* path);
	uint8_t define_list(SaveStates states);
	void load(CascabelBaseFeature* ccbf);

	// interaction
	void open_list(uint8_t id);
	void close_list(uint8_t id);
	inline void discolour(uint8_t cid,uint16_t eid,glm::vec4 col) { mlists[cid].entities[eid].colour = col; }
	// TODO: obsoletion through text feature

	// serialization
	void write_attributes(uint8_t id);
	void reset_attributes(uint8_t id);
	bool linked_variables_changed(uint16_t list_id,bool& reload);

	// draw
	int8_t update(ProcessedMenuInput& input,InputMap* iMap,bool& rrnd);
	void render();
	void update_background_component();
	void update_overlays();

private:

	// geometry creation
	void create_checkbox(float vscroll);
	void create_slider(float vscroll);

	// calculation
	uint8_t calculate_grid_position();

public:

	// interaction
	bool system_active = false;
	std::vector<bool> conditions;
	uint16_t status = 0;
	uint8_t instruction_mod = 0;

	// data
	std::vector<MenuListComplex> mlists;

private:

	// engine
	CascabelBaseFeature* m_ccbf;
	Buffer checkbox_buffer = Buffer(),ddbgr_buffer = Buffer(),slider_buffer = Buffer();
	Shader checkbox_shader = Shader(),ddbgr_shader = Shader(),slider_shader = Shader();

	// data
	uint16_t rid_diffs;
	std::vector<uint8_t> active_ids;
	std::vector<float> checkbox_vertices,slider_vertices;

	// positioning
	float crr_scroll = .0f;
	float anim_prog = .0f;

	// globe scene
	uint16_t rid_globe;
	uint8_t globe_target_id;
	FrameBuffer fb_globe;
	Camera3D gb_cam3D = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.f,720.f,45.f,15.f,-110.f);
	Lighting gb_lights = Lighting();
	bool show_globe = false;
	glm::vec2 globe_rotation = glm::vec2(0);

	// states
	bool tf_list_opened = false, subfunc_opened = false;

	// visuals
	Font de_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25);
	Font st_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_LIST_HEAD_SIZE,MENU_LIST_HEAD_SIZE);
	glm::vec4 diff_colours[4]
		= { glm::vec4(1),glm::vec4(1,.5f,0,1),glm::vec4(.75f,0,0,1),glm::vec4(.5f,0,1,1) };
};


/**
 * 		MenuDialogue Definition
*/

// vertex structure for dialogue background
struct DialogueBackgroundGeometry
{
	glm::vec2 position;
	uint32_t disp_id;		// TODO: change to 8-bit unsigned
};

// tuple pairing confirmation behaviour and value of dialogue entity
struct DialogueActionTuple
{
	LDCEntityType type;
	uint16_t value;
};

// all required data for a menu dialogue
struct SingularDialogueData
{
	// list height, approximate dialogue dimensions and vertex expansion targets
	float liststart_y;
	float max_width,max_height;
	float dim_width = .0f,dim_height = .0f;

	// progression of transition .0f = fully closed (no render) 1.f = fully active (input ready)
	float dgtrans = .0f;

	// selection
	int8_t sindex = 0;
	uint8_t max_options;

	// entity values
	std::vector<DialogueActionTuple> action;

	// text to display information to user
	uint8_t option_size;
	Text tx_title,tx_options,tx_descriptions;
	size_t title_length,option_length = 0,description_length = 0;
};

// collective menu dialogue component
class MenuDialogue
{
public:

	// construction
	MenuDialogue() {  }
	~MenuDialogue() {  }

	// creation
	uint8_t add_dialogue_window(const char* path,glm::vec2 center,float width,float height,
			uint8_t tsize,uint8_t dsize);
	void load();

	// interaction
	void open_dialogue(uint8_t did);
	void close_dialogue(uint8_t did);

	// draw
	void update(ProcessedMenuInput& input);
	void render();
	void update_background_component();

private:

	// render
	void draw_dialogue(uint8_t id);

public:

	// status
	bool system_active = false;
	uint8_t status = 0;

private:

	// engine
	Buffer bgr_buffer = Buffer(),slc_buffer = Buffer();
	Shader bgr_shader = Shader(),slc_shader = Shader();

	// data
	std::vector<SingularDialogueData> dg_data;
	std::vector<DialogueBackgroundGeometry> bgr_verts;
	std::vector<glm::vec2> slc_verts;

	// id states
	std::vector<uint8_t> opening_ids,active_ids,closing_ids;
};


/**
 * 		MainMenu Definition
*/

// enumeration of interface behaviours changed by main horizontal option list
enum OptionLogicID
{
	MACRO_EXIT,
	OPTIONS,
	EXTRAS,
	PRACTICE,
	LOAD,
	CONTINUE,
	NEWGAME,
	LOGIC_COUNT,
	OPTION_CAP = LOGIC_COUNT-1
};

// main menu component
class MainMenu : public UI
{
public:

	// construction
	MainMenu() {  }
	MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float& progress,float pseq);
	~MainMenu() {  }

	// draw
	virtual void render(FrameBuffer* game_fb,bool& running,bool& reboot);

private:

	// system
	void update_peripheral_annotations();

public:

	// engine
	CascabelBaseFeature* m_ccbf;
	SaveStates savestates;
	bool request_close = false,request_restart = false;
	ProcessedMenuInput input;

	// interactables
	SelectionSpliceGeometry splices_geometry = SelectionSpliceGeometry();
	MenuList mlists;
	MenuDialogue mdialogues;

	// index
	uint8_t interface_logic_id = OptionLogicID::MACRO_EXIT;
	uint8_t vselect = OptionLogicID::LOGIC_COUNT-2;
	uint8_t splice_head_id,splice_selection_id,head_mod_id;
	uint8_t ml_options,ml_extras,ml_stages,ml_saves;
	uint8_t dg_diffs,dg_continue,dg_optsave;

	// general animation
	bool menu_action = false;
	float mtransition = .0f,inv_mtransition;
	float ftransition = .0f,inv_ftransition;
	float lext_selection = .0f,uext_selection = .0f;

	// macro options menu
	glm::vec2 mo_prog = MENU_OPTIONS_CADDR;
	glm::vec2 mo_cposition[OptionLogicID::LOGIC_COUNT];
	float mo_twidth[OptionLogicID::LOGIC_COUNT],mo_hwidth[OptionLogicID::LOGIC_COUNT];
	float st_rot = .0f;

	// memory for static continue
	bool queued_restart = false;
	uint8_t logic_setup = 0;

	// globals
	inline static float transition_delta = .0f;

private:

	// engine
	CCBManager* m_ccbm;
	World* m_world;
	FrameBuffer fb_menu,fb_nslice,fb_slice;
	MSAA msaa;

	// text
	Font fnt_mopts = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_OPTIONS_TSIZE,MENU_OPTIONS_TSIZE);
	Font fnt_reqt = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25);
	Text tx_dare = Text(fnt_reqt),tx_instr = Text(fnt_reqt),
		tx_version = Text(Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",15,15));
	std::vector<Text> tx_mopts = std::vector<Text>(OptionLogicID::LOGIC_COUNT,Text(fnt_mopts));
	uint8_t tcap_dare,tcap_instr = 0,tcap_version;

	// index
	uint16_t index_ranim;

	// animation
	float anim_timing = .0f;
	float dt_tshiftdown = .0f,dt_tnormalize = .0f;
	bool speedup = true;
	float mlist_tsidebar = .0f;

	// remote
	float tkey_head = .0f,tkey_selection = .0f,tkey_title = .0f;

	// triggers
	bool trg_lmb = false,trg_rmb = false;
};

#endif
