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
		std::string rclh;file >> rclh;
		std::string val;file >> val;
		std::vector<uint32_t> ex = read_cartesian(val);
		uint16_t j = find_iKey(rclh.c_str());
		for (int i=0;i<ex.size();i++) iConfig[j+i] = ex[i];
	}
}
// FIXME: find a faster way to do this

/*
	find_iKey(const char*) -> uint32_t
	key: key string as referenced by in menu list file destinations
	returns: converts string form of key into the key id integer and returns it
*/
uint32_t Init::find_iKey(const char* key)
{
	uint32_t out = 0;
	while (out<InitVariable::VARIABLE_KEY_LENGTH) {
		if (!strcmp(key,iKeys[out].name)) break;
		out++;
	} return out;
}
// TODO: find out why default 0 is preventing the program from terminating setup process

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

/*
	read_cartesian(std::string) -> std::vector<uint32_t>
	val: raw value, possibly containing string1xstring2xstring3 pattern
	returns: cartesian writing style gets converted into list of all factors.
		example: string1xstring2xstring3
			-> { string1, string2, string3 } as std::vector<uint32_t>
		the x contained in the string marks the seperation of contained values.
*/
std::vector<uint32_t> Init::read_cartesian(std::string val)
{
	std::string proc;
	std::vector<std::string> raw_out;
	for (int i=0;i<val.size();i++) {
		if (val[i]=='x') {
			raw_out.push_back(proc);
			proc = "";
		} else proc += val[i];
	} raw_out.push_back(proc);
	std::vector<uint32_t> out;
	for (int i=0;i<raw_out.size();i++) {
		if (raw_out[i].size()>0) out.push_back(std::stoi(raw_out[i]));
		// FIXME: delete check and find out why it was necessary
	}
	return out;
}

// FIXME: OUTDATED WRITING & CONCEPTS! UPDATE THIS IN THE FUTURE
