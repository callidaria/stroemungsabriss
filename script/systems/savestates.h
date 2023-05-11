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
};

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

	uint64_t read_bitrange(std::bitset<64> data,uint8_t start,uint8_t end);

public:

	// saves
	std::vector<SaveData> saves;
};