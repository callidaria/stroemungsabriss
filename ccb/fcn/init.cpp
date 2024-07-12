#include "init.h"

/*
	!O(nlog(n)) .length of config file /load -> (static)
	purpose: read config file and extract variable values for program initialization
*/
void Init::run_init()
{
	// attempting to read the config file
	const char* path = "config.ini";
	std::ifstream file(path,std::ios::in);

	// copy standard config file if non existent and open created default
	if (!file)
	{
		Toolbox::copy_file("./res/config.ini",path);
		file = std::ifstream(path,std::ios::in);
	}

	// extract values from config file
	while (!file.eof())
	{
		std::string key,value;
		file >> key, file >> value;

		// correlate key literal with its memory index
		iConfig[key] = stoi(value);
	}

	// close config file
	file.close();
}
// TODO: try/catch stoi exceptions

/*
	!O(n) .initialization variables /function -> (public)
	purpose: write all variables back to config file
*/
void Init::write_changes()
{
	// handle frame resolution preset translation to vector
	iConfig[RESOLUTION_WIDTH_IDENTIFIER] = resolutionWidthPresets[iConfig[RESOLUTION_PRESET_IDENTIFIER]];
	iConfig[RESOLUTION_HEIGHT_IDENTIFIER] = resolutionHeightPresets[iConfig[RESOLUTION_PRESET_IDENTIFIER]];

	// write changes to config file
	std::ofstream chwrite("./config.ini",std::ios::out);
	for (auto& e : iConfig) chwrite << e.first << ' ' << std::to_string(e.second) << '\n';

	// close config file
	chwrite.close();
}
