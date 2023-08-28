#ifndef CCB_AUDIO_LISTENER
#define CCB_AUDIO_LISTENER

#include <iostream>

#include <AL/al.h>
#include <glm/glm.hpp>

class Listener
{

public:

	// construction
	Listener(glm::vec3 pos=glm::vec3(0.0f),glm::vec3 vel=glm::vec3(0.0f),
			glm::vec3 ori=glm::vec3(0,0,-1));

	// setter
	inline void set_position(glm::vec3 pos) { alListener3f(AL_POSITION,pos.x,pos.y,pos.z); }
	inline void set_velocity(glm::vec3 vel) { alListener3f(AL_VELOCITY,vel.x,vel.y,vel.z); }
	inline void set_orientation(glm::vec3 ori) { alListener3f(AL_ORIENTATION,ori.x,ori.y,ori.z); }
};

#endif