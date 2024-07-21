#include "config.h"

/*
	!O(nlog(n)) .length of config file /load -> (static)
	purpose: read config file and extract variable values for program initialization
*/
Config::Config()
{
	// config file extraction
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

		// correlate key literal with its memory index & handle potential problems with value conversion
		try { iConfig[key] = stoi(value); }
		catch (std::exception const& ex) { iConfig[key] = 0; }
		// TODO: log mistaken value conversions in a future logging system
	}

	// close config file
	file.close();

	// config variable correlation
	// iterate config variables in datastructure
	uint32_t* addr = &g_Config.iFrameResolutionPreset;
	for (uint8_t i=0;i<AMOUNT_MAPPED_VARIABLES;i++) *(addr+i) = iConfig[VKEY_IDENTIFIER_MAP[i]];
}

/*
	!O(n) .initialization variables /function -> (public)
	purpose: write all variables back to config file
*/
void Config::write_changes()
{
	// frame resolution preset translation to vector
	g_Config.vFrameResolutionWidth = resolutionWidthPresets[g_Config.iFrameResolutionPreset];
	g_Config.vFrameResolutionHeight = resolutionHeightPresets[g_Config.iFrameResolutionPreset];

	// write changes to config file
	std::ofstream chwrite("./config.ini",std::ios::out);
	for (auto& e : iConfig) chwrite << e.first << ' ' << std::to_string(e.second) << '\n';

	// close config file
	chwrite.close();
}
