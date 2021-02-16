#include "../fcn/cldl_interpreter.h"

CLDLInterpreter::CLDLInterpreter(Renderer2D* r2d,Text* txt)
	: m_r2d(r2d),m_txt(txt) { }
void CLDLInterpreter::load_level(const char* path)
{
	std::fstream lvlfile;
	lvlfile.open(path,std::ios::in);
	std::string call;
	while (getline(lvlfile,call)) {
		std::string type;
		std::stringstream rd(call);rd>>type;
		if (type=="sprite:") {
			std::string tp;
			float x,y,w,h;
			rd>>x;rd>>y;rd>>w;rd>>h;rd>>tp;
			char* tpcp = new char[tp.size()+1];
			std::strcpy(tpcp,tp.c_str());
			m_r2d->add(glm::vec2(x,y),w,h,tpcp);
			spritesCount++;
		} else if (type=="text:") {
			std::string txt;
			float x,y;
			rd>>txt;rd>>x;rd>>y;
			m_txt->add(txt.c_str(),glm::vec2(x,y));
			charsCount += txt.length();
		}
	}
}
