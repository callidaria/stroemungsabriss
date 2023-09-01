#ifndef SRC_MENU_MAIN
#define SRC_MENU_MAIN

#include "../../ccb/frm/framebuffer.h"
#include "../../ccb/fcn/ccb_manager.h"

#include "../struct/feature_base.h"
#include "../struct/world_structures.h"
#include "../world.h"

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
};

#endif