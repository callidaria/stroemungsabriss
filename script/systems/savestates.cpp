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
		save.title = raw_savedata[0];
		save.description = raw_savedata[1];
		std::string sdata = raw_savedata[2];

		// bitwise data interpretation
		save.ld_inst = (LoadInstruction)sdata[0];
		save.diff = sdata[1];
		save.skill = sdata[2];

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
	// open save file to write
	std::ofstream file("./dat/saves");
	for (auto save : saves) {

		// write save data title & description
		file << save.title << ';' << save.description << ';';

		// write general savestate information
		unsigned char ldi = save.ld_inst,wdiff = save.diff,wskill = save.skill;
		file << ldi << wdiff << wskill << '\n';
	} file.close();
}