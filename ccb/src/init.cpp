#include "../fcn/init.h"

Init::Init(const char* path)
{
	std::ifstream file(path,std::ios::in);
	if (!file) printf("no config file found!\n");
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
