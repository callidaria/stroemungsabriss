#pragma once

#include <iostream>
#include <AL/al.h>
#include <glm/glm.hpp>


/*
 *	!!! SOME WEIRD POINTER ERROR WHEN TRYING TO PLAY RIGHT AFTER CONTRUCTOR CALL !!!
 *	PLEASE DON'T CALL play() RIGHT AFTER CONSTRUCTOR
 * */
class Audio
{
public:
	Audio(const char* path,float gain=1.0f,float pitch=1.0f,glm::vec3 pos=glm::vec3(0.0f),
			glm::vec3 vel=glm::vec3(1.0f),bool play_loop=false);

	void play();	// plays the sound
	void remove();	// removes the audio entity

	// setters
	void set_all(float gain,float pitch,glm::vec3 pos,glm::vec3 vel,bool play_loop);
	void set_gain(float gain);
	void set_pitch(float pitch);
	void set_position(glm::vec3 pos);
	void set_velocity(glm::vec3 vel);
	void set_loop(bool play_loop);
private:
	unsigned int m_buffer,m_audio;	// buffer and audio members
};
