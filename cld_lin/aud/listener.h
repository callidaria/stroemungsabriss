#pragma once

#include <iostream>
#include <AL/al.h>
#include <glm/glm.hpp>

class Listener
{
public:
	Listener(glm::vec3 pos=glm::vec3(0.0f),glm::vec3 vel=glm::vec3(0.0f),glm::vec3 ori=glm::vec3(0,0,-1));

	// setters
	void set_position(glm::vec3 pos);
	void set_velocity(glm::vec3 vel);
	void set_orientation(glm::vec3 ori);
};
