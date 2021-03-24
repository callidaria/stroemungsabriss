#include "../fcn/ccb_linpr.h"

CCBLInterpreter::CCBLInterpreter(Renderer2D* r2d,Text* txt)
	: m_r2d(r2d),m_txt(txt) { }
int CCBLInterpreter::load_level(const char* path)
{
	int out = m_r2d->get_max_sprite();
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
	} return out;
}
