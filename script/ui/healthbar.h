#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/mat/camera2d.h"
#include "../../ccb/gfx/shader.h"

class Healthbar
{
public:
    Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,std::vector<uint8_t> phases,
            std::vector<uint16_t> hp);
    ~Healthbar();

    void render();

    void register_damage(uint16_t dmg);
private:
    uint16_t combine_hp();
private:
    Buffer hpbuffer = Buffer();
    Shader shp = Shader();
    std::vector<uint8_t> hb_phases;
    std::vector<uint16_t> hp_list;
    uint16_t max_width,max_hp,curr_hp=0;
    uint8_t hp_bars,cphase=0,chb=0,fill_cooldown=241;
};
