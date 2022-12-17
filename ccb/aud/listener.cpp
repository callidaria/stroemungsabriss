#include "listener.h"

/*
	TODO
*/
Listener::Listener(glm::vec3 pos,glm::vec3 vel,glm::vec3 ori)
{
	set_position(pos);
	set_velocity(vel);
	set_orientation(ori);
}

/*
	TODO
*/
void Listener::set_position(glm::vec3 pos)
{ alListener3f(AL_POSITION,pos.x,pos.y,pos.z); }

/*
	TODO
*/
void Listener::set_velocity(glm::vec3 vel)
{ alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z); }

/*
	TODO
*/
void Listener::set_orientation(glm::vec3 ori)
{ alListener3f(AL_ORIENTATION,ori.x,ori.y,ori.z); }