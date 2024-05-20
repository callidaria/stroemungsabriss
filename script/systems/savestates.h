#ifndef SCR_SYSTEMS_SAVESTATES
#define SCR_SYSTEMS_SAVESTATES

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>

#include "../../ccb/core.h"
#include "../../ccb/fcn/text.h"


// data structure for save information
struct SaveData
{
	std::string title,description;
	LoadInstruction ld_inst;
	uint8_t diff,skill;
	bool tiles_numbers[9],tiles_circles[9],tiles_bamboo[9];
	bool tiles_winddragon[7],tiles_flowerseason[8];
	bool pcbkey[8];
};

/*
	save file data structure (newlines only for eyecandy purposes):
	title;description;
	8-bit <ld_inst> 8-bit <diff> 8-bit <skill>
	6-byte <tiles>(free 6-bit) 8-bit <pcbkey>
*/

class SaveStates
{
public:

	// construction
	SaveStates();
	~SaveStates() {  }

	// read & write
	void read_savefile();
	void write_savefile();

private:

	// bitwise helper
	static unsigned char booleans_to_byte(bool* xs,unsigned char obyte=0x00,uint8_t range=8,
			uint8_t sstart=7,uint8_t lstart=0);
	static unsigned char to_byte_list_overflow(bool* xs,bool* ys,uint8_t xstart,size_t xrange);
	static void byte_to_booleans(unsigned char byte,bool out[8]);
	static void boolean_list_byte_overflow(bool byte0[8],bool byte1[8],uint8_t bstart,bool* ls,
			size_t range);

public:

	// saves
	std::vector<SaveData> saves;
};

#endif
