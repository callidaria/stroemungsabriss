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
		file << ldi << wdiff << wskill;

		// write collectible flags
		save.tiles_numbers[7] = true;
		save.tiles_numbers[8] = true;
		save.tiles_circles[0] = true;
		save.tiles_circles[8] = true;
		save.tiles_bamboo[4] = true;
		save.tiles_bamboo[6] = true;
		save.tiles_winddragon[0] = true;
		save.tiles_winddragon[5] = true;
		save.tiles_flowerseason[0] = true;
		save.tiles_flowerseason[6] = true;
		save.pcbkey[0] = false;
		save.pcbkey[1] = true;
		unsigned char ncb = booleans_to_byte(save.tiles_numbers,0x00,1,7,8);
		ncb = booleans_to_byte(save.tiles_circles,ncb,7,6);
		unsigned char cbb = booleans_to_byte(save.tiles_circles,0x00,2,7,7);
		cbb = booleans_to_byte(save.tiles_bamboo,cbb,6,5);
		unsigned char bdb = booleans_to_byte(save.tiles_bamboo,0x00,3,7,6);
		bdb = booleans_to_byte(save.tiles_winddragon,bdb,5,4);
		unsigned char dfb = booleans_to_byte(save.tiles_winddragon,0x00,2,7,5);
		dfb = booleans_to_byte(save.tiles_flowerseason,dfb,6,5);
		file << booleans_to_byte(save.tiles_numbers) << ncb << cbb << bdb << dfb
				<< booleans_to_byte(save.tiles_flowerseason,0x00,2,7,6);
		file << booleans_to_byte(save.pcbkey);

		// end writing for current save file
		file << '\n';
	} file.close();
}

/*
	TODO
*/
unsigned char SaveStates::booleans_to_byte(bool* xs,unsigned char out,uint8_t range,uint8_t sstart,
		uint8_t lstart)
{
	for (uint8_t i=0;i<range;i++) out |= xs[lstart+i]<<(sstart-i);
	return out;
}