#pragma once

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D
{
public:

	// Camera3D() {  }
	Camera3D(glm::vec3 cp,float w_res,float h_res,float fov);

	void update();

public:

	glm::vec3 pos,front,up;
	glm::mat4 view3D,proj3D;
};