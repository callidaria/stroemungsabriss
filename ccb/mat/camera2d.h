#ifndef CCB_MATHEMATICS_CAMERA2D
#define CCB_MATHEMATICS_CAMERA2D

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera2D
{
public:

	Camera2D(float w_res,float h_res);

public:

	glm::mat4 view2D,proj2D;
};

#endif