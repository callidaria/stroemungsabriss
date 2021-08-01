#include "level_manager.h"

LevelManager::LevelManager(Frame* frame,Renderer2D* r2d)
	: m_frame(frame),m_r2d(r2d)
{
	//ri = m_r2d->add(glm::vec2(100,100),100,100,"res/flyfighter.png");
	player = Player(r2d);
	BossDPilot dpilot;
	dpilot.load(r2d);
	lmains[0] = dpilot;
}
LevelManager::~LevelManager() {  }
void LevelManager::run(uint8_t &mm)
{
	while (mm==2) {
		bool eq = false;
		m_frame->print_fps();m_frame->input(eq,NULL);m_frame->clear(.1f,.1f,.1f);
		if (m_frame->kb.ka[SDL_SCANCODE_ESCAPE]) break;
		//m_r2d->prepare();m_r2d->render_sprite(ri,ri+1);
		//player.update();
		lmains[0].update();
		m_frame->update();
	}
}
