#include "ccb_linpr.h"

/*
	constructor(const char*)
	path: path to environment cascabel file holding all creation requests & parameters
	purpose: saves path & renderer for later usage
*/
CCBLInterpreter::CCBLInterpreter(const char* path)
	: lvpath(path) {  }

/*
	load_level() -> uint16_t
	purpose: process all creation requests held by the cascabel environment file
	returns: starting draw address for 2D renderer
*/
uint16_t CCBLInterpreter::load_level(uint8_t bfr_id)
{
	// setup
	int out = Core::gRenderer.bfr_sprite[bfr_id].sprites.size();
	std::ifstream lvfile(lvpath,std::ios::in);
	std::string tline;
	std::string args[CANM_ARGUMENT_COUNT];

	// interpret level file
	while (getline(lvfile,tline)) {
		std::stringstream ssline(tline);

		// extract arguments
		uint8_t arg_index = 0;
		std::string arg;
		while (std::getline(ssline,arg,' ')) args[arg_index++] = arg;

		// pattern for spritedef: lh position_x position_y width height texpath
		if (args[CSPR_COMMAND]=="sprite:") {

			// extract values
			glm::vec2 pos = glm::vec2(stoi(args[CSPR_POSX]),stoi(args[CSPR_POSY]));
			float width = stoi(args[CSPR_WIDTH]),height = stoi(args[CSPR_HEIGHT]);
			char* tex_path = new char[args[CSPR_TEXPATH].length()+1];
			strcpy(tex_path,args[CSPR_TEXPATH].c_str());

			// save values & create sprite
			InterpreterSpriteData proc = {
				.position = pos,
				.width = width,
				.height = height,
				.texpath = tex_path
			};
			sprite_data.push_back(proc);
			Core::gRenderer.add_sprite(bfr_id,pos,width,height,tex_path);
		}

		// pattern for animdef: lh position_x position_y width height texpath rows columns
		//		frames timesplit
		else if (args[CANM_COMMAND]=="anim:") {

			// extract values
			glm::vec2 pos = glm::vec2(stoi(args[CANM_POSX]),stoi(args[CANM_POSY]));
			float width = stoi(args[CANM_WIDTH]),height = stoi(args[CANM_HEIGHT]);
			char* tex_path = new char[args[CANM_TEXPATH].length()+1];
			strcpy(tex_path,args[CANM_TEXPATH].c_str());
			uint8_t r = stoi(args[CANM_REPEAT]),
				c = stoi(args[CANM_COUNT]),
				f = stoi(args[CANM_FRAMES]),
				t = stoi(args[CANM_TICKS]);

			// save values & create animation
			InterpreterAnimData proc = {
				.position = pos,
				.width = width,
				.height = height,
				.texpath = tex_path,
				.row = r,
				.column = c,
				.frames = f,
				.ticks = t
			};
			anim_data.push_back(proc);
			Core::gRenderer.add_sprite(bfr_id,pos,width,height,tex_path,r,c,f,t);
		}
	}
	return out;
}
// FIXME: duplicate code
// FIXME: maybe it is time to retire manager system as it is (it is not very useful)

/*
	write_level() -> void
	purpose: used by the developer console; writes changes to the creation list
*/
void CCBLInterpreter::write_level()
{
	// write mesh positions
	std::ofstream lvfile(lvpath,std::ios::out);
	for (InterpreterSpriteData sprite : sprite_data) {
		std::stringstream lvbuff;
		lvbuff << "sprite: "
				<< sprite.position.x << ' ' << sprite.position.y << ' '
				<< sprite.width << ' ' << sprite.height << ' '
				<< sprite.texpath << '\n';
		lvfile << lvbuff.str();
	}

	// write animation positions
	for (InterpreterAnimData anim : anim_data) {
		std::stringstream lvbuff;
		lvbuff << "anim: "
				<< anim.position.x << ' ' << anim.position.y << ' '
				<< anim.width << ' ' << anim.height << ' '
				<< anim.texpath << ' '
				<< anim.row << ' ' << anim.column << ' ' << anim.frames << ' ' << anim.ticks << '\n';
		lvfile << lvbuff.str();
	}
	lvfile.close();
}
