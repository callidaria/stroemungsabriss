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
	int out = proc.load_level();
	linpr.push_back(proc);
	index.push_back(out);
	return out;
}
void CCBManager::dev_console(bool &running,bool &dactive)
{
	m_r2d->prepare();m_r2d->render_sprite(0,1);
	if (m_frame->kb.ka[SDL_SCANCODE_COMMA]&&!activeonsc) { // ??find a way without branch. CPU dont like much.
		activeonsc = true;
		if (dactive) { m_frame->input_stop();m_frame->tline.pop_back(); } // !!optimize
		else m_frame->input_start();
		dactive = !dactive;
	} activeonsc = m_frame->kb.ka[SDL_SCANCODE_COMMA];
	if (mv) {
		if (m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) mv = false;
		activeonentr = m_frame->kb.ka[SDL_SCANCODE_RETURN];
		glm::vec2 deltamv = glm::vec2(m_frame->mouse.mxfr-mlf.x,m_frame->mouse.myfr-mlf.y);
		int ti = index.at(i_lv)+i_rf;
		m_r2d->sl.at(ti).model = glm::translate(m_r2d->sl.at(ti).model,glm::vec3(deltamv.x,deltamv.y,0));
		linpr.at(i_lv).m_pos.at(i_rf) += deltamv;
		mlf = glm::vec2(m_frame->mouse.mxfr,m_frame->mouse.myfr);
	} else if (scl) {
		if (m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) scl = false;
		activeonentr = m_frame->kb.ka[SDL_SCANCODE_RETURN];
		deltascl += (m_frame->mouse.myfr-mlf.y)*0.001f;
		int ti = index.at(i_lv)+i_rf;
		m_r2d->sl.at(ti).scale_arbit(deltascl,deltascl);
		linpr.at(i_lv).m_width.at(i_rf)=tmp_wscale*deltascl;
		linpr.at(i_lv).m_height.at(i_rf)=tmp_hscale*deltascl;
		mlf = glm::vec2(m_frame->mouse.mxfr,m_frame->mouse.myfr);
	} else if (dactive) {
		if (m_frame->kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) {
			activeonentr = true;
			ct.add(m_frame->tline.c_str(),glm::vec2(770,console_y));console_y-=20;
			std::vector<std::string> args;
			std::string tmp = "";
			for (int i=0;i<m_frame->tline.length();i++) {
				if (m_frame->tline[i]==' ') { args.push_back(tmp);tmp=""; }
				else tmp.push_back(m_frame->tline[i]);
			} args.push_back(tmp);
			if (args.at(0)=="exit") running = false;
			else if (args.at(0)=="mv_sprite") {
				if (args.size()!=3)
					ct.add("invalid number of arguments; should be 2",glm::vec2(750,console_y));
				else {
					mv = true;
					mlf = glm::vec2(m_frame->mouse.mxfr,m_frame->mouse.myfr);
					i_lv=std::stoi(args.at(1));i_rf=std::stoi(args.at(2));
				}
			} else if (args.at(0)=="scl_sprite") {
				if (args.size()!=3)
					ct.add("invalid number of arguments; should be 2",glm::vec2(750,console_y));
				else {
					scl = true;
					mlf = glm::vec2(m_frame->mouse.mxfr,m_frame->mouse.myfr);
					i_lv=std::stoi(args.at(1));i_rf=std::stoi(args.at(2));
					tmp_wscale=linpr.at(i_lv).m_width.at(i_rf);
					tmp_hscale=linpr.at(i_lv).m_height.at(i_rf);
				}
			} else if (args.at(0)=="write_changes") {
				for (int i=0;i<linpr.size();i++) linpr.at(i).write_level();
				ct.add("changes written",glm::vec2(750,console_y));
			} else {
				std::string errline = "invalid command: \""+args.at(0)+"\"";
				ct.add(errline.c_str(),glm::vec2(750,console_y));
			}
			m_frame->tline = "";console_y-=20;
			ct.add('>',glm::vec2(750,console_y));
			cscroll = glm::translate(cscroll,glm::vec3(0,40,0));
		} activeonentr = m_frame->kb.ka[SDL_SCANCODE_RETURN];
		if (m_frame->mouse.mcl&&!activeonmcl) {
			std::string mouse_out
				= std::to_string(m_frame->mouse.mxfr)+'x'+std::to_string(m_frame->mouse.myfr);
			ct.add(mouse_out.c_str(),glm::vec2(770,console_y));console_y-=20;
			cscroll = glm::translate(cscroll,glm::vec3(0,20,0));
		} activeonmcl = m_frame->mouse.mcl;
		cl.clear(); // ??maybe directly add and remove chars from text instead of rewrite
		cl.add(m_frame->tline.c_str(),glm::vec2(770,30));
		//cl.load_wcam(m_cam2d);
		ct.prepare();//ct.set_scroll(glm::translate(cscroll,glm::vec3(0,-console_y,0)));
		ct.set_scroll(cscroll);ct.render(1024,glm::vec4(0,.7f,0,1));
		cl.prepare();cl.render(m_frame->tline.size(),glm::vec4(.7f,.7f,.2f,1));
	}
}
