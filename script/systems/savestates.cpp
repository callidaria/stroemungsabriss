#include "savestates.h"

/*
	constructor()
	purpose: savestate information loading and setup for interaction
*/
SaveStates::SaveStates()
{ read_savefile(); }

/*
	read_savefile() -> void
	purpose: reads save file and refills save data information list
*/
void SaveStates::read_savefile()
{
	// process each save
	std::ifstream file("./dat/saves");
	std::string datastring;
	saves.clear();
	while (std::getline(file,datastring)) {

		// data setup
		SaveData save = {  };
		std::istringstream data(datastring);

		// grind save data
		std::string curr_data;
		std::vector<std::string> raw_savedata;
		while (std::getline(data,curr_data,';')) raw_savedata.push_back(curr_data);

		// interpret save file
		save.title = raw_savedata[0].c_str();
		save.description = raw_savedata[1].c_str();

		// bitwise interpretations
		std::bitset<64> bwsdata(std::stoi(raw_savedata[2]));
		save.ld_inst = (LoadInstruction)read_bitrange(bwsdata,32,16);
		save.diff = read_bitrange(bwsdata,16,8);
		save.skill = read_bitrange(bwsdata,8,0);

		// list save data
		saves.push_back(save);
	}
}

/*
	write_savefile() -> void
	purpose: write current save data information to save file
*/
void SaveStates::write_savefile()
{
	std::ofstream file("./dat/saves");
	for (auto save : saves) {
		file << save.title << ';';
		file << save.description << ';';
		std::bitset<64> bwsdata(save.ld_inst<<16|save.diff<<8|save.skill);
		file << bwsdata.to_ulong() << '\n';
	} file.close();
}

/*
	read_bitrange(bitset<64>,uint8_t,uint8_t) -> uint64_t
	purpose: read save data within bitrange, 64 being the leftmost, 0 being the rightmost bit
	\param data: save data bit sequence, which is to be processed
	\param start: start of bitrange, for start>end => start is left of end
	\param end: end of bitrange, for end<start => end is right of start
	\returns interpreted value from data bit sequence within bitrange
*/
uint64_t SaveStates::read_bitrange(std::bitset<64> data,uint8_t start,uint8_t end)
{
	std::bitset<64> out;
	for (uint8_t i=end;i<start;i++) out[i] = data[i];
	out >>= end;
	return out.to_ulong();
}