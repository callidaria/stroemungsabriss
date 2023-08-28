#ifndef CCB_AUDIO_SELFTITLED
#define CCB_AUDIO_SELFTITLED

#include <iostream>

#include <AL/al.h>
#include <glm/glm.hpp>


/*
 *	!!! SOME WEIRD POINTER ERROR WHEN TRYING TO PLAY RIGHT AFTER CONSTRUCTOR CALL !!!
 *	PLEASE DON'T CALL play() RIGHT AFTER CONSTRUCTOR
 * */

class Audio
{

public:

	// construction
	Audio(const char* path,float gain=1.0f,float pitch=1.0f,glm::vec3 pos=glm::vec3(0.0f),
			glm::vec3 vel=glm::vec3(0.0f),bool play_loop=false);

	// action
	inline void play() { alSourcePlay(m_audio); }
	void remove();

	// setter
	void set_all(float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop);
	inline void set_gain(float gain) { alSourcef(m_audio,AL_GAIN,gain); }
	inline void set_pitch(float pitch) { alSourcef(m_audio,AL_PITCH,pitch); }
	inline void set_position(glm::vec3 pos) { alSource3f(m_audio,AL_POSITION,pos.x,pos.y,pos.z); }
	inline void set_velocity(glm::vec3 vel) { alSource3f(m_audio,AL_VELOCITY,vel.x,vel.y,vel.z); }
	inline void set_loop(bool play_loop) { alSourcei(m_audio,AL_LOOPING,play_loop); }

private:

	unsigned int m_buffer,m_audio;	// buffer and audio members

};

#endif