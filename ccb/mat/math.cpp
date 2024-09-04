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
	: m_position(position),m_target(target)
{
	view3D = glm::lookAt(position,target,glm::vec3(0,0,1));
	proj3D = glm::perspective(glm::radians(fov),ratio,.1f,10000.f);
}

/*
	TODO
*/
void Camera3D::rotate_around_target(float deg)
{
	m_position = glm::rotate(glm::mat4(1.f),glm::radians(deg),glm::vec3(0,0,1))*glm::vec4(m_position,1);
}

/*
	TODO
*/
void Camera3D::update()
{
	view3D = glm::lookAt(m_position,m_target,glm::vec3(0,0,1));
}


/**
 *		TODO expand
*/

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r)
{
	translate(p);
	scale(w,h);
	rotate(r);
}

/*
	TODO
*/
void Transform2D::transform(glm::vec2 p,float w,float h,float r,glm::vec2 a)
{
	model = glm::mat4(1.f);
	translate(a-position);
	transform(position+p-a,w,h,r);
}

/*
	TODO
*/
void Transform2D::scale(float w,float h,glm::vec2 a)
{
	model = glm::mat4(1.f);
	translate(a-position);
	transform(position-a,w,h,rotation);
}

/*
	TODO
*/
void Transform2D::rotate(float r)
{
	model = glm::rotate(model,glm::radians(r),glm::vec3(0,0,1));
	rotation = r;
}

/*
	TODO
*/
void Transform2D::rotate(float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	float t_WidthFactor = model[0][0], t_HeightFactor = model[1][1];
	model = glm::mat4(1.f);
	translate(a-position);
	transform(position-a,t_WidthFactor,t_HeightFactor,r);
}


/*
	TODO
*/
void Transform3D::transform(glm::vec3 pos,float scl,glm::vec3 rot)
{
	translate(pos);
	scale(scl);
	rotate(rot);
}

/*
	TODO
*/
void Transform3D::transform(glm::vec3 pos,float scl,glm::vec3 rot,glm::vec3 arbit)
{
	model = glm::mat4(1.f);
	translate(arbit-position);
	transform(position+pos-arbit,scl,rot);
}

/*
	TODO
*/
void Transform3D::scale(float scl,glm::vec3 arbit)
{
	model = glm::mat4(1.f);
	translate(arbit-position);
	transform(position-arbit,scl,rotation);
}

/*
	TODO
*/
void Transform3D::rotate(glm::vec3 rot)
{
	model = glm::rotate(model,glm::radians(rot.x),glm::vec3(1,0,0));
	model = glm::rotate(model,glm::radians(rot.y),glm::vec3(0,1,0));
	model = glm::rotate(model,glm::radians(rot.z),glm::vec3(0,0,1));
	rotation = rot;
}

/*
	TODO
*/
void Transform3D::rotate(glm::vec3 rot,glm::vec3 arbit)
{
	float t_ScaleFactor = model[0][0];  // FIXME: untested questionable math trickery
	model = glm::mat4(1.f);
	translate(arbit-position);
	transform(position-arbit,t_ScaleFactor,rot);
}
