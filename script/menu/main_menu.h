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

// information
constexpr uint8_t INFO_VERSION_RELEASE = 0;
constexpr uint8_t INFO_VERSION_SUBRELEASE = 0;
constexpr uint8_t INFO_VERSION_DEVSTEP = 6;
constexpr char INFO_VERSION_MODE_SUFFIX = 'c';
// MODI: c = "development", t = "QA build", p = "polishing", R = "release"

// system constants
constexpr uint8_t SPLICE_VERTEX_FLOAT_COUNT = 6;

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
constexpr glm::vec3 SPLICE_TITLE_COLOUR = glm::vec3(.5f,0,0);

// head splice
constexpr float SPLICE_HEAD_LOWER_START = 520.f;
constexpr float SPLICE_HEAD_UPPER_START = 470.f;
constexpr float SPLICE_HEAD_LOWER_WIDTH = 50.f;
constexpr float SPLICE_HEAD_UPPER_WIDTH = 75.f;
constexpr glm::vec3 SPLICE_HEAD_COLOUR = glm::vec3(.5f,.5f,.0f);

// selection splice
constexpr float SPLICE_SELECTION_LOWER_START = 670.f;
constexpr float SPLICE_SELECTION_UPPER_START = 820.f;
constexpr float SPLICE_SELECTION_LOWER_WIDTH = 20.f;
constexpr float SPLICE_SELECTION_UPPER_WIDTH = 125.f;
constexpr glm::vec3 SPLICE_SELECTION_COLOUR = glm::vec3(.0f,.5f,.5f);

// text attributes
constexpr glm::vec2 TEXT_DARE_POSITION = glm::vec2(450,250);
constexpr glm::vec4 TEXT_DARE_COLOUR = glm::vec4(1,0,0,1);
constexpr glm::vec2 TEXT_VERSION_POSITION = glm::vec2(650,20);
constexpr glm::vec4 TEXT_VERSION_COLOUR = glm::vec4(.25f,0,.75f,1);

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

class MainMenu : public UI
{
public:

	MainMenu() {  }
	MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float &progress,float pseq);
	~MainMenu() {  }

	// draw
	virtual void render(FrameBuffer* game_fb,bool &running,bool &reboot);

private:

	// splashes
	void create_splash(std::vector<float> &sverts,glm::vec2 l,glm::vec2 u,glm::vec3 c);
	void modify_splash(glm::vec2 lp,glm::vec2 up,float le,float ue,bool hrz);

	// system
	void update_peripheral_annotations();

private:

	// cascabel
	CCBManager* m_ccbm;
	CascabelBaseFeature* m_ccbf;
	World* m_world;
	FrameBuffer fb_menu;
	Text tx_dare = Text(Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",25,25)),
		tx_version = Text(Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15));
	MSAA msaa;

	// splashes
	Buffer sh_buffer = Buffer();
	Shader sh_shader = Shader();

	// index
	uint16_t index_rsprite,index_ranim;
	uint8_t tcap_dare,tcap_version;

	// input
	bool cpref_peripheral;
	bool trg_lmb = false,trg_rmb = false;

	// animation
	bool menu_action = false;
	float mtransition = .0f;
	float anim_timing = .0f;
	float dt_tshiftdown = .0f,dt_tnormalize = .0f;
	bool speedup = true;
};

#endif

/*
			TODO QA:

	- TRANSITION_SPEED modifier too fast/too slow?
	- input satisfaction feedback
*/
