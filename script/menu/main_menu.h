#ifndef SRC_MENU_MAIN
#define SRC_MENU_MAIN

#include "../../ccb/frm/framebuffer.h"
#include "../../ccb/fcn/ccb_manager.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"
#include "../world.h"

// title position & transition destination
constexpr glm::vec3 VRT_TITLE_START = glm::vec3(300,300,0);
constexpr glm::vec3 VRT_TITLE_END = glm::vec3(100,250,0);
constexpr glm::vec3 HRZ_TITLE_START = glm::vec3(590,800,0);
constexpr glm::vec3 HRZ_TITLE_END = glm::vec3(590,642,0);
constexpr glm::vec3 VRT_TITLE_TRANSITION = VRT_TITLE_END-VRT_TITLE_START;
constexpr glm::vec3 HRZ_TITLE_TRANSITION = HRZ_TITLE_END-HRZ_TITLE_START;

// animation timing
constexpr float TITLE_SPEED = 8.5f;

class MainMenu : public UI
{
public:

	MainMenu() {  }
	MainMenu(CCBManager* ccbm,CascabelBaseFeature* ccbf,World* world,float &progress,float pseq);
	~MainMenu() {  }

	virtual void render(FrameBuffer* game_fb,bool &running,bool &reboot);

private:

	// cascabel
	CCBManager* m_ccbm;
	CascabelBaseFeature* m_ccbf;
	World* m_world;
	FrameBuffer fb;

	// index
	uint16_t index_rsprite,index_ranim;

	// input
	bool trg_lmb = false,trg_rmb = false;

	// animation
	bool menu_action = false;
	float mtransition = .0f;
};

#endif