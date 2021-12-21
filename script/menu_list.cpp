#include "menu_list.h"

/*
	Constructor()
	purpose: constructor, that sets up an empty menu list, to fill with lines later. no interpretations.
*/
MenuList::MenuList()
{
	Font fproc = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",0,0); // TODO: minimize
	LEntity proc = {
		Text(fproc),Text(fproc),std::vector<Text>(),std::vector<std::string>(),
		false,0,0,0,0,0,0,""
	}; // FIXME: check if defaults actually important
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
		(optional)<dest>*destination save name in config*,*default value*</dest>
		(optional)<le_TargetMonitor></>
		(optional)<diff>*estimated difficulty rating (no:0,kakusei:1-6,master:7-11,gm:12-14,ugm:15-17)*</>
*/
MenuList::MenuList(Renderer2D* r2d,Camera2D* cam2d,const char* path)
	: m_r2d(r2d)
{
	// setting up textures for 2D renderer
	diffRID = m_r2d->add(glm::vec2(950,550),250,50,"./res/menu/est_diff.png",16,2,30,0);
	m_r2d->add(glm::vec2(-125,-25),250,50,"./res/menu/est_diff.png",16,2,30,0);
	m_r2d->load_wcam(cam2d); // FIXME: sprite always in split, use different spritesheets

	// setting up the different fonts & texts for menu parts
	Font lfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",30,30);
	Font dfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	Text t_desc,t_sle; // temporary storage variable for node descriptions & sublist elements
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

	// load current configuration
	std::ifstream i_conf("config.ini",std::ios::in);
	std::vector<std::string> curr;
	while (!i_conf.eof()) {
		std::string line;getline(i_conf,line);
		std::vector<std::string> args = split_arguments(line,' ');
		if (args.size()>1) { // FIXME: overly obsolete branching, if only...
			curr.push_back(args[0]);
			curr.push_back(args[1]);
		} // FIXME: remove check when target monitor
	} // FIXME: default value after writing sits at 0 regardless of determined default in cmli

	// interpreting raw node data as cmli pattern
	std::vector<std::string> args;
	std::string t_extract;
	std::ofstream o_conf("config.ini",std::ios::app);
	bool written;
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
				// TODO: read from system for some list elements (eg. monitor ID)
				args = split_arguments(excnt,','); // getting argument list
				t_sle = Text(lfnt);
				t_sle.add(args[0].c_str(),glm::vec2(650,lscroll+45));
				t_sle.load_wcam(cam2d);
				t_le.lee.push_back(t_sle); // save entity parameter as sublist element
				t_le.lev.push_back(args[1]); // save value parameter as value list element
				break;
			case 4: // slider element inserted
				t_le.slide = true; // setting slider to be active and visible
				args = split_arguments(excnt,','); // define the capacities according to pattern
				t_le.sl_min = stoi(args[0]);
				t_le.sl_max = stoi(args[1]);
				t_le.sl_vmin = stoi(args[2]); // FIXME: remove slider value caps
				t_le.sl_vmax = stoi(args[3]);
				break;
			case 5: // destination annotation
				args = split_arguments(excnt,',');
				t_le.saveID = args[0];
				written = false;
				for (int i=0;i<curr.size();i+=2) {
					if (curr[i]==args[0]) {
						if (t_le.slide) t_le.sID = stoi(curr[i+1]);
						else t_le.sID = translate_index(t_le.lev,curr[i+1]);
						written = true;
						break;
					}
				} if (!written) {
					if (t_le.slide) o_conf << args[0] << ' ' << stoi(args[1]) << '\n';
					else if (t_le.lev.size()>0) // FIXME: remove check when target monitor
						o_conf << args[0] << ' ' << t_le.lev[stoi(args[1])] << '\n';
				}
				break; // FIXME: double branching where better solutions
			case 6: // reading target monitor for menu list insert
				for (int disp=0;disp<SDL_GetNumVideoDisplays();disp++) {
					t_sle = Text(lfnt);
					t_sle.add(std::to_string(disp).c_str(),glm::vec2(650,lscroll+45));
					t_sle.load_wcam(cam2d);
					t_le.lee.push_back(t_sle);
					t_le.lev.push_back(std::to_string(disp));
				} break; // FIXME: display value not as integer but as comm name
			case 7: // estimated difficulty rating
				t_le.diff = stoi(excnt);
				break;
			default:printf("%s uses unknown or outdated annotation ID: %i\n",path,emode);
			}
		} t_le.ltxt.load_wcam(cam2d); // load conservative list element data
		les.push_back(t_le);
	} // FIXME: text with annotated \n in written file format -> is it \n or \\n?
} // TODO: write interpreter fault instead of letting the mem take the fall

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
	render(float,float,uint16_t,float&,int8_t,bool)
	dtrans: represents the stage of transition in the menu's colouring & geometry
	lscroll: shows how far the player scrolled through the menu
	index: shows the index of the selected menu point
	edge_mod: midedge modding for the sublist splash
	delta: reads the slider sID increment from menu controlling
	rsl: shows if sublist mode is rendered
	md: returns the option entity render offset for menu
	purpose: renders the menu list on top of the menu visuals.
*/
void MenuList::render(float dtrans,float lscroll,uint16_t index,float &edge_mod,int8_t delta,bool rsl,uint8_t &md)
{
	// rendering all head list entities
	edge_mod = -1;
	md = 0;
	for (int i=0;i<les.size();i++) {
		// precalculations
		float x_ofs = -150*(les[i].slide||les[i].lee.size()>0);

		// rendering the selectable list text
		les[i].ltxt.prepare();
		int32_t fscroll = lscroll*45; // calculating the amount of scrolling
		les[i].ltxt.set_scroll(glm::translate(glm::mat4(1.0f),
			glm::vec3(x_ofs+rand()%10,fscroll+rand()%10,0)));		 // shadow scroll
		les[i].ltxt.render(dtrans*64*(index==i),glm::vec4(.54f,.17f,.89f,.75f)); // rendering shadow

		les[i].ltxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(x_ofs,fscroll,0)));
		les[i].ltxt.render(dtrans*64,glm::vec4(1,1,1,1));	// render main text
		// ??maybe do shadow calculation in shader
		// FIXME: performance

		// rendering the sublist and selection entity
		if (les[i].lee.size()>0&&!rsl) {
			les[i].lee[les[i].sID].prepare();
			les[i].lee[les[i].sID].set_scroll(glm::mat4(1.0f));
			les[i].lee[les[i].sID].render(dtrans*128,glm::vec4(1,1,1,1));
		} else if (rsl&&index==i&&!les[i].slide) {
			t_slID = t_slID*lf_open+les[i].sID*!lf_open;
			int32_t ofs = (les[i].lee.size()*25)/2-12;
			md = ofs;
			t_slID += delta*((delta>0&&t_slID<les[i].lee.size()-1)||(delta<0&&t_slID>0));
			for (int j=0;j<les[i].lee.size();j++) {
				int csel = 1-(j==t_slID);
				les[i].lee[j].prepare();
				les[i].lee[j].set_scroll(glm::translate(glm::mat4(1.0f),
						glm::vec3(-60,ofs+j*-25,0)));
				les[i].lee[j].render(128,glm::vec4(1,1-csel,1-csel,1));
			}
		} // FIXME: remove branching from main loop!!!!

		// setting edge mod variable if head with slider attachment selected & delta slider state
		bool has_slider = (i==index)&&les[i].slide;
		les[i].sID += delta*(has_slider&&
			((delta>0&&les[i].sID<les[i].sl_max)||(delta<0&&les[i].sID>les[i].sl_min)));
		edge_mod += (1+(les[i].sID+les[i].sl_min)/(float)(les[i].sl_max+les[i].sl_min))*has_slider;
	} lf_open = rsl;

	// rendering the estimated difficulty impression banner
	bool is_diff = !!les[index].diff;
	if (les[index].diff&&dtrans>.75f) {
		m_r2d->prepare();

		// rendering belt colour
		m_r2d->upload_model(glm::mat4(1.0f));
		m_r2d->render_state(diffRID,glm::vec2(0,les[index].diff));

		// render belt description
		glm::mat4 trans_model = glm::translate(glm::mat4(1.0f),glm::vec3(1075,575,0));
		trans_model = glm::scale(trans_model,glm::vec3(val_scl,val_scl,0));
		trans_model = glm::rotate(trans_model,glm::radians(val_rot),glm::vec3(0,0,1));
		m_r2d->al[diffRID+1].model = trans_model;
		m_r2d->render_state(diffRID+1,glm::vec2(1,les[index].diff));

		// animate belt description movement
		neg_scl += (val_scl>1&&!neg_scl)-(val_scl<.5f&&neg_scl);
		val_scl += .02f*(1-2*neg_scl);
		neg_rot += (val_rot>27&&!neg_rot)-(val_rot<-27&&neg_rot);
		val_rot += (rand()%20)*.01f*(1-2*neg_rot);
	}

	// rendering the description text in white
	les[index].dtxt.prepare();
	les[index].dtxt.set_scroll(glm::translate(glm::mat4(1.0f),glm::vec3(0,-75*(is_diff),0)));
	les[index].dtxt.render(dtrans*1024,glm::vec4(1,1,1,1));
}

/*
	write_tempID(uint8_t)
	index: gets the selected menu list element
	purpose: writes the temporary selection id to the selected menu list entity id when called
*/
void MenuList::write_tempID(uint8_t index) { les[index].sID = t_slID; }

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
		6 := sublist has a list of possible target monitors with "<le_TargetMonitor>"
		7 := estimated difficulty of level/arcade/boss marked as "<diff>"
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
	return !strcmp(pcnt.c_str(),"head")
		+2*!strcmp(pcnt.c_str(),"dsc")
		+3*!strcmp(pcnt.c_str(),"le")
		+4*!strcmp(pcnt.c_str(),"slider")
		+5*!strcmp(pcnt.c_str(),"dest")
		+6*!strcmp(pcnt.c_str(),"le_TargetMonitor")
		+7*!strcmp(pcnt.c_str(),"diff");
	// ??maybe do this with case
}

/*
	split_arguments(std::string)
	ext: represents the extraction string
	sep: the character to split the line at
	purpose: getting an argument list from extraction string
*/
std::vector<std::string> MenuList::split_arguments(std::string ext,char sep)
{
	std::vector<std::string> args;
	std::string t_arg;
	for (int i=0;i<ext.size();i++) {
		if (ext[i]==sep) {
			args.push_back(t_arg);
			t_arg = "";
		} else t_arg += ext[i];
	} args.push_back(t_arg);
	return args;
} // TODO: optimize

/*
	translate_index(std::vector<std::string>,std::string)
	lev: holds the value flag list to compare with
	arg: the extracted value to translate to index value
	purpose: translates the read value from config into index integer
*/
uint32_t MenuList::translate_index(std::vector<std::string> lev,std::string arg)
{
	for (int i=0;i<lev.size();i++) {
		if (lev[i]==arg) return i;
	} return 0;
}
