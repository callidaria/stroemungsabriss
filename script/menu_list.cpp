#include "menu_list.h"

/*
	Constructor()
	purpose: constructor, that sets up an empty menu list, to fill with lines later. no interpretations.
*/
MenuList::MenuList()
{
	Font fproc = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",0,0); // TODO: minimize
	dtxt.push_back(Text(fproc));
	esize = 0;
}

/*
	Constructor(Camera2D*,const char*)
	cam2d: text will be rendered according to the camera parameter's perspective and view
	path: path to menu list configuration file in cmli language format
	dpath: path to menu description configuration
	purpose: contructor, that interprets the given cmli file and sets up the menu list accordingly.
	!! ATTENTION !! will be outdated soon
*/
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

MenuList::MenuList(Camera2D* cam2d,const char* path)
{
	// setting up the different fonts & texts for menu parts
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	ltxt = Text(lfnt);
	// FIXME: do not make the fonts with static parameters

	// gathering raw node data
	std::ifstream mlfile(path,std::ios::in);	// reading file
	std::vector<std::string> ndata; 		// variable for raw node data
	std::string t_content;				// temporary node content
	bool ractive = false;				// active reading process (in node)
	while (!mlfile.eof()) {
		std::string line;getline(mlfile,line);
		bool enode = !strcmp(line.c_str(),"</>");
		if (ractive&&!enode) {
			line.erase(std::remove(line.begin(),line.end(),'\t'),line.end());
			t_content+=line;
		} else if (enode) {
			ractive = false;
			ndata.push_back(t_content);
			t_content = "";
		} else if (!strcmp(line.c_str(),"<node>")) ractive = true;
		//ractive = bnode+ractive*enode; // FIXME: reform
	} for (int i=0;i<ndata.size();i++) {
		// TODO: interpret raw lines
	}
}

/*
	add_lines(Camera2D*,std::vector<const char*>)
	cam2d: text will be rendered according to the camera parameter's perspective and view
	lines: list of lines that will be added to the menu list
	purpose: good question.
*/
void MenuList::add_lines(Camera2D* cam2d,std::vector<const char*> lines)
{
	// FIXME: add a respective description with the lines or remove this adder
	for (int i=0;i<lines.size();i++) {
		ltxt.add(lines.at(i),glm::vec2(250,lscroll));
		lscroll-=45;esize++;
	} ltxt.load_wcam(cam2d);
}

/*
	render(float,float,uint16_t)
	dtrans: represents the stage of transition in the menu's colouring & geometry
	lscroll: shows how far the player scrolled through the menu
	index: shows the index of the selected menu point
	purpose: renders the menu list on top of the menu visuals.
*/
void MenuList::render(float dtrans,float lscroll,uint16_t index)
{
	// rendering the selectable list text
	ltxt.prepare();
	int32_t fscroll = lscroll*45; // calculating the amount of scrolling
	ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(rand()%10,fscroll+rand()%10,0))); // shadow scroll
	ltxt.render(dtrans*2048,glm::vec4(.35f,.35f,.35f,.5f)); // rendering fake shadow
	ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(0,fscroll,0))); // list scroll in shader
	ltxt.render(dtrans*2048,glm::vec4(1,1,1,1));
	// ??maybe do shadow calculation in shader
	// FIXME: reduce call to just translation from mat4(1) and translation from model
	// FIXME: performance

	// rendering the description text in white
	dtxt.at(index).prepare();
	dtxt.at(index).render(dtrans*1024,glm::vec4(1,1,1,1));
}
