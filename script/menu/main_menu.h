#ifndef SRC_MENU_MAIN
#define SRC_MENU_MAIN

#include "../../ccb/fcn/ccb_manager.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

#include "../../ccb/frm/framebuffer.h"
#include "../../ccb/ppe/msaa.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"
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
constexpr float MENU_HALFSCREEN_UI = 1280.f*.5f;
constexpr uint8_t LIST_LANGUAGE_COMMAND_COUNT = 11;

// menu list positioning
constexpr uint16_t MENU_LIST_HEADPOS_X = 250;
constexpr uint16_t MENU_LIST_SCROLL_START = 515;
constexpr uint16_t MENU_LIST_SCROLL_Y = 45;
constexpr uint16_t MENU_LIST_HEAD_SIZE = 30;
constexpr uint16_t MENU_LIST_SEGMENT_PUSH_X = 100;
constexpr uint8_t MENU_LIST_GRID_RANGE = 7;

// menu dialogue positioning
constexpr uint8_t MENU_DIALOGUE_TITLE_SIZE = 30;
constexpr uint8_t MENU_DIALOGUE_OPTION_SIZE = 25;
constexpr float MENU_DIALOGUE_OFFSET_FACTOR = .9f;

// list entity types
constexpr uint8_t LIST_ENTITY_TYPE_CHECKBOX = 1;
constexpr uint8_t LIST_ENTITY_TYPE_DROPDOWN = 2;
constexpr uint8_t LIST_ENTITY_TYPE_SLIDER = 3;
constexpr uint8_t LIST_ENTITY_TYPE_RETURN = 4;

// title position & transition destination
constexpr glm::vec3 VRT_TITLE_START = glm::vec3(300,300,0);
constexpr glm::vec3 VRT_TITLE_END = glm::vec3(100,250,0);
constexpr glm::vec3 HRZ_TITLE_START = glm::vec3(590,800,0);
constexpr glm::vec3 HRZ_TITLE_END = glm::vec3(590,642,0);
constexpr glm::vec3 VRT_TITLE_TRANSITION = VRT_TITLE_END-VRT_TITLE_START;
constexpr glm::vec3 HRZ_TITLE_TRANSITION = HRZ_TITLE_END-HRZ_TITLE_START;
constexpr glm::vec3 VRT_TITLE_SCALESET = VRT_TITLE_END-glm::vec3(640,360,0);
constexpr glm::vec3 HRZ_TITLE_SCALESET = HRZ_TITLE_END-glm::vec3(640,360,0);

// title splice
constexpr float SPLICE_TITLE_LOWER_START = 12.5f;
constexpr float SPLICE_TITLE_UPPER_START = 510.f;
constexpr float SPLICE_TITLE_LOWER_SWIDTH = 10.f;
constexpr float SPLICE_TITLE_UPPER_SWIDTH = 100.f;
constexpr float SPLICE_TITLE_LOWER_MOD = 150.f;
constexpr float SPLICE_TITLE_UPPER_MOD = -485.f;
constexpr float SPLICE_TITLE_LWIDTH_MOD = 40.f;
constexpr float SPLICE_TITLE_UWIDTH_MOD = -85.f;

// head splice
constexpr float SPLICE_HEAD_ORIGIN_POSITION = MENU_LIST_SCROLL_START-MENU_LIST_HEAD_SIZE*.5f;
constexpr float SPLICE_HEAD_MINIMUM_WIDTH = 4.f;
constexpr float SPLICE_HEAD_ORIGIN_WIDTH = 15.f-SPLICE_HEAD_MINIMUM_WIDTH;
constexpr float SPLICE_HEAD_LOWER_START = 520.f-SPLICE_HEAD_ORIGIN_POSITION;
constexpr float SPLICE_HEAD_UPPER_START = 470.f-SPLICE_HEAD_ORIGIN_POSITION;
constexpr float SPLICE_HEAD_LOWER_WIDTH = 42.f-SPLICE_HEAD_ORIGIN_WIDTH+SPLICE_HEAD_MINIMUM_WIDTH;
constexpr float SPLICE_HEAD_UPPER_WIDTH = 50.f-SPLICE_HEAD_ORIGIN_WIDTH+SPLICE_HEAD_MINIMUM_WIDTH;
constexpr uint8_t SPLICE_HEAD_TILT_THRESHOLD = 10;
constexpr uint8_t SPLICE_HEAD_TILT_DBTHRESHOLD = SPLICE_HEAD_TILT_THRESHOLD*2;

// selection splice
constexpr float SPLICE_OFFCENTER_MV = .33f;

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

// text attributes
constexpr glm::vec2 TEXT_DARE_POSITION = glm::vec2(450,250);
constexpr glm::vec4 TEXT_DARE_COLOUR = glm::vec4(1,0,0,1);
constexpr glm::vec2 TEXT_VERSION_POSITION = glm::vec2(650,20);
constexpr glm::vec4 TEXT_VERSION_COLOUR = glm::vec4(.25f,0,.75f,1);

// menu option text translation scope
constexpr float MENU_OPTIONS_TSIZE = 35.f;
constexpr float MENU_OPTIONS_HSIZE = MENU_OPTIONS_TSIZE/2.f;
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
 * 		MenuList Definiton Documentation
 * TODO: expand
*/

struct ListLanguageCommand
{
	uint8_t id = 0;
	std::string tail,buffer;
};

struct MenuListEntity
{
	std::string head,description,child_name;
	uint8_t child_id,condition_id = 0;
	uint8_t etype;
	std::vector<std::string> dropdown_options;
	uint16_t rval;
	bool jsegment = false;
};
// TODO: add multicondition support

struct MenuListSegment
{
	uint16_t position;
	std::string title;
};

struct MenuListCluster
{
	std::string id;
	std::vector<MenuListEntity> elist;
	std::vector<MenuListSegment> slist;

	// previously saved MenuListEntity* list to iterate parents, but memory issues made it break
	// TODO: look into the original memory issue for the slightest of improvements
	std::vector<uint16_t> parents;

	// TODO: this awful, horrible, trash, garbage, bullshit text implementation does four things:
	//		1) creation is complete and utter garbage trash
	//		2) you instanciate a shader & buffer for each and every single element
	//		3) every text element => list element has it's own load
	//		4) EVERY GODDAMN TEXT ELEMENT IS A SINGLE GODDAMN DRAWCALL!!!??!?!??!?!?!?
	// HOW COULD I HAVE WORKED WITH THIS UTTERLY STUPID TEXT REPRESENTATION FOR SO LONG!?!?!
	// handling text this way uses !!20MB!! yes right, !!20MB!! for the options list alone
	// anyway, here's the implementation for now...
	std::vector<Text> tx_elist,tx_slist;
};

class MenuList
{
public:

	// construction
	MenuList(const char* path);
	~MenuList() {  }

	// draw
	void update(int8_t &grid,bool conf,bool &back);

public:

	// data
	const char* fpath;
	uint32_t cline = 1;

	// listing
	std::vector<MenuListCluster> clusters;

	// interaction
	uint8_t active_cluster_id = 0;
	std::vector<bool> condition_list;

private:

	// buffer
	std::vector<ListLanguageCommand> cmd_buffer;

	// status
	uint8_t lscroll = 0;

	// predefinitions
	Font st_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_LIST_HEAD_SIZE,MENU_LIST_HEAD_SIZE);
	std::string mlcmd[LIST_LANGUAGE_COMMAND_COUNT] = {
		"cluster","logic","define","describe","segment","condition",
		"subsequent","checkbox","dropdown","slider","return"
	};
};

// command interpretation logic
typedef void (*interpreter_logic)(MenuList&,const ListLanguageCommand&);
static void command_logic_cluster(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_logiclist(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_define(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_describe(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_segment(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_condition(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_subsequent(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_checkbox(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_dropdown(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_slider(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_return(MenuList &ml,const ListLanguageCommand &cmd);
static void command_logic_syntax_error(MenuList &ml,const ListLanguageCommand &cmd);


/**
 *		MenuDialogue Definition Documentation
 * TODO: expand
*/

struct SingularDialogueData
{
	bool dg_active = false;
	float max_width,max_height;
	float dim_width = .0f,dim_height = .0f;
	float dgtrans = .0f;
	Text tx_title,tx_options;
};

class MenuDialogue
{
public:

	// construction
	MenuDialogue() {  }
	~MenuDialogue() {  }

	// creation
	uint8_t add_dialogue_window(const char* title,std::vector<const char*> options,
			glm::vec2 center,float width,float height);
	void load();

	// interaction
	void open_dialogue(uint8_t did);
	void close_dialogue(uint8_t did);

	// draw
	void update(float transition_delta);
	void selection_component(int8_t &grid,bool conf,bool back);

private:

	// render
	void draw_dialogue(uint8_t id);

public:

	// data
	std::vector<SingularDialogueData> dg_data;

private:

	// engine
	Buffer bgr_buffer = Buffer(),slc_buffer = Buffer();
	Shader bgr_shader = Shader(),slc_shader = Shader();
	Font title_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_DIALOGUE_TITLE_SIZE,MENU_DIALOGUE_TITLE_SIZE);
	Font option_font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",
			MENU_DIALOGUE_OPTION_SIZE,MENU_DIALOGUE_OPTION_SIZE);

	// data
	std::vector<float> bgr_verts,slc_verts;

	// id states
	std::vector<uint8_t> opening_ids,active_ids,closing_ids;
};


/**
 * 		MainMenu Definition Documentation
 * TODO: expand
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

	// logic
	void update_list_grid(MenuList &ml);

private:

	// system
	void update_peripheral_annotations();

	// splashes
	void create_splash(std::vector<float> &sverts,glm::vec2 l,glm::vec2 u,glm::vec3 c);
	void modify_splash(glm::vec2 lp,glm::vec2 up,float le,float ue,bool hrz);

public:

	// engine
	CascabelBaseFeature* m_ccbf;
	bool request_close = false;

	// interactables
	MenuList ml_options = MenuList("./lvload/options.ldc");
	MenuDialogue mdialogues;

	// index
	uint8_t interface_logic_id = INTERFACE_LOGIC_MACRO;
	uint8_t vselect = MENU_MAIN_OPTION_COUNT-2,hselect = 0;
	int8_t vgrid_id = 0;

	// processed input
	bool hit_a,hit_b;
	int8_t lrmv,udmv;

	// animation
	float transition_delta;
	bool menu_action = false;
	float mtransition = .0f,inv_mtransition;
	float ftransition = .0f,inv_ftransition;
	float delta_tspeed;

	// text
	glm::vec2 mo_prog = MENU_OPTIONS_CADDR;
	glm::vec2 mo_cposition[MENU_MAIN_OPTION_COUNT];
	float mo_twidth[MENU_MAIN_OPTION_COUNT],mo_hwidth[MENU_MAIN_OPTION_COUNT];
	float st_rot = .0f;

	// selectors
	glm::vec2 vrt_lpos = glm::vec2(0),vrt_upos = glm::vec2(0);
	uint16_t vrt_lwidth = 0,vrt_uwidth = 0;
	float lhead_translation_y,uhead_translation_y;

	// memory for static continue
	uint8_t dg_continue;
	bool shot_popup = false;

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

	// splashes
	Buffer sh_buffer = Buffer();
	Shader sh_shader = Shader();

	// head selector
	float lr_head_extend = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_WIDTH),
		ur_head_extend = SPLICE_HEAD_MINIMUM_WIDTH+rand()%((uint16_t)SPLICE_HEAD_ORIGIN_WIDTH);

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
