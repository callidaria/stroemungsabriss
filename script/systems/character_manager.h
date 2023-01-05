#pragma once

#include <iostream>
#include <vector>

// character data representation to maybe be later moved to class
struct CharacterData
{
    uint16_t id;
    const char* name;
};

class CharacterManager
{
public:

    // construction
    CharacterManager();
    ~CharacterManager() {  }

    // creation
    void add_character(uint16_t id,const char* name);

    // getter
    CharacterData get_character(uint16_t id);

private:

    // character list
    std::vector<CharacterData> char_list;
};