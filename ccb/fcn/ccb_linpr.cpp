#include "ccb_linpr.h"

/*
	constructor(Renderer2D*,const char*)
	r2d: points to 2D renderer handling all sprite & anim creation requests
	path: path to environment cascabel file holding all creation requests & parameters
	purpose: saves path & renderer for later usage
*/
CCBLInterpreter::CCBLInterpreter(Renderer2D* r2d,const char* path)
	: m_r2d(r2d),lvpath(path) {  }

/*
	load_level() -> uint16_t
	purpose: process all creation requests held by the cascabel environment file
	returns: starting draw address for 2D renderer
*/
uint16_t CCBLInterpreter::load_level()
{
	int out = m_r2d->get_max_sprite();
	std::ifstream lvfile(lvpath,std::ios::in);
	while (!lvfile.eof()) {
		char* lh = new char[32];
		lvfile >> lh;
		const char* rclh = reinterpret_cast<char*>(&lh[0]);
		if (!strcmp(rclh,"sprite:")) {
		// pattern for spritedef: lh position_x position_y width height texpath
			float x,y,w,h;
			char* tpath = new char[128];
			const char* rctpath = reinterpret_cast<char*>(&tpath[0]); // ??wat
			lvfile >> x >> y >> w >> h >> tpath;
			m_pos.push_back(glm::vec2(x,y));
			m_width.push_back(w);m_height.push_back(h);
			m_tex.push_back(rctpath);
			m_r2d->add(glm::vec2(x,y),w,h,rctpath);
		} else if (!strcmp(rclh,"anim:")) {
		// pattern for animdef: lh position_x position_y width height texpath rows columns frames timesplit
			float x,y,w,h,r,c,f,t;
			char* tpath = new char[128];
			const char* rctpath = reinterpret_cast<char*>(&tpath[0]);
			lvfile >> x >> y >> w >> h >> tpath >> r >> c >> f >> t;
			a_pos.push_back(glm::vec2(x,y));
			a_width.push_back(w);a_height.push_back(h);
			a_tex.push_back(rctpath);
			a_row.push_back(r);a_column.push_back(c);
			a_frames.push_back(f);a_ts.push_back(t);
			m_r2d->add(glm::vec2(x,y),w,h,rctpath,r,c,f,t);
		}
	} return out;
}

/*
	write_level() -> void
	purpose: used by the developer console; writes changes to the creation list
*/
void CCBLInterpreter::write_level()
{
	std::ofstream lvfile(lvpath,std::ios::out);
	for (int i=0;i<m_pos.size();i++) {
		std::stringstream lvbuff;
		lvbuff <<"sprite: "<<m_pos.at(i).x<<' '<<m_pos.at(i).y<<' '
			<<m_width.at(i)<<' '<<m_height.at(i)<<' '<<m_tex.at(i)<<'\n';
		lvfile << lvbuff.str();
	} for (int i=0;i<a_pos.size();i++) {
		std::stringstream lvbuff;
		lvbuff <<"anim: "<<a_pos.at(i).x<<' '<<a_pos.at(i).y<<' '
			<<a_width.at(i)<<' '<<a_height.at(i)<<' '<<a_tex.at(i)<<' '
			<<a_row.at(i)<<' '<<a_column.at(i)<<' '<<a_frames.at(i)<<' '<<a_ts.at(i)<<'\n';
		lvfile << lvbuff.str();
	} lvfile.close();
}