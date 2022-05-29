#include "../fcn/init.h"

Init::Init(const char* path)
{
	std::ifstream file(path,std::ios::in);
	if (!file) {
		std::ifstream tplate("res/config.ini",std::ios::in);
		std::ofstream trget("config.ini",std::ios::app);
		while (!tplate.eof()) {
			std::string line;
			getline(tplate,line);
			trget << line << '\n';
		} file = std::ifstream(path,std::ios::in);
		/*std::vector<std::string> wkeys,wvals;
		std::vector<const char*> files = {
			"lvload/ml_optaud","lvload/ml_optfrm",
			"lvload/ml_optgfx","lvload/ml_optgm"
		};
		for (const char* path : files) {
			std::ifstream lvfile(path,std::ios::in);
			while (!lvfile.eof()) {
				bool assess=false,read=false;
				std::string mode;
				std::string line;
				getline(lvfile,line);
				for (char ltr : line) {
					assess = ltr=='<'||(ltr!='>'&&assess);
					if (assess&&ltr!='<') mode += ltr;
					else if (ltr=='>') {
						if (!strcmp(mode.c_str(),"dest")) read = true;
					}
				}
			}
		}
		std::ofstream nconf("config.ini",std::ios::app);*/
		// TODO: write default init file automatically
		// FIXME: autocreation through MenuList worked accidentally while testing
		// 		i do not know if this could be used to optimize the init creation
	}
	while (!file.eof()) {
		std::string rclh;file >> rclh;
		std::string val;file >> val;
		std::vector<uint32_t> ex = read_cartesian(val);
		uint16_t j = find_iKey(rclh.c_str());
		for (int i=0;i<ex.size();i++) {
			iConfig[j+i] = ex[i];
		}
	}
} // FIXME: find a faster way to do this

uint32_t Init::rINT(uint32_t id) { return iConfig[id]; }

uint32_t Init::find_iKey(const char* key)
{
	int out = 0;
	while (out<INTEGER_KEY_LENGTH) {
		if (!strcmp(key,iKeys[out])) break;
		out++;
	} return out;
}

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
