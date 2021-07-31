#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Player
{
public:
	Player();
	~Player();
	void update();
private:
	glm::vec2 pos = glm::vec2(615,40);
};
