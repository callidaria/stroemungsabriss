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
	view2D = glm::lookAt(glm::vec3(0,-0.0001f,1),glm::vec3(.0f),glm::vec3(0,0,1));
	proj2D = glm::ortho(0.0f,x_bounds,0.0f,y_bounds,0.1f,10.0f);
}


/**
 *		TODO
*/

/*
	TODO
*/
Camera3D::Camera3D(glm::vec3 pos,glm::vec3 tgt,float width,float height)
{
	float t_HalfWidth = width*.5f,t_HalfHeight = height*.5f;
	view3D = glm::lookAt(pos,tgt,glm::vec3(0,0,1));
	proj3D = glm::ortho(-t_HalfWidth,t_HalfWidth,-t_HalfHeight,t_HalfHeight,.1f,100.f);
}

Camera3D::Camera3D(glm::vec3 pos,glm::vec3 tgt,float width,float height,float fov)
	: position(pos),target(tgt)
{
	view3D = glm::lookAt(position,target,glm::vec3(0,0,1));
	proj3D = glm::perspective(glm::radians(fov),width/height,.1f,1000.f);
}

/*
	TODO
*/
void Camera3D::rotate_around_target(float deg)
{
	position = glm::rotate(glm::mat4(1.f),glm::radians(deg),glm::vec3(0,0,1))*glm::vec4(position,.0f);
}

/*
	TODO
*/
void Camera3D::update()
{
	view3D = glm::lookAt(position,target,glm::vec3(0,0,1));
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
