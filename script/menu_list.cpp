#include "menu_list.h"

MenuList::MenuList(Camera2D* cam2d,const char* path)
{
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	ltxt = Text(lfnt);
	std::ifstream mlfile(path,std::ios::in);
	while (!mlfile.eof()) {
		std::string line;
		getline(mlfile,line);
		ltxt.add(line.c_str(),glm::vec2(250,lscroll));
		lscroll -= 45;
	} ltxt.load_wcam(cam2d);
}
MenuList::~MenuList() {  }
void MenuList::render(float dtrans)
{
	ltxt.prepare();
	ltxt.render(dtrans*2048,glm::vec4(1,1,1,1));
}
