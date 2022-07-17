#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/mat/camera2d.h"
#include "../../ccb/gfx/shader.h"

class Healthbar
{
public:
    Healthbar(glm::vec2 pos,uint16_t width,uint16_t height,uint16_t hp);
    ~Healthbar();

    void render();

    void register_damage(uint16_t dmg);
private:
    Buffer hpbuffer = Buffer();
    Shader shp = Shader();
    uint16_t max_width,max_hp,curr_hp;
};