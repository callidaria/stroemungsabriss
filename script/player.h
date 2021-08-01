#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include "../ccb/gfx/renderer2d.h"

class Player
{
public:
	Player() {  }
	Player(Renderer2D* r2d);
	~Player();
	void update();
private:
	Renderer2D* m_r2d;
	int ri;
	glm::vec2 pos = glm::vec2(200,200);
};
