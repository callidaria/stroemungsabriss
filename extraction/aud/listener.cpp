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