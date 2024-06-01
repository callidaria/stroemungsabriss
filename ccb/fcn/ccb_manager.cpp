#include "ccb_manager.h"

/*
	constructor(Frame*,Renderer*)
	frame: frame entity, handling the relevant input for dev console
	renderer: renderer to handle loaded & added 2D entities
	cam2d: reference to 2D camera defining the coordinate system and perspective for renderer
	purpose: build a manager instance, for later usage of developer console and level loading
*/
CCBManager::CCBManager()
{
	// create developer mode label
	//Core::gRenderer.add_sprite(0,glm::vec2(1230,690),40,20,"./res/dev.png");

	// create developer console greetings and initial setup
	ct.add("Welcome to the CASCABEL shell",glm::vec2(750,console_y+20));
	int prog = ct.add('>',glm::vec2(750,console_y));

	// add line error message
	cl.add("lineerr",glm::vec2(700+prog,30));

	// load text instances
	ct.load();cl.load();
}

/*
	add_lv(const char*) -> uint16_t
	path: path to level environment file, specifying object loading
	purpose: loads a cascabel level environment file
	returns: start address of environment objects within 2D renderer memory
*/
uint16_t CCBManager::add_lv(uint8_t bfr_id,const char* path)
{
	// interpret level environment file
	CCBLInterpreter proc = CCBLInterpreter(path);
	int out = proc.load_level(bfr_id);

	// save to interpreter & renderer memory index list
	linpr.push_back(proc);
	index.push_back(out);
	return out;
}

/*
	dev_console(uint32_t&,bool&) -> void
	running: reference to runstate variable to possibly end program from console by setting it to 0
	dactive: reference to boolean holding if developer console should be activated & shown
	purpose: renders and breaths life into developer console
*/
void CCBManager::dev_console(bool &running,bool &dactive)
{
	// draw developer mode label
	/*Core::gRenderer.prepare_sprites();
	Core::gRenderer.render_sprite(0);*/

	// open or close console with comma if not opened or closed last frame
	if (Core::gFrame.kb.ka[SDL_SCANCODE_COMMA]&&!activeonsc) {
		activeonsc = true;	// TODO: initially set this to false and remove later trigger call

		// process
		if (dactive) { Core::gFrame.input_stop();Core::gFrame.tline.pop_back(); }
		else Core::gFrame.input_start();

		// negate console's state of openness
		dactive = !dactive;
	}

	// set if open was requested for next frame
	activeonsc = Core::gFrame.kb.ka[SDL_SCANCODE_COMMA];

	// object movement through user input
	if (mv) {

		// break movement at confirmation
		if (Core::gFrame.kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) mv = false;
		activeonentr = Core::gFrame.kb.ka[SDL_SCANCODE_RETURN];

		// move sprite according to mouse vector delta
		glm::vec2 deltamv = glm::vec2(Core::gFrame.mouse.mxfr-mlf.x,Core::gFrame.mouse.myfr-mlf.y);
		int ti = index[i_lv]+i_rf;
		//Core::gRenderer.sprites[ti].transform.translate(deltamv);
		linpr[i_lv].sprite_data[i_rf].position += deltamv;
		mlf = glm::vec2(Core::gFrame.mouse.mxfr,Core::gFrame.mouse.myfr);
	}

	// object scaling throught user input
	else if (scl) {

		// break scaling through user input
		if (Core::gFrame.kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) scl = false;
		activeonentr = Core::gFrame.kb.ka[SDL_SCANCODE_RETURN];
		// FIXME: duplicate code. reduce!

		// scale sprite according to mouse movement vector length
		deltascl += (Core::gFrame.mouse.myfr-mlf.y)*0.001f;
		int ti = index[i_lv]+i_rf;
		//Core::gRenderer.sprites[ti].transform.scale(deltascl,deltascl,glm::vec2(0));
		linpr[i_lv].sprite_data[i_rf].width = tmp_wscale*deltascl;
		linpr[i_lv].sprite_data[i_rf].height = tmp_hscale*deltascl;
		mlf = glm::vec2(Core::gFrame.mouse.mxfr,Core::gFrame.mouse.myfr);
	}

	// developer console is active and normal due to exclusion of the above cases
	else if (dactive) {

		// input confirmation and processing
		if (Core::gFrame.kb.ka[SDL_SCANCODE_RETURN]&&!activeonentr) {

			// reset confirmation frame trigger
			activeonentr = true;  // FIXME: not really necessary, is it?

			// add command to passive console text lines & scroll
			ct.add(Core::gFrame.tline.c_str(),glm::vec2(770,console_y));
			console_y -= 20;

			// split command & arguments in input string
			std::vector<std::string> args;
			std::string tmp = "";
			for (int i=0;i<Core::gFrame.tline.length();i++) {
				if (Core::gFrame.tline[i]==' ') { 
					args.push_back(tmp);
					tmp = "";
				}
				else tmp.push_back(Core::gFrame.tline[i]);
			}
			args.push_back(tmp);

			// program termination recognition
			if (args[0]=="exit") running = false;

			// sprite movement recognition
			else if (args[0]=="mv_sprite") {

				// check if valid argument list size
				if (args.size()!=3)
					ct.add("invalid number of arguments; should be 2",glm::vec2(750,console_y));
				else {

					// enable sprite position change mode
					mv = true;

					// save argument values & origin mouse coordinates to calculate delta from
					mlf = glm::vec2(Core::gFrame.mouse.mxfr,Core::gFrame.mouse.myfr);
					i_lv = std::stoi(args[1]);i_rf = std::stoi(args[2]);
				}
			}

			// sprite scaling recognition
			else if (args[0]=="scl_sprite") {

				// check if valid argument list size
				if (args.size()!=3)
					ct.add("invalid number of arguments; should be 2",glm::vec2(750,console_y));
				else {

					// enable sprite scaling mode
					scl = true;

					// save delta scaling variables & input arguments
					mlf = glm::vec2(Core::gFrame.mouse.mxfr,Core::gFrame.mouse.myfr);
					i_lv = std::stoi(args[1]);i_rf = std::stoi(args[2]);
					tmp_wscale = linpr[i_lv].sprite_data[i_rf].width;
					tmp_hscale = linpr[i_lv].sprite_data[i_rf].height;
				}
			}

			// request to save changes to .ccb files
			else if (args[0]=="write_changes") {
				for (int i=0;i<linpr.size();i++) linpr[i].write_level();
				ct.add("changes written",glm::vec2(750,console_y));
			}

			// identify all other commands as invalid
			else {
				std::string errline = "invalid command: \""+args[0]+"\"";
				ct.add(errline.c_str(),glm::vec2(750,console_y));
			}  // FIXME: make a dynamically changable command list with function jmp not branching

			// remove input line contents & autoscroll terminal
			Core::gFrame.tline = "";
			console_y -= 20;
			ct.add('>',glm::vec2(750,console_y));
			cscroll = glm::translate(cscroll,glm::vec3(0,40,0));
		}

		// prevent consecutive frame confirmation input
		activeonentr = Core::gFrame.kb.ka[SDL_SCANCODE_RETURN];

		// if mouse button is pressen while console active
		if (Core::gFrame.mouse.mb[0]&&!activeonmcl) {

			// catch mouse coordinates and print them
			std::string mouse_out
				= std::to_string(Core::gFrame.mouse.mxfr)+'x'+std::to_string(Core::gFrame.mouse.myfr);
			ct.add(mouse_out.c_str(),glm::vec2(770,console_y));

			// autoscroll after output
			console_y -= 20;
			cscroll = glm::translate(cscroll,glm::vec3(0,20,0));
		}

		// prevent consecutive frame mouse click printout
		activeonmcl = Core::gFrame.mouse.mb[0];

		// rewrite console input line
		cl.clear(); // ??maybe directly add and remove chars from text instead of rewrite
		cl.add(Core::gFrame.tline.c_str(),glm::vec2(770,30));

		// draw console text above input line & upload scroll
		ct.prepare();
		ct.set_scroll(cscroll);
		ct.render(1024,glm::vec4(0,.7f,0,1));

		// draw console input line
		cl.prepare();
		cl.render(Core::gFrame.tline.size(),glm::vec4(.7f,.7f,.2f,1));
	}
}
