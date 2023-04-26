#pragma once

#include <iostream>
#include <vector>

#include "feature_base.h"

struct SaveState
{
	const char* title,*description;
	LoadInstruction ld_inst;
	uint8_t diff,skill;
};