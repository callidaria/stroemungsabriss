#include "menu_list.h"

MenuList::MenuList()
{
	Font fproc = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",0,0); // TODO: minimize
	dtxt.push_back(Text(fproc));
	esize = 0;
}
MenuList::MenuList(Camera2D* cam2d,const char* path,const char* dpath)
{
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	ltxt = Text(lfnt);
	std::ifstream mlfile(path,std::ios::in);
	std::ifstream mdfile(dpath,std::ios::in);
	while (!mlfile.eof()) {
		std::string line;getline(mlfile,line);
		ltxt.add(line.c_str(),glm::vec2(250,lscroll));
		lscroll-=45;esize++;
	} ltxt.load_wcam(cam2d);
	bool body = false;Text proc=Text(dfnt);
	while (!mdfile.eof()) {
		std::string line;getline(mdfile,line);
		if (body) { // FIXME: YIKES AES this is what nietzsche meant
			if (!strcmp(line.c_str(),">")) {
				body=false;proc.load_wcam(cam2d);dtxt.push_back(proc);dscroll=600;proc=Text(dfnt);
			} else { proc.add(line.c_str(),glm::vec2(920,dscroll));dscroll-=20; }
		} else {
			if (!strcmp(line.c_str(),"><b")) { body=true;dscroll-=100; }
			else { proc.add(line.c_str(),glm::vec2(920,dscroll));dscroll-=20; }
		}
	}
}
void MenuList::add_lines(Camera2D* cam2d,std::vector<const char*> lines)
{
	// FIXME: add a respective description with the lines or remove this adder
	for (int i=0;i<lines.size();i++) {
		ltxt.add(lines.at(i),glm::vec2(250,lscroll));
		lscroll-=45;esize++;
	} ltxt.load_wcam(cam2d);
}
void MenuList::render(float dtrans,float lscroll,uint16_t index)
{
	ltxt.prepare();
	ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(0,lscroll*45,0)));
	ltxt.render(dtrans*2048,glm::vec4(1,1,1,1));
	dtxt.at(index).prepare();
	dtxt.at(index).render(dtrans*1024,glm::vec4(1,1,1,1));
}
