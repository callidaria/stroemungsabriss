#include "menu_list.h"

MenuList::MenuList(Camera2D* cam2d,const char* path,const char* dpath)
{
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	ltxt = Text(lfnt);
	std::ifstream mlfile(path,std::ios::in);
	std::ifstream mdfile(dpath,std::ios::in);
	while (!mlfile.eof()) {
		std::string line;getline(mlfile,line);
		ltxt.add(line.c_str(),glm::vec2(250,lscroll));
		lscroll-=45;esize++;
	} ltxt.load_wcam(cam2d);
	bool body = false;Text proc=Text(lfnt);
	while (!mdfile.eof()) {
		// TODO: insert description reading
		std::string line;getline(mdfile,line);
		if (body) { // FIXME: YIKES AES this is what nietzsche meant
			if (!strcmp(line.c_str(),">")) {
				body=false;proc.load_wcam(cam2d);dtxt.push_back(proc);dscroll=675;proc=Text(lfnt);
			} else { proc.add(line.c_str(),glm::vec2(800,dscroll));dscroll-=45; }
		} else {
			if (!strcmp(line.c_str(),"><b")) { body=true;dscroll-=70; }
			else { proc.add(line.c_str(),glm::vec2(800,dscroll));dscroll-=45; }
		}
	}
}
void MenuList::add_lines(Camera2D* cam2d,std::vector<const char*> lines)
{
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
	if (index<dtxt.size()) { // FIXME: yikes flushing is for degerates
		dtxt.at(index).prepare();
		dtxt.at(index).render(dtrans*1024,glm::vec4(1,1,1,1));
	}
}
