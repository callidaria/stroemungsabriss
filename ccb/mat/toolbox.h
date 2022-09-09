#pragma once

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

class Toolbox
{
public:
	static std::vector<float> create_sprite_canvas(glm::vec2 pos,float width,float height);
};