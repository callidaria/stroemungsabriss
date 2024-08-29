#include "math.h"


/**
 *		TODO expand
*/

/*
	!O(1) /load -> (construction)
	x_bounds: maximum on-camera visible x value
	y_bounds: maximum on-camera visible y value
	purpose: creates an orthogonal 2D camera for sprites
*/
Camera2D::Camera2D(float x_bounds,float y_bounds)
{
	view2D = glm::lookAt(glm::vec3(0,-0.01f,1),glm::vec3(),glm::vec3(0,0,1));
	proj2D = glm::ortho(0.0f,x_bounds,0.0f,y_bounds,0.1f,10.0f);
}


/**
 *		TODO
*/

Camera3D::Camera3D(glm::vec3 position,glm::vec3 target,float ratio,float fov)
{
	view3D = glm::lookAt(position,target,glm::vec3(0,0,1));
	proj3D = glm::perspective(glm::radians(fov),ratio,.1f,10000.f);
}


/**
 *		TODO expand
*/

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r)
{
	rotate(r);
	scale(w,h);
	translate(p);
}

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// execute transformation
	rotate(r);
	scale(w,h);
	translate(position+p-a);

	// save current rotation
	rot = r;
}

/*
	TODO
*/
void Transform2D::to_origin()
{
	translate(position);
	scale(width,height);
}

/*
	TODO
*/
void Transform2D::scale(float w,float h,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// scale from arbitrary position & reconstruct
	rotate(rot);
	scale(w,h);
	translate(position-a);
}

/*
	TODO
*/
void Transform2D::rotate(float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	float t_wfac = model[0][0], t_hfac = model[1][1];
	model = glm::mat4(1.f);
	translate(a-position);

	// change rotation and reset scaling and previous position
	rotate(r);
	scale(t_wfac,t_hfac);

	// reconstruct valid transform state
	translate(position-a);
	rot = r;
}
