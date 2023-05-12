#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <bitset>
#include <sstream>
#include <fstream>

#include "../struct/feature_base.h"
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

	static unsigned char booleans_to_byte(bool* xs,unsigned char out=0x00,uint8_t range=8,
			uint8_t sstart=7,uint8_t lstart=0);

public:

	// saves
	std::vector<SaveData> saves;
};