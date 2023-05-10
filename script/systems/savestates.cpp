#include "savestates.h"

/*
	TODO
*/
SaveStates::SaveStates()
{ read_savefile(); }

/*
	TODO
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
		save.ld_inst = (LoadInstruction)std::stoi(raw_savedata[2]);
		save.diff = std::stoi(raw_savedata[3]);
		save.skill = std::stoi(raw_savedata[4]);

		// list save data
		saves.push_back(save);
	}
}

/*
	TODO
*/
void SaveStates::write_savefile()
{
	for (auto save : saves) {
		std::ofstream file("./dat/saves");
		file << save.title << ';';
		file << save.description << ';';
		file << save.ld_inst << ';';
		file << save.diff << ';';
		file << save.skill << ';';
		file << '\n';
		file.close();
	}
}