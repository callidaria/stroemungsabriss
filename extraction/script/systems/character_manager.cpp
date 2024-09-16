#include "character_manager.h"

/*
	add_character(uint16_t,const char*,const char*,uint8_t,vec4) -> void
	id: id to refer to character by conversation & potentially other systems
	name: readable display name of character
	texpath: path to portrait spritesheet texture
	cols: amount of columns on spritesheet
	text_colour: rgba colour the spoken text of the character is written in
	purpose: adds a character to character list, so systems can interact with it,
		spritesheet holds a single row of portraits in 100x400 pixels, displaying different moods
*/
void CharacterManager::add_character(uint16_t id,const char* name,const char* texpath,uint8_t cols,
		glm::vec4 text_colour)
{
	GLuint tex;
	glGenTextures(1,&tex);

	CharacterData proc = { id,name,texpath,cols,tex,text_colour };
	char_list.push_back(proc);
}

/*
	load_spritesheets() -> void
	purpose: load spritesheets for all characters
*/
void CharacterManager::load_spritesheets()
{
	for (uint16_t i=0;i<char_list.size();i++) 
		Toolbox::load_texture(char_list[i].tex_sheet,char_list[i].texpath);
}

/*
	get_character(uint16_t) -> CharacterData
	id: character id as search reference
	returns: character, related to the given id
*/
CharacterData CharacterManager::get_character(uint16_t id)
{
	for (auto chr:char_list) {
		if (chr.id==id) return chr;
	} return { id,"error: unused character id" };
}