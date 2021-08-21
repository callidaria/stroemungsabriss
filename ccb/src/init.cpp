#include "../fcn/init.h"

Init::Init(const char* path)
{
	std::ifstream file(path,std::ios::in);
	while (!file.eof()) {
		char* lh = new char[128];file >> lh;
		const char* rclh = reinterpret_cast<char*>(&lh[0]);
		char* val = new char[64];file >> val;
		iConfig[find_iKey(rclh)] = atoi(val);
	}
}
uint32_t Init::rINT(uint32_t id) { return iConfig[id]; }
uint32_t Init::find_iKey(const char* key)
{
	int out = 0;
	while (out<INTEGER_KEY_LENGTH) {
		if (!strcmp(key,iKeys[out])) break;
		out++;
	} return out;
}
