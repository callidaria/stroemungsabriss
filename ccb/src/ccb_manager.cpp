#include "../fcn/ccb_manager.h"

CCBManager::CCBManager(Frame* frame,Renderer2D* r2d,Camera2D* cam2d)
	: m_frame(frame),m_r2d(r2d),m_cam2d(cam2d)
{
	m_r2d->add(glm::vec2(1230,10),40,20,"res/dev.png");
	cf = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",20,20);
	ct = Text(&cf);
	ct.add("Welcome to the CASCABEL shell",glm::vec2(750,50));
	ct.add('>',glm::vec2(750,30));
}
int CCBManager::add_lv(const char* path,Text* txt)
{
	CCBLInterpreter proc = CCBLInterpreter(m_r2d,txt,path);
	linpr.push_back(proc);
	return proc.load_level();
}
void CCBManager::dev_console()
{
	m_r2d->prepare();m_r2d->render_sprite(0,1);
	if (m_frame->kb.ka[SDL_SCANCODE_COMMA]&&!activeonsc) { // ??find a way without branch. CPU dont like much.
		activeonsc = true;
		dev_active = !dev_active;
	} activeonsc = m_frame->kb.ka[SDL_SCANCODE_COMMA];
	if (dev_active) {
		ct.prepare();ct.render(1024,glm::vec4(0,.7f,0,1));
		ct.load_wcam(m_cam2d);
	}
}
