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
	path: path to menu list configuration file in updated cmli2 language format
	purpose: constructor, that interprets single cmli2 file and sets up the menu list accordingly
*/
MenuList::MenuList(Camera2D* cam2d,const char* path)
{
	// setting up the different fonts & texts for menu parts
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	ltxt = Text(lfnt); // variable, storing the main list entity text
	Text t_desc;	   // temporary storage variable for node descriptions
	// FIXME: do not make the fonts with static parameters

	// gathering raw node data
	std::ifstream mlfile(path,std::ios::in); // reading file
	std::vector<std::string> ndata; 	 // variable for raw node data
	std::string t_content;			 // temporary node content
	std::string t_line = "";		 // temporary line content for description text before break
	bool ractive = false;			 // active reading process (in node)
	while (!mlfile.eof()) {
		std::string line;getline(mlfile,line);	  // reading line by line
		bool enode = !strcmp(line.c_str(),"</>"); // end node boolean true if line contains end annotation
		if (ractive&&!enode) { // if reading in node & the end of node has not been reached
			line.erase(std::remove(line.begin(),line.end(),'\t'),line.end()); // getting rid of tabs
			t_content+=line; // add processed line to content collection
		} else if (enode) { // if end of node has been reached
			ractive = false;	    // stop active reading (outside node)
			ndata.push_back(t_content); // save aquired content inside data list
			t_content = "";		    // reset content variable
		} else if (!strcmp(line.c_str(),"<node>")) ractive = true; // open node if marked accordingly
	} for (int i=0;i<ndata.size();i++) {
		uint32_t ix = 0; // index of raw node data
		while (ix<ndata[i].size()) {
			uint8_t emode = get_readmode(ndata[i],ix);   // check character for mode annotation
			std::string excnt = breakgrind(ndata[i],ix); // get content inside annotation area
			switch (emode) {
			case 1: // conservative menu list element
				ltxt.add(excnt.c_str(),glm::vec2(250,lscroll)); // write to main list elements
				lscroll -= 45; // scroll down after write
				esize++;       // inc list element counter
				break;
			case 2: // menu splash description
				t_desc = Text(dfnt); // temporary description to add later
				for (uint32_t iy=0;iy<excnt.size();iy++) { // reading through desc content
					if (textgrind(excnt,iy)) { // detecting if character is innocent
						t_desc.add(t_line.c_str(),glm::vec2(920,dscroll)); // add to desc
						dscroll -= 20; // scroll down description text cursor
						t_line = "";   // reset current temporary line
					} else t_line += excnt[iy]; // add innocent character to temporary line
				} t_desc.add(t_line.c_str(),glm::vec2(920,dscroll)); // add leftover line to desc
				t_desc.load_wcam(cam2d);			     // load written desc data
				dtxt.push_back(t_desc); // save description in the description list
				dscroll = 600;		// reset cursor scroll
				t_line = "";		// reset temporary line
				break;
			default:printf("%s is a broken cmli file\n",path);
			}
		} ltxt.load_wcam(cam2d); // load conservative list element data
	} // FIXME: text with annotated \n in written file format -> is it \n or \\n?
}

/*
	add_lines(Camera2D*,std::vector<const char*>)
	cam2d: text will be rendered according to the camera parameter's perspective and view
	lines: list of lines that will be added to the menu list
	purpose: good question.
	TODO: PREPARE FOR REMOVAL
*/
void MenuList::add_lines(Camera2D* cam2d,std::vector<const char*> lines)
{
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
	ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(rand()%10,fscroll+rand()%10,0))); // shadow scrll
	ltxt.render(dtrans*2048,glm::vec4(.35f,.35f,.35f,.5f)); 		 // rendering fake shadow
	ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(0,fscroll,0))); // list scroll in shader
	ltxt.render(dtrans*2048,glm::vec4(1,1,1,1)); // rendering the main text
	// ??maybe do shadow calculation in shader
	// FIXME: reduce call to just translation from mat4(1) and translation from model
	// FIXME: performance

	// rendering the description text in white
	dtxt.at(index).prepare();
	dtxt.at(index).render(dtrans*1024,glm::vec4(1,1,1,1));
}

/*
	breakgrind(std::string,uint32_t&)
	nl: represents the remaining nodeline to scan until break
	i: reading index for nodeline
	purpose: get contents to write into given interpreter mode
*/
std::string MenuList::breakgrind(std::string nl,uint32_t &i)
{
	std::string out;
	while (i<nl.length()) { // read nodeline per character
		if (nl[i]=='<'&&nl[i+1]=='/'&&nl[i+2]=='>') { // check for end annotation
			i += 3; // go 3 steps further on nodeline
			return out;
		} out += nl[i]; // add character to outputtable contents
		i++;		// go one character further on nodeline
	} return out; // catch contents if line apruptly ends & no mem shenanigans should occur somehow
	// FIXME: the mem issue if reading outside nl for "</>" annotation checks
}

/*
	textgrind(std::string,uint32_t&)
	fline: the current line in question
	i: index of current character in line
	purpose: check if current character contains beginning of a special value
		0 := faulty bracket contents
		1 := linebreak annotation with "<br>"
*/
uint8_t MenuList::textgrind(std::string fline,uint32_t &i)
{
	if (fline[i]=='<') { // character is not innocent
		std::string cast = "";	  // variable for readable cast
		i++;			  // go further on descline
		while (fline[i]!='>') {   // scan for description end
			cast += fline[i]; // add character to casted innocence description
			i++;		  // go even further on descline
		} if(!strcmp(cast.c_str(),"br")) return 1; // breakline annotation recognized
		else return 0; // not innocent but annotation not recognized
	} return 0;
}

/*
	get_readmode(std::string,uint8_t&)
	nl: represents the remaining nodeline to scan until break
	i: reading index for nodeline
	purpose: get the modeID, representing the mode in which the following text is supposed to be added
	modeID:
		0 := faulty element identifier read "<*>"
		1 := conservative menu list element marked as "<head>"
		2 := menu splash description text element marked as "<dsc>"
*/
uint8_t MenuList::get_readmode(std::string nl,uint32_t &i)
{
	std::string pcnt = "";	// mode annotation cast variable
	while (nl[i]!='<') i++;	// go to next annotation
	i++;			// go further
	while (nl[i]!='>') {	// scan until annotation bracket closes
		pcnt += nl[i];	// add current character to the cast variable
		i++;		// increment line index
	} i++;			// ready for further work with the nodeline
	return !strcmp(pcnt.c_str(),"head")+2*!strcmp(pcnt.c_str(),"dsc"); // output ID according to annotation
}
