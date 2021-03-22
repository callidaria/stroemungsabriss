#include "../fcn/ccb_linpr.h"

CCBLInterpreter::CCBLInterpreter(Renderer2D* r2d,Text* txt)
	: m_r2d(r2d),m_txt(txt) { }
void CCBLInterpreter::load_level(const char* path)
{
	/*std::fstream lvlfile;
	lvlfile.open(path,std::ios::in);
	std::string call;
	while (getline(lvlfile,call)) {
		std::string type;
		std::stringstream rd(call);rd>>type;
		if (type=="sprite:") {
			std::string tp;
			float x,y,w,h;
			//rd>>x;rd>>y;rd>>w;rd>>h;rd>>tp;
			char* tpcp = new char[tp.size()+1];
			std::strcpy(tpcp,tp.c_str());
			//printf("vec2(%f,%f),%f %f %s\n",x,y,w,h,tpcp);
			//m_r2d->add(glm::vec2(x,y),w,h,tpcp);
			std::cout << tpcp << '\n';
			m_r2d->add(glm::vec2(x,y),w,h,tpcp);
			spritesCount++;
		} else if (type=="text:") {
			std::string txt;
			float x,y;
			rd>>txt;rd>>x;rd>>y;
			m_txt->add(txt.c_str(),glm::vec2(x,y));
			charsCount += txt.length();
		}
	}*/
	FILE* lvfile = fopen(path,"r");
	while (true) {
		char lh[128];
		int res = fscanf(lvfile,"%s",lh);
		if (res==EOF) break;
		else {
			if (!strcmp(lh,"sprite:")) {
				float x,y,w,h;
				char* tp = new char[32];
				fscanf(lvfile,"%f %f %f %f %s",&x,&y,&w,&h,tp);
				m_r2d->add(glm::vec2(x,y),w,h,reinterpret_cast<char*>(&tp[0]));
			} else if (!strcmp(lh,"text:")) {
				printf("loading text\n");
			}
		}
	}
}
