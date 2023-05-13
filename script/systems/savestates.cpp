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

		// information interpretation
		save.ld_inst = (LoadInstruction)sdata[0];
		save.diff = sdata[1];
		save.skill = sdata[2];

		// bitwise collectibles
		bool clp[6][8];
		for (uint8_t i=0;i<6;i++) byte_to_booleans(sdata[3+i],clp[i]);
		save.tiles_numbers[0] = clp[0][0],save.tiles_numbers[1] = clp[0][1],
		save.tiles_numbers[2] = clp[0][2],save.tiles_numbers[3] = clp[0][3],
		save.tiles_numbers[4] = clp[0][4],save.tiles_numbers[5] = clp[0][5],
		save.tiles_numbers[6] = clp[0][6],save.tiles_numbers[7] = clp[0][7],
		save.tiles_numbers[8] = clp[1][0];
		save.tiles_circles[0] = clp[1][1],save.tiles_circles[1] = clp[1][2],
		save.tiles_circles[2] = clp[1][3],save.tiles_circles[3] = clp[1][4],
		save.tiles_circles[4] = clp[1][5],save.tiles_circles[5] = clp[1][6],
		save.tiles_circles[6] = clp[1][7],save.tiles_circles[7] = clp[2][0],
		save.tiles_circles[8] = clp[2][1];
		save.tiles_bamboo[0] = clp[2][2],save.tiles_bamboo[1] = clp[2][3],
		save.tiles_bamboo[2] = clp[2][4],save.tiles_bamboo[3] = clp[2][5],
		save.tiles_bamboo[4] = clp[2][6],save.tiles_bamboo[5] = clp[2][7],
		save.tiles_bamboo[6] = clp[3][0],save.tiles_bamboo[7] = clp[3][1],
		save.tiles_bamboo[8] = clp[3][2];
		save.tiles_winddragon[0] = clp[3][3],save.tiles_winddragon[1] = clp[3][4],
		save.tiles_winddragon[2] = clp[3][5],save.tiles_winddragon[3] = clp[3][6],
		save.tiles_winddragon[4] = clp[3][7],save.tiles_winddragon[5] = clp[4][0],
		save.tiles_winddragon[6] = clp[4][1];
		save.tiles_flowerseason[0] = clp[4][2],save.tiles_flowerseason[1] = clp[4][3],
		save.tiles_flowerseason[2] = clp[4][4],save.tiles_flowerseason[3] = clp[4][5],
		save.tiles_flowerseason[4] = clp[4][6],save.tiles_flowerseason[5] = clp[4][7],
		save.tiles_flowerseason[6] = clp[5][0],save.tiles_flowerseason[7] = clp[5][1];
		byte_to_booleans(sdata[9],save.pcbkey);

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

/*
	TODO
*/
void SaveStates::byte_to_booleans(unsigned char byte,bool out[8])
{ for (uint8_t i=0;i<8;i++) out[7-i] = byte&(0x01<<i); }