#include "../fcn/ccb_linpr.h"

CCBLInterpreter::CCBLInterpreter(Renderer2D* r2d,Text* txt,const char* path)
	: m_r2d(r2d),m_txt(txt),lvpath(path) {  }
int CCBLInterpreter::load_level()
{
	int out = m_r2d->get_max_sprite();
	std::ifstream lvfile(lvpath,std::ios::in);
	while (!lvfile.eof()) {
		char* lh = new char[32];
		lvfile >> lh;
		const char* rclh = reinterpret_cast<char*>(&lh[0]);
		if (!strcmp(rclh,"sprite:")) {
			float x,y,w,h;
			char* tpath = new char[128];
			const char* rctpath = reinterpret_cast<char*>(&tpath[0]);
			lvfile >> x >> y >> w >> h >> tpath;
			m_pos.push_back(glm::vec2(x,y));
			m_width.push_back(w);m_height.push_back(h);
			m_tex.push_back(rctpath);
			m_r2d->add(glm::vec2(x,y),w,h,rctpath);
		}
	} return out;
}
int CCBLInterpreter::edit_level(const char* path,int id,glm::vec2 pos,float width,float height,const char* tp)
{
	return 0;
}
