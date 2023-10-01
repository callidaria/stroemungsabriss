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

// system constants
constexpr uint8_t MENU_GBUFFER_COLOUR = 0;
constexpr uint8_t MENU_GBUFFER_NORMALS = 1;
constexpr uint8_t MENU_MAIN_OPTION_COUNT = 7;
constexpr uint8_t MENU_MAIN_OPTION_CAP = MENU_MAIN_OPTION_COUNT-1;
constexpr uint8_t SPLICE_VERTEX_FLOAT_COUNT = 6;
constexpr float MENU_HALFSCREEN_UI = 1280.f*.5f;

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
constexpr float SPLICE_HEAD_LOWER_START = 520.f;
constexpr float SPLICE_HEAD_UPPER_START = 470.f;
constexpr float SPLICE_HEAD_LOWER_WIDTH = 42.f;
constexpr float SPLICE_HEAD_UPPER_WIDTH = 50.f;

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
 * 		TODO QA
 * 
 * - TRANSITION_SPEED modifier too fast/too slow?
 * - input satisfaction feedback
 * - aliasing recognition threshold & which technique preferred
*/

class MainMenu : public UI
{
public:

	MainMenu() {  }
	MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float &progress,float pseq);
	~MainMenu() {  }

	// draw
	virtual void render(FrameBuffer* game_fb,bool &running,bool &reboot);

private:

	// logic
	uint8_t get_selected_main_option(float mx,bool &ch_select);

	// splashes
	void create_splash(std::vector<float> &sverts,glm::vec2 l,glm::vec2 u,glm::vec3 c);
	void modify_splash(glm::vec2 lp,glm::vec2 up,float le,float ue,bool hrz);

	// system
	void update_peripheral_annotations();

public:

	// index
	uint8_t interface_logic_id = 0;

	// processed input
	bool hit_a,hit_b;
	int8_t lrmv;

private:

	// engine
	CCBManager* m_ccbm;
	CascabelBaseFeature* m_ccbf;
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

	// text
	glm::vec2 mo_prog = MENU_OPTIONS_CADDR;
	glm::vec2 mo_cposition[MENU_MAIN_OPTION_COUNT];
	float mo_twidth[MENU_MAIN_OPTION_COUNT],mo_hwidth[MENU_MAIN_OPTION_COUNT];
	float st_rot = .0f;

	// index
	uint16_t index_rsprite,index_ranim;
	uint8_t tcap_dare,tcap_version;

	// input
	bool cpref_peripheral;
	bool trg_lmb = false,trg_rmb = false;

	// selectors
	uint8_t vselect = MENU_MAIN_OPTION_COUNT-2,hselect = 0;
	glm::vec2 vrt_lpos = glm::vec2(0),vrt_upos = glm::vec2(0);
	uint16_t vrt_lwidth = 0,vrt_uwidth = 0;

	// animation
	bool menu_action = false;
	float mtransition = .0f;
	float anim_timing = .0f;
	float dt_tshiftdown = .0f,dt_tnormalize = .0f;
	bool speedup = true;

	// predefinitions
	typedef void (*interface_logic)(MainMenu&);
	interface_logic interface_behaviour[2];
	const char* main_options[MENU_MAIN_OPTION_COUNT]
		= { "exit","options","extras","practice","load","continue","new game" };
};

static void interface_behaviour_macro(MainMenu &tm);
static void interface_behaviour_options(MainMenu &tm);

#endif
