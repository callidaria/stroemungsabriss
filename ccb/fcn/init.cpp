#include "init.h"

/*
	Constructor(const char*)
	path: path to the config.ini file
	purpose: read config values from file. If the expected file does not exists,
		it is created with standard values and read afterwards.
*/
Init::Init(const char* path)
{
	// reading file
	std::ifstream file(path,std::ios::in);

	// create config file if non existent
	if (!file) {
		// copy standard config file
		std::ifstream tplate("res/config.ini",std::ios::in);
		std::ofstream trget(path,std::ios::app);
		while (!tplate.eof()) {
			std::string line;
			getline(tplate,line);
			trget << line << '\n';
		} file = std::ifstream(path,std::ios::in); // read created file

		// TODO: ??write default init file automatically based on descriptions in .ldc files
		// FIXME: autocreation through MenuList worked accidentally while testing? wtf?
		// 		i do not know if this could be used to maybe optimize the init creation?
	}

	// extract values from config file
	while (!file.eof()) {
		std::string rclh; file >> rclh;
		std::string val; file >> val;
		uint16_t j = find_iKey(rclh.c_str());
		if (j!=InitVariable::VARIABLE_ERROR) {
			uint32_t ex = stoi(val);
			iConfig[j] = ex;
		}
	}
}
// FIXME: find a faster way to do this
// TODO: implement a more stable solution and use error checking for message return
// TODO: try/catch stoi exceptions

/*
	find_iKey(const char*) -> uint32_t
	key: key string as referenced by in menu list file destinations
	returns: converts string form of key into the key id integer and returns it
*/
uint16_t Init::find_iKey(const char* key)
{
	uint16_t out = 0;
	while (out<InitVariable::VARIABLE_KEY_LENGTH) {
		if (!strcmp(key,iKeys[out].name)) break;
		out++;
	} return out;
}

/*
	!O(n) .initialization variables /function -> (public)
	purpose: write all variables back to config file
*/
void Init::write_changes()
{
	// handle frame resolution preset translation to vector
	iConfig[InitVariable::FRAME_RESOLUTION_WIDTH]
			= resolutionWidthPresets[iConfig[InitVariable::FRAME_RESOLUTION_PRESET]];
	iConfig[InitVariable::FRAME_RESOLUTION_HEIGHT]
			= resolutionHeightPresets[iConfig[InitVariable::FRAME_RESOLUTION_PRESET]];

	// write changes
	std::ofstream chwrite("./config.ini",std::ios::out);
	for (uint32_t i=0;i<InitVariable::VARIABLE_KEY_LENGTH;i++)
		chwrite << iKeys[i].name << ' ' << std::to_string(iConfig[i]) << '\n';
	chwrite.close();
}

// FIXME: OUTDATED WRITING & CONCEPTS! UPDATE THIS IN THE FUTURE
