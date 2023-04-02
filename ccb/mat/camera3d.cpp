#include "camera3d.h"

/*
	PARAMETER DEFINITIONS:
	cp: camera position vector
	w_res: camera resolution width
	h_res: camera resolution height
	fov: cameras field of view
*/

/*
	constructor(float,float)
	purpose: create orthogonal 3D camera for objects
*/
Camera3D::Camera3D(float w_res,float h_res)
{
	view3D = glm::lookAt(glm::vec3(0,-0.01f,100),glm::vec3(),glm::vec3(0,0,1));
	proj3D = glm::ortho(0.0f,w_res,0.0f,h_res,0.1f,10000.0f);
}
// FIXME: duplicate code

/*
	constructor(vec3,float,float,float)
	purpose: create perspective 3D camera for objects
*/
Camera3D::Camera3D(glm::vec3 cp,float w_res,float h_res,float fov)
	: pos(cp)
{
	view3D = glm::lookAt(pos,pos+front,up);
	proj3D = glm::perspective(glm::radians(fov),w_res/h_res,0.1f,10000.0f);
}
// FIXME: completely nonsensical constructor definition

/*
	constructor(vec3,vec3,float,float,float)
	in_front: front look at direction
	purpose: create perspective 3D camera with defined look at position
*/
Camera3D::Camera3D(glm::vec3 cp,glm::vec3 in_front,float w_res,float h_res,float fov)
	: pos(cp),front(in_front)
{
	view3D = glm::lookAt(pos,front,up);
	proj3D = glm::perspective(glm::radians(fov),w_res/h_res,0.1f,10000.0f);
}

/*
	update() -> void
	purpose: recalculate view matrix after position and front change
*/
void Camera3D::update()
{ view3D = glm::lookAt(pos,pos+front,up); }