#ifndef CCB_MATHEMATICS_CAMERA3D
#define CCB_MATHEMATICS_CAMERA3D

#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Camera3D
{
public:

	// construction
	Camera3D(float w_res,float h_res);
	Camera3D(glm::vec3 cp,float w_res,float h_res,float fov);
	Camera3D(glm::vec3 cp,glm::vec3 front,float w_res,float h_res,float fov);
	Camera3D(glm::vec3 cp,float w_res,float h_res,float fov,float pitch,float yaw);

	// calculations
	inline void update() { view3D = glm::lookAt(pos,pos+front,up); }

public:

	glm::vec3 pos,front = glm::vec3(0,0,-1),up = glm::vec3(0,1,0);
	glm::mat4 view3D,proj3D;
};

#endif
