#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>

#include "../../ccb/mat/toolbox.h"

// character data representation to maybe be later moved to class
struct CharacterData
{
    uint16_t id;
    const char* name;
    const char* texpath;
    uint8_t cols;
    GLuint tex_sheet;
};

class CharacterManager
{
public:

    // construction
    CharacterManager();
    ~CharacterManager() {  }

    // creation
    void add_character(uint16_t id,const char* name,const char* texpath,uint8_t cols);
    void load_spritesheets();

    // getter
    CharacterData get_character(uint16_t id);

private:

    // character list
    std::vector<CharacterData> char_list;
};