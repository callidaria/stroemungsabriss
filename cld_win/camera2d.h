#pragma once

#include <iostream>
#include <glm/glm.hpp>

class Camera2D
{
public:
	Camera2D()
	{
		view2D=glm::lookAt(glm::vec3(0,-0.01f,1),glm::vec3(),
				glm::vec3(0,0,1));
		proj2D=glm::ortho(0.0f,1280.0f,0.0f,720.0f,0.1f,10.0f);
	}
public:
	glm::mat4 view2D,proj2D;
};
