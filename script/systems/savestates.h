#pragma once

#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

#include "../struct/feature_base.h"
#include "../../ccb/fcn/text.h"

// data structure for save information
struct SaveData
{
	const char* title,*description;
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

public:

	// saves
	std::vector<SaveData> saves;
};