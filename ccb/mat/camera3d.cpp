#include "camera3d.h"

/*
	constructor(float,float)
	[...]
*/
Camera3D::Camera3D(float w_res,float h_res)
{
	view3D = glm::lookAt(glm::vec3(0,-0.01f,1),glm::vec3(),glm::vec3(0,0,1));
	proj3D = glm::ortho(0.0f,w_res,0.0f,h_res,0.1f,10000.0f);
}
// FIXME: duplicate code

/*
	constructor(vec3,float,float,float)
	[...]
*/
Camera3D::Camera3D(glm::vec3 cp,float w_res,float h_res,float fov)
	: pos(cp)
{
	front = glm::vec3(0.0f,0.0f,-1.0f);up=glm::vec3(0.0f,1.0f,0.0f);
	view3D = glm::lookAt(pos,pos+front,up);
	proj3D = glm::perspective(glm::radians(fov),w_res/h_res,0.1f,10000.0f);
}

/*
	update() -> void
	[...]
*/
void Camera3D::update()
{
	view3D = glm::lookAt(pos,pos+front,up);
}