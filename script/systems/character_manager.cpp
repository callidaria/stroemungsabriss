#include "character_manager.h"

/*
	TODO
*/
CharacterManager::CharacterManager() {  }

/*
	TODO
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
	TODO
*/
void CharacterManager::load_spritesheets()
{
	for (uint16_t i=0;i<char_list.size();i++) 
		Toolbox::load_texture(char_list[i].tex_sheet,char_list[i].texpath);
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