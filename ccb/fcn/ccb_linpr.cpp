#include "ccb_linpr.h"

/*
	constructor(Renderer2D*,const char*)
	r2d: points to 2D renderer handling all sprite & anim creation requests
	path: path to environment cascabel file holding all creation requests & parameters
	purpose: saves path & renderer for later usage
*/
CCBLInterpreter::CCBLInterpreter(Renderer2D* r2d,const char* path)
	: m_r2d(r2d),lvpath(path) {  }

/*
	load_level() -> uint16_t
	purpose: process all creation requests held by the cascabel environment file
	returns: starting draw address for 2D renderer
*/
uint16_t CCBLInterpreter::load_level()
{
	// setup
	int out = m_r2d->sl.size();
	std::ifstream lvfile(lvpath,std::ios::in);
	std::string tline;
	std::string args[CCBArgs::ARGUMENT_COUNT];

	// interpret level file
	while (getline(lvfile,tline)) {
		std::stringstream ssline(tline);

		// extract arguments
		uint8_t arg_index = 0;
		std::string arg;
		while (std::getline(ssline,arg,' ')) args[arg_index++] = arg;

		// pattern for spritedef: lh position_x position_y width height texpath
		if (args[CCBArgs::COMMAND]=="sprite:") {

			// extract values
			glm::vec2 pos = glm::vec2(stoi(args[CCBArgs::POS_X]),stoi(args[CCBArgs::POS_Y]));
			float width = stoi(args[CCBArgs::WIDTH]),height = stoi(args[CCBArgs::HEIGHT]);
			char* tex_path = new char[args[CCBArgs::TEXPATH].length()+1];
			strcpy(tex_path,args[CCBArgs::TEXPATH].c_str());

			// save values & create sprite
			InterpreterSpriteData proc = {
				.position = pos,
				.width = width,
				.height = height,
				.texpath = tex_path
			};
			sprite_data.push_back(proc);
			m_r2d->add(pos,width,height,tex_path);
		}

		// pattern for animdef: lh position_x position_y width height texpath rows columns
		//		frames timesplit
		else if (args[CCBArgs::COMMAND]=="anim:") {

			// extract values
			glm::vec2 pos = glm::vec2(stoi(args[CCBArgs::POS_X]),stoi(args[CCBArgs::POS_Y]));
			float width = stoi(args[CCBArgs::WIDTH]),height = stoi(args[CCBArgs::HEIGHT]);
			char* tex_path = new char[args[CCBArgs::TEXPATH].length()+1];
			strcpy(tex_path,args[CCBArgs::TEXPATH].c_str());
			uint8_t r = stoi(args[CCBArgs::REPEAT]),
				c = stoi(args[CCBArgs::COUNT]),
				f = stoi(args[CCBArgs::FRAMES]),
				t = stoi(args[CCBArgs::TICKS]);

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
			m_r2d->add(pos,width,height,tex_path,r,c,f,t);
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

/*
	delete_level() -> void
	purpose: manually remove all texture paths from allocated memory
*/
void CCBLInterpreter::delete_level()
{
	/*for (const char* tex : m_tex) delete[] tex;
	for (const char* tex : a_tex) delete[] tex;*/
	// TODO
}
// TODO: completely unload all of the level from memory