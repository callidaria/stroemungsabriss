#ifndef CCB_GRAPHICS_LIGHT2D
#define CCB_GRAPHICS_LIGHT2D

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "../gfx/shader.h"

struct light_spot { glm::vec2 pos;glm::vec2 dir;float icut;float ocut; };

#endif