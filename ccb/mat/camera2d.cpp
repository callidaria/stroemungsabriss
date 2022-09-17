#include "camera2d.h"

/*
    constructor(float,float)
    [...]
*/
Camera2D::Camera2D(float w_res,float h_res)
{
	view2D = glm::lookAt(glm::vec3(0,-0.01f,1),glm::vec3(),glm::vec3(0,0,1));
	proj2D = glm::ortho(0.0f,w_res,0.0f,h_res,0.1f,10.0f);
}