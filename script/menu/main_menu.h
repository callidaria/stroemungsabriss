#ifndef SRC_MENU_MAIN
#define SRC_MENU_MAIN

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

#define COLOUR_COMPOSITION_NEO

// information
constexpr uint8_t INFO_VERSION_RELEASE = 0;
constexpr uint8_t INFO_VERSION_SUBRELEASE = 0;
constexpr uint8_t INFO_VERSION_DEVSTEP = 6;
constexpr char INFO_VERSION_MODE_SUFFIX = 'c';
// MODI: c = "development", t = "QA build", p = "polishing", R = "release"

// menu options index constants
constexpr uint8_t MENU_MAIN_OPTION_EXIT = 0;
constexpr uint8_t MENU_MAIN_OPTION_OPTIONS = 1;
constexpr uint8_t MENU_MAIN_OPTION_EXTRAS = 2;
constexpr uint8_t MENU_MAIN_OPTION_PRACTICE = 3;
constexpr uint8_t MENU_MAIN_OPTION_LOAD = 4;
constexpr uint8_t MENU_MAIN_OPTION_CONTINUE = 5;
constexpr uint8_t MENU_MAIN_OPTION_NEWGAME = 6;
constexpr uint8_t MENU_MAIN_OPTION_COUNT = 7;

// function pointer index constants
constexpr uint8_t INTERFACE_LOGIC_MACRO = 0;
constexpr uint8_t INTERFACE_LOGIC_OPTIONS = 1;
constexpr uint8_t INTERFACE_LOGIC_EXTRAS = 2;
constexpr uint8_t INTERFACE_LOGIC_PRACTICE = 3;
constexpr uint8_t INTERFACE_LOGIC_LOAD = 4;
constexpr uint8_t INTERFACE_LOGIC_CONTINUE = 5;
constexpr uint8_t INTERFACE_LOGIC_NEWGAME = 6;
constexpr uint8_t INTERFACE_LOGIC_COUNT = 7;

// system constants
constexpr uint8_t MENU_GBUFFER_COLOUR = 0;
constexpr uint8_t MENU_GBUFFER_NORMALS = 1;
constexpr uint8_t MENU_MAIN_OPTION_CAP = MENU_MAIN_OPTION_COUNT-1;
constexpr uint8_t SPLICE_VERTEX_FLOAT_COUNT = 6;
constexpr uint8_t DIALOGUEBGR_VERTEX_FLOAT_COUNT = 3;
constexpr uint8_t LIST_LANGUAGE_COMMAND_COUNT = 13;

// menu list positioning
constexpr uint8_t MENU_LIST_GRID_RANGE = 7;
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
#ifdef COLOUR_COMPOSITION_NEO
constexpr glm::vec3 SPLICE_HEAD_COLOUR = glm::vec3(.75f,.4125f,0);
constexpr glm::vec3 SPLICE_SELECTION_COLOUR = glm::vec3(.0985f,.270f,.037f);
#else
constexpr glm::vec3 SPLICE_HEAD_COLOUR = glm::vec3(.5f,.5f,.0f);
constexpr glm::vec3 SPLICE_SELECTION_COLOUR = glm::vec3(.0f,.5f,.5f);
#endif
// TODO: figure out colour scheme and if gamma correction in splice shader will be upheld

// dialogue constants
constexpr glm::vec3 DIALOGUE_HEAD_COLOUR = glm::vec3(.75f,.75f,.0f);
constexpr glm::vec3 DIALOGUE_OPTION_COLOUR = glm::vec3(.75f,.75f,.0f);

// text attributes
constexpr glm::vec2 TEXT_DARE_POSITION = glm::vec2(450,250);
constexpr glm::vec4 TEXT_DARE_COLOUR = glm::vec4(1,0,0,1);
constexpr glm::vec2 TEXT_VERSION_POSITION = glm::vec2(650,20);
constexpr glm::vec4 TEXT_VERSION_COLOUR = glm::vec4(.25f,0,.75f,1);
constexpr glm::vec4 TEXT_SEGMENT_COLOUR = glm::vec4(.7f,.7f,.7f,1.f);

// menu option text translation scope
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

// math constants
constexpr double MATH_PI = 3.141592653;
constexpr double MATH_OCTAPI = MATH_PI/(2.0*TITLE_SHIFTDOWN_TIMEOUT);


/**
 *		LDC Compiler Definition
*/

enum LDCEntityType
{
	UNDEFINED = 0,
	CHECKBOX = 1,
	DROPDOWN = 2,
	SLIDER = 3,
	RETURN = 4,
	SUBSEQUENT = 5,
	SYSTEM = 6
};

struct ListLanguageCommand
{
	uint8_t id = 0;
	std::string tail,buffer;
	uint16_t line_number;
};

struct LDCSubsequentReferences
{
	std::vector<uint16_t> parent_ids;
	std::vector<std::string> child_names;
};

struct LDCEntity
{
	// basic element data & availability state
	std::string head,description;
	uint16_t serialization_id = 0;
	std::vector<uint8_t> condition_id;
	uint16_t vlink = 0;

	// element behaviour type, value of tdata switches meaning based on etype
	LDCEntityType etype = LDCEntityType::UNDEFINED;
	uint16_t tdata = 0;

	// storage for additional attributes
	std::vector<float> fattribs;
	std::vector<std::string> cattribs;

	// segment behaviour (probably not the best solution and is subject to change)
	bool jsegment = false;
};

struct LDCSegment
{
	uint16_t position;
	std::string title;
};

struct LDCCluster
{
	std::string id = "";
	std::vector<LDCEntity> elist;
	std::vector<LDCSegment> slist;
	std::vector<uint16_t> linked_ids;
	uint16_t cnt_checkbox = 0,cnt_dropdown = 0,cnt_slider = 0;
};

struct LDCProcessState
{
	// info
	const char* fpath;

	// working data
	ListLanguageCommand* cmd;
	std::vector<LDCSubsequentReferences> crefs;
	std::vector<std::vector<std::string>> srefs;

	// resulting data
	std::vector<LDCCluster> clusters;
};

class LDCCompiler
{
public:

	// code processing
	static std::vector<LDCCluster> compile(const char* path);
};

// command interpretation logic
typedef void (*interpreter_logic)(LDCProcessState&);
static void command_logic_cluster(LDCProcessState &state);
static void command_logic_define(LDCProcessState &state);
static void command_logic_describe(LDCProcessState &state);
static void command_logic_attributes(LDCProcessState &state);
static void command_logic_segment(LDCProcessState &state);
static void command_logic_condition(LDCProcessState &state);
static void command_logic_link(LDCProcessState &state);
static void command_logic_subsequent(LDCProcessState &state);
static void command_logic_sysbehaviour(LDCProcessState &state);
static void command_logic_checkbox(LDCProcessState &state);
static void command_logic_dropdown(LDCProcessState &state);
static void command_logic_slider(LDCProcessState &state);
static void command_logic_return(LDCProcessState &state);
static void command_logic_syntax_error(LDCProcessState &state);
static void compiler_error_msg(LDCProcessState &state,const char* msg);


/**
 *		Selection Splice Geometry Definition
*/

struct SpliceVertexGeometry
{
	glm::vec3 colour;
	uint32_t edge_id;
};

struct SelectionSpliceKey
{
	glm::vec2 disp_lower,disp_upper;
	float ext_lower,ext_upper;
};

struct SelectionSplice
{
	// current uniform variables
	SelectionSpliceKey current;
	bool horizontal;

	// target data
	std::vector<SelectionSpliceKey> ssk;
	float* transition_ref;
};

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

struct MenuListEntity
{
	// function
	glm::vec4 colour;
	LDCEntityType etype;
	uint16_t value;
	uint16_t link_id;
	float anim_transition;

	// representation
	Text text;
	size_t tlen;

	// attribute component
	std::vector<Text> dd_options;
	std::vector<glm::vec4> dd_colours;
	std::vector<size_t> dd_length;
};

struct MenuListSegment
{
	uint16_t position;
	Text text;
	size_t tlen;
};

struct MenuListComplex
{
	// navigation
	uint16_t lselect,lscroll;
	size_t full_range;

	// data
	std::vector<MenuListEntity> entities;
	std::vector<MenuListSegment> segments;

	// text
	Text description;
	size_t dtlen;

	// index
	std::vector<uint16_t> checkbox_ids,dropdown_ids,slider_ids,link_ids;
};

class MenuList
{
public:

	// construction
	MenuList() {  }
	~MenuList() {  }

	// invokation
	uint8_t define_list(const char* path);
	uint8_t define_list(SaveStates states);
	void load();

	// interaction
	void open_list(uint8_t id);
	void close_list(uint8_t id);
	inline void discolour(uint8_t cid,uint16_t eid,glm::vec4 col) { mlists[cid].entities[eid].colour = col; }

	// draw
	uint8_t update(int8_t vdir,int8_t hdir,glm::vec2 mpos,int8_t mscroll,bool conf,bool ntconf,bool back,
			bool mperiph,bool &rrnd);
	void update_background_component(float anim_delta);

	// info
	inline bool system_active() { return active_ids.size(); }
	bool linked_variables_changed(uint16_t list_id);

private:

	// geometry creation
	void create_checkbox(float vscroll);
	void create_slider(float vscroll);

public:

	// interaction
	bool lists_open = false;
	std::vector<bool> conditions;
	uint16_t status = 0;

	// data
	std::vector<MenuListComplex> mlists;

private:

	// engine
	Buffer checkbox_buffer = Buffer(),ddbgr_buffer = Buffer(),slider_buffer = Buffer();
	Shader checkbox_shader = Shader(),ddbgr_shader = Shader(),slider_shader = Shader();

	// data
	std::vector<uint8_t> active_ids;
	std::vector<float> checkbox_vertices,slider_vertices;

	// positioning
	uint16_t crr_select = 0,seg_passed = 0;
	float crr_scroll = .0f;

	// triggers
	bool tf_list_opened = false,subfunc_opened = false;
	float rtt = .0f;

	// visuals
	Font de_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25);
	Font st_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_LIST_HEAD_SIZE,MENU_LIST_HEAD_SIZE);
	glm::vec4 diff_colours[4]
		= { glm::vec4(1),glm::vec4(1,.5f,0,1),glm::vec4(.75f,0,0,1),glm::vec4(.5f,0,1,1) };
};


/**
 * 		MenuDialogue Definition:
 * 
 * DialogueBackgroundGeometry
 * 	-> background vertex geometry information pattern
 * 
 * SingularDialogueData
 * 	-> information for each popup dialogue
 * 
 * MenuDialogue
 * 	-> main handler of popup dialogue system, creates dialogues and automates them
 * 
 * 
 * 		TODO QA
 * - trying to break opening/closing routine
 * - assessing animations, style, timing & randomizer ranges visually
 * - are background dimension manipulation ranges ok or should they be tweaked
*/

struct DialogueBackgroundGeometry
{
	glm::vec2 position;
	uint32_t disp_id;		// TODO: change to 8-bit unsigned
};

struct SingularDialogueData
{
	// interaction state of the dialogue
	bool dg_active = false;

	// list height, approximate dialogue dimensions and vertex expansion targets
	float liststart_y;
	float max_width,max_height;
	float dim_width = .0f,dim_height = .0f;

	// progression of transition .0f = fully closed (no render) 1.f = fully active (input ready)
	float dgtrans = .0f;

	// index of selected option
	int8_t sindex = 0,max_options;

	// action tuple
	std::vector<uint8_t> action_id;
	std::vector<uint16_t> action_value;

	// text to display information to user
	uint8_t option_size;
	Text tx_title,tx_options,tx_descriptions;
	uint32_t title_length,option_length = 0,description_length = 0;
};

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
	void update(int8_t imv,float mypos,bool mperiph,bool conf,bool back);
	void background_component(float transition_delta);

private:

	// render
	void draw_dialogue(uint8_t id);

public:

	// status
	bool system_active = false;
	uint8_t dg_state = 0;
	std::vector<SingularDialogueData> dg_data;

private:

	// engine
	Buffer bgr_buffer = Buffer(),slc_buffer = Buffer();
	Shader bgr_shader = Shader(),slc_shader = Shader();

	// data
	std::vector<DialogueBackgroundGeometry> bgr_verts;
	std::vector<glm::vec2> slc_verts;

	// id states
	std::vector<uint8_t> opening_ids,active_ids,closing_ids;
};


/**
 * 		MainMenu Definition
 * 
 * 		TODO QA
 * 
 * - TRANSITION_SPEED modifier too fast/too slow?
 * - input satisfaction feedback
 * - aliasing recognition threshold & which technique preferred
*/

class MainMenu : public UI
{
public:

	// construction
	MainMenu() {  }
	MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float &progress,float pseq);
	~MainMenu() {  }

	// draw
	virtual void render(FrameBuffer* game_fb,bool &running,bool &reboot);

private:

	// system
	void update_peripheral_annotations();

public:

	// engine
	CascabelBaseFeature* m_ccbf;
	bool request_close = false;

	// interactables
	SelectionSpliceGeometry splices_geometry = SelectionSpliceGeometry();
	MenuList mlists;
	MenuDialogue mdialogues;

	// index
	uint8_t interface_logic_id = INTERFACE_LOGIC_MACRO;
	uint8_t vselect = MENU_MAIN_OPTION_COUNT-2,hselect = 0;
	int8_t vgrid_id = 0;
	uint8_t splice_head_id,splice_selection_id,head_mod_id;

	// processed input
	bool hit_a,hit_b;
	int8_t lrmv,udmv;
	glm::vec2 crd_mouse;

	// animation
	float transition_delta;
	bool menu_action = false;
	float mtransition = .0f,inv_mtransition;
	float ftransition = .0f,inv_ftransition;
	float lext_selection = .0f,uext_selection = .0f;

	// text
	glm::vec2 mo_prog = MENU_OPTIONS_CADDR;
	glm::vec2 mo_cposition[MENU_MAIN_OPTION_COUNT];
	float mo_twidth[MENU_MAIN_OPTION_COUNT],mo_hwidth[MENU_MAIN_OPTION_COUNT];
	float st_rot = .0f;

	// memory for static continue
	uint8_t ml_options,ml_extras,ml_stages,ml_saves,dg_diffs,dg_continue,dg_optsave;
	bool logic_setup = false;

private:

	// engine
	CCBManager* m_ccbm;
	World* m_world;
	FrameBuffer fb_menu,fb_nslice,fb_slice;
	Font fnt_mopts = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_OPTIONS_TSIZE,MENU_OPTIONS_TSIZE);
	Text tx_dare = Text(Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",25,25)),
		tx_version = Text(Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",15,15));
	std::vector<Text> tx_mopts = std::vector<Text>(MENU_MAIN_OPTION_COUNT,Text(fnt_mopts));
	MSAA msaa;

	// data
	SaveStates savestates;

	// index
	uint16_t index_rsprite,index_ranim;
	uint8_t tcap_dare,tcap_version;

	// input
	bool cpref_peripheral;
	bool trg_lmb = false,trg_rmb = false;

	// animation
	float anim_timing = .0f;
	float dt_tshiftdown = .0f,dt_tnormalize = .0f;
	bool speedup = true;
	float mlist_tsidebar = .0f;

	// remote
	float tkey_head = .0f,tkey_selection = .0f,tkey_title = .0f;

	// predefinitions
	typedef void (*interface_logic)(MainMenu&);
	interface_logic interface_behaviour[INTERFACE_LOGIC_COUNT];
	const char* main_options[MENU_MAIN_OPTION_COUNT]
		= { "exit","options","extras","practice","load","continue","new game" };
};

// predefinition interface behaviour logic
static void interface_behaviour_macro(MainMenu &tm);
static void interface_behaviour_options(MainMenu &tm);
static void interface_behaviour_extras(MainMenu &tm);
static void interface_behaviour_practice(MainMenu &tm);
static void interface_behaviour_load(MainMenu &tm);
static void interface_behaviour_continue(MainMenu &tm);
static void interface_behaviour_newgame(MainMenu &tm);

#endif
