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
		boolean_list_byte_overflow(clp[0],clp[1],0,save.tiles_numbers,9);
		boolean_list_byte_overflow(clp[1],clp[2],1,save.tiles_circles,9);
		boolean_list_byte_overflow(clp[2],clp[3],2,save.tiles_bamboo,9);
		boolean_list_byte_overflow(clp[3],clp[4],3,save.tiles_winddragon,7);
		boolean_list_byte_overflow(clp[4],clp[5],2,save.tiles_flowerseason,8);
		byte_to_booleans(sdata[9],save.pcbkey);

		// list save data
		saves.push_back(save);
	} file.close();
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
		file << booleans_to_byte(save.tiles_numbers)
				<< to_byte_list_overflow(save.tiles_numbers,save.tiles_circles,8,9)
				<< to_byte_list_overflow(save.tiles_circles,save.tiles_bamboo,7,9)
				<< to_byte_list_overflow(save.tiles_bamboo,save.tiles_winddragon,6,9)
				<< to_byte_list_overflow(save.tiles_winddragon,save.tiles_flowerseason,5,7)
				<< booleans_to_byte(save.tiles_flowerseason,0x00,2,7,6)
				<< booleans_to_byte(save.pcbkey);

		// end writing for current save file
		file << '\n';
	} file.close();
}

/*
	booleans_to_byte(bool*,unsigned char,uint8_t,uint8_t,uint8_t) -> unsigned char (private,static)
	\param xs: boolean list to compress into byte
	\param obyte: origin byte to bitwise shift booleans from list into
	\param range: list copy range
	\param sstart: starting bit to shift first list value to
	\param lstart: starting list element to crunch into byte
	\returns byte holding crunched boolean values
*/
unsigned char SaveStates::booleans_to_byte(bool* xs,unsigned char obyte,uint8_t range,uint8_t sstart,
		uint8_t lstart)
{
	for (uint8_t i=0;i<range;i++) obyte |= xs[lstart+i]<<(sstart-i);
	return obyte;
}

/*
	to_byte_list_overflow(bool*,bool*,uint8_t,size_t) -> unsigned char (private,static)
	\param xs: boolean list, that is guaranteed to overflow within byte range
	\param ys: boolean list, that will overtake bytecrunching process after first list overflow
	\param xstart: starting array index for first boolean list
	\param xrange: array size of the first boolean list
	\returns fully crunched byte, as result from two lists in sequence order
*/
unsigned char SaveStates::to_byte_list_overflow(bool* xs,bool* ys,uint8_t xstart,size_t xrange)
{
	uint8_t xcp = xrange-xstart;
	unsigned char out = booleans_to_byte(xs,0x00,xcp,7,xstart);
	return booleans_to_byte(ys,out,8-xcp,7-xcp);
}

/*
	byte_to_booleans(unsigned char,bool[8]) -> void (private,static)
	purpose: copies all bitstates from given byte to a size 8 array, 0 being the leftmost bit
	\param byte: byte to bitwise convert to boolean list of size 8
	\param out: output array to copy bitwise values from byte to
*/
void SaveStates::byte_to_booleans(unsigned char byte,bool out[8])
{ for (uint8_t i=0;i<8;i++) out[7-i] = byte&(0x01<<i); }

/*
	boolean_list_byte_overflow(bool[8],bool[8],uint8_t,bool*,size_t) -> void (private,static)
	purpose: map booleanized bytes to an array, expecting list overflow
	\param byte0: boolean list, expecting to overflow, while copying
	\param byte1: boolean list to continue copy process from after overflow
	\param bstart: starting list index for first boolean list
	\param ls: list to copy the boolean values to
	\param ls_range: range of bits (here listed booleans) to copy to given list array ls
*/
void SaveStates::boolean_list_byte_overflow(bool byte0[8],bool byte1[8],uint8_t bstart,bool* ls,
		size_t ls_range)
{
	// iterate first byte, with expectation of byte overflow
	uint8_t idx = 0;
	for (uint8_t i=bstart;i<8;i++) {
		ls[idx] = byte0[i];
		idx++;
	}

	// iterate second byte until range condition
	uint8_t i = 0;
	while (idx+i<ls_range) {
		ls[idx+i] = byte1[i];
		i++;
	}
}