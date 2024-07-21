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

inline Camera2D g_Camera2D = Camera2D(1280.f,720.f);
// TODO: relocate

#endif
