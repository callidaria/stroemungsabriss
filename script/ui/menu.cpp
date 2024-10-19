#include "menu.h"

/*
	TODO
*/
MainMenu::MainMenu()
{
	// asset load
	//g_Renderer.load("./lvload/main_menu.ccb",0);
	g_Renderer.write_text(0,
			"yomisensei by callidaria. danmaku v"+std::to_string(INFO_VERSION_RELEASE)
			+ '.'+std::to_string(INFO_VERSION_SUBRELEASE)
			+ '.'+std::to_string(INFO_VERSION_DEVSTEP)+INFO_VERSION_MODE_SUFFIX
			+ " - running on cascabel base (OpenGL)",
			glm::vec2(-10,10),.125f,glm::vec3(.4f),
			TEXT_ALIGN_BOTTOMRIGHT);
	// TODO: write version and input request text
	// TODO: peripheral sensitive input request annotations
	// TODO: write main menu options text

	// selection splices
	// TODO

	// interaction setup
	// lists
	// TODO
	// TODO: enumerate available screens

	// dialogues
	// TODO
}

/*
	TODO
*/
void MainMenu::update()
{
	// TODO
}
