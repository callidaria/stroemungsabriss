#include "menu_list.h"

/*
	Constructor()
	purpose: constructor, that sets up an empty menu list, to fill with lines later. no interpretations.
*/
MenuList::MenuList()
{
	Font fproc = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",0,0); // TODO: minimize
	//dtxt.push_back(Text(fproc)); // FIXME: breakdown if necessary in new update
	LEntity proc = {
		Text(fproc),Text(fproc),std::vector<std::string>(),std::vector<std::string>(),
		false,0,0,0,0,0,""
	};
	les.push_back(proc);
	esize = 0;
}

/*
	Constructor(Camera2D*,const char*)
	cam2d: text will be rendered according to the camera parameter's perspective and view
	path: path to menu list configuration file in updated cmli2 language format
	purpose: constructor, that interprets single cmli2 file and sets up the menu list accordingly
	cmli syntax definitions:
		<head>*content*</head>
		<desc>*description*</desc>
		(optional)<le>*element*,*value*</le>
		(optional)<slider>*slider min (int)*,*slider max (int)*</slider>
		(optional)<dest>*destination save name in config*</dest>
*/
MenuList::MenuList(Camera2D* cam2d,const char* path)
{
	// setting up the different fonts & texts for menu parts
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	Text t_desc;	   // temporary storage variable for node descriptions
	// FIXME: do not make the fonts with static parameters

	// gathering raw node data
	std::ifstream mlfile(path,std::ios::in); // reading file
	std::vector<std::string> ndata; 	 // variable for raw node data
	std::string t_content;			 // temporary node content
	std::string t_line = "";		 // temporary line content for description text before break
	bool ractive = false;			 // active reading process (in node)

	// extracting raw node data from file
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
	}

	// interpreting raw node data as cmli pattern
	std::vector<std::string> args;
	std::string t_extract;
	for (int i=0;i<ndata.size();i++) {
		uint32_t ix = 0;     // index of raw node data
		struct LEntity t_le; // temporary list element, getting pushed back later
		t_le.ltxt = Text(lfnt);
		while (ix<ndata[i].size()) {
			uint8_t emode = get_readmode(ndata[i],ix);   // check character for mode annotation
			std::string excnt = breakgrind(ndata[i],ix); // get content inside annotation area
			switch (emode) {
			case 1: // conservative menu list element
				t_le.ltxt.add(excnt.c_str(),glm::vec2(250,lscroll)); // write to main list element
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
				t_le.dtxt = t_desc; // save description in the description list
				dscroll = 600;	    // reset cursor scroll
				t_line = "";	    // reset temporary line
				break;
			case 3: // selectable menu sublist element in dropdown
				// TODO: split integer interpretation according to set pattern
				// TODO: read from system for some list elements (eg. monitor ID)
				args = split_arguments(excnt); // getting argument list
				t_le.lee.push_back(args[0]); // save entity parameter as sublist element
				t_le.lev.push_back(args[1]); // save value parameter as value list element
				break;
			case 4: // slider element inserted
				// TODO: interpret slider parameters as integer construct
				t_le.slide = true; // setting slider to be active and visible
				args = split_arguments(excnt); // define the capacities according to pattern
				t_le.sl_min = stoi(args[0]);
				t_le.sl_max = stoi(args[1]);
				t_le.sl_vmin = stoi(args[2]);
				t_le.sl_vmax = stoi(args[3]);
				break;
			case 5: // destination annotation
				t_le.saveID = excnt;
				break;
			default:printf("%s uses unknown or outdated annotation ID: %i\n",path,emode);
			}
		} t_le.ltxt.load_wcam(cam2d); // load conservative list element data
		les.push_back(t_le);
	} // FIXME: text with annotated \n in written file format -> is it \n or \\n?
} 	// TODO: write interpreter fault instead of letting the mem take the fall
	// TODO: read selection status from config file

/*
	save(void)
	purpose: identify destination & write all changes to config file
*/
void MenuList::save()
{
	// TODO: destination check
	// TODO: writing routine
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
	for (int i=0;i<les.size();i++) {
		// rendering the selectable list text
		les.at(i).ltxt.prepare();
		int32_t fscroll = lscroll*45; // calculating the amount of scrolling
		les.at(i).ltxt.set_scroll(glm::translate(glm::mat4(1.0f),
			glm::vec3(rand()%10,fscroll+rand()%10,0)));			 // shadow scroll
		les.at(i).ltxt.render(dtrans*2048,glm::vec4(.35f,.35f,.35f,.5f)); 	 // rendering fake shadow
		les.at(i).ltxt.set_scroll(glm::translate(glm::mat4(1.0f),
			glm::vec3(0,fscroll,0)));					 // list scroll in shader
		les.at(i).ltxt.render(dtrans*2048,glm::vec4(1,1,1,1));			 // render main text
		// ??maybe do shadow calculation in shader
		// FIXME: reduce call to just translation from mat4(1) and translation from model
		// FIXME: reduce max render cap after broken down save system
		// FIXME: performance

		// rendering the description text in white
		les.at(i).dtxt.prepare();
		les.at(i).dtxt.render(dtrans*1024,glm::vec4(1,1,1,1));
		// FIXME: broken render flow
	}
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
uint8_t MenuList::textgrind(std::string fline,uint32_t &i) // FIXME: collapsable into grind or getter??
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
		3 := dropdown list element marked as "<le>"
		4 := slider element marked as "<slider>"
		5 := save destination annotation marked as "<dest>"
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
	return !strcmp(pcnt.c_str(),"head")+2*!strcmp(pcnt.c_str(),"dsc")+3*!strcmp(pcnt.c_str(),"le")
		+4*!strcmp(pcnt.c_str(),"slider")+5*!strcmp(pcnt.c_str(),"dest");
	// output ID according to annotation
}

/*
	split_arguments(std::string)
	ext: represents the extraction string
	purpose: getting an argument list from extraction string
*/
std::vector<std::string> MenuList::split_arguments(std::string ext)
{
	std::vector<std::string> args;
	std::string t_arg;
	for (int i=0;i<ext.size();i++) {
		if (ext[i]==',') args.push_back(t_arg);
		else t_arg += ext[i];
	} args.push_back(t_arg);
	return args;
} // TODO: optimize
