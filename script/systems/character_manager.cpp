#include "character_manager.h"

/*
	TODO
*/
CharacterManager::CharacterManager() {  }

/*
	TODO
*/
void CharacterManager::add_character(uint16_t id,const char* name)
{
	CharacterData proc = { id,name };
	char_list.push_back(proc);
}

/*
	TODO
*/
CharacterData CharacterManager::get_character(uint16_t id)
{
	for (auto chr:char_list) {
		if (chr.id==id) return chr;
	} return { id,"error: unused character id" };
}