#include "../fcn/ccb_manager.h"

CCBManager::CCBManager(Frame* frame,Renderer2D* r2d,Camera2D* cam2d)
	: m_frame(frame),m_r2d(r2d),m_cam2d(cam2d)
{
	m_r2d->add(glm::vec2(1230,10),40,20,"res/dev.png");
	cf = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",20,20);
	ct = Text(&cf);cl = Text(&cf);
	ct.add("Welcome to the CASCABEL shell",glm::vec2(750,console_y+20));
	int prog = ct.add('>',glm::vec2(750,console_y));
	cl.add("lineerr",glm::vec2(700+prog,30));
	ct.load_wcam(m_cam2d);cl.load_wcam(m_cam2d);
}
int CCBManager::add_lv(const char* path,Text* txt)
{
	CCBLInterpreter proc = CCBLInterpreter(m_r2d,txt,path);
	linpr.push_back(proc);
	return proc.load_level();
}
void CCBManager::dev_console(bool &dactive)
{
	m_r2d->prepare();m_r2d->render_sprite(0,1);
	if (m_frame->kb.ka[SDL_SCANCODE_COMMA]&&!activeonsc) { // ??find a way without branch. CPU dont like much.
		activeonsc = true;
		if (dactive) { m_frame->input_stop();m_frame->tline.pop_back(); } // !!optimize
		else m_frame->input_start();
		dactive = !dactive;
	} activeonsc = m_frame->kb.ka[SDL_SCANCODE_COMMA];
	if (dactive) {
		if (m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) {
			activeonentr = true;
			ct.add(m_frame->tline.c_str(),glm::vec2(770,console_y));
			m_frame->tline = "";console_y-=20;
			ct.add('>',glm::vec2(750,console_y));
			cscroll = glm::translate(cscroll,glm::vec3(0,20,0));
		} activeonentr = m_frame->kb.ka[SDL_SCANCODE_RETURN];
		cl.clear(); // ??maybe directly add and remove chars from text instead of rewrite
		cl.add(m_frame->tline.c_str(),glm::vec2(770,30));
		//cl.load_wcam(m_cam2d);
		ct.prepare();ct.set_scroll(cscroll);ct.render(1024,glm::vec4(0,.7f,0,1));
		cl.prepare();cl.render(m_frame->tline.size(),glm::vec4(.7f,.7f,.2f,1));
	}
}
