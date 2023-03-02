#include "listener.h"

/*
	construction(vec3,vec3,vec3)
	pos: position of sound & music receiver
	vel: velocity the receiver travels in the given direction
	ori: direction the receiver is oriented towards
	purpose: create a listener, to receive audio objects
*/
Listener::Listener(glm::vec3 pos,glm::vec3 vel,glm::vec3 ori)
{
	set_position(pos);
	set_velocity(vel);
	set_orientation(ori);
}

/*
	set_position(vec3) -> void
	pos: new position of audio receiver
	purpose: update receiver position
*/
void Listener::set_position(glm::vec3 pos)
{ alListener3f(AL_POSITION,pos.x,pos.y,pos.z); }

/*
	set_velocity(vec3) -> void
	vel: new direction & velocity the audio receiver travels in
	purpose: update receiver velocity & direction
*/
void Listener::set_velocity(glm::vec3 vel)
{ alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z); }

/*
	set_orientation(vec3) -> void
	ori: new orientation of audio receiver
	purpose: update receiver orientation
*/
void Listener::set_orientation(glm::vec3 ori)
{ alListener3f(AL_ORIENTATION,ori.x,ori.y,ori.z); }