# Audio

## Purpose

The Audio objects holds information for a specific sound file in .wav format. \
Additionally it pretends to be played from a specific position in 3D space,
picked up by the current [Listener](listener.md), which also has a 3D position,
so the sound appears to be immersed into the world and can represent real-world physical attributes.

<br>

***

<br>

## Construction

```c++
// variable setup
const char* path = "./path/to/soundfile.wav";	// path to soundfile. has to be defined
float gain = 1.0f;								// volume multiplier from native gain
float pitch = 1.0f;								// pitch, default 1.0f, intuitive tuning
glm::vec3 position = glm::vec3(0);				// origin position of sound emitter
glm::vec3 velocity = glm::vec3(0);				// starting velocity of sound emitter
bool looped = false;							// set if audio should be repeated constantly

// construction
Audio test_audio0 = Audio(path);	// same as following constructor, uses default variables
Audio test_audio1 = Audio(path,gain,pitch,position,velocity,looped);
```
*in this case test_audio0 is identical to test_audio1 because the variables hold default values*

<br>

***

<br>

## Features

### Action

- play() -> void : to play the sound
- remove() -> void : to remove the audio entity

### Setter

- set_all(float,float,vec3,vec3,bool) -> void : run all other setters over given values
- set_gain(float) -> void : set volume of sound or music
- set_pitch(float) -> void : set pitch, default pitch is (float)1
- set_position(vec3) -> void : update simulated origin position of the sound
- set_velocity(vec3) -> void : set velocity of origin object while emitting the sound (physics)
- set_loop(bool) -> void : determine if sound effect or music should loop

<br>

***

<br>

## Usage

### Background Music Example

```c++
// create listener to be able to hear emitted music
Listener listener = Listener();

// create looping background music audio
Audio bgm = Audio("./path/to/background_track.wav",1.0f,1.0f,glm::vec3(0),glm::vec3(0),true);

// then the stage gets rendered and the boss has appeared, the time is right
bgm.play();

// then something happens and your music has to react
bgm.set_gain(.4f);		// a menu has been opened and you want to reduce volume in menus
bgm.set_pitch(.25f);	// a slow motion effect has been enabled and you pitch accordingly

// the stage has been cleared and the boss utterly defeated, unloading music
bgm.remove();
```

### Sound Effect Example

```c++
// important pseudo variables
glm::vec3 player_position = glm::vec3(4,7,2);
glm::vec3 player_velocity = glm::vec3(2,4,7);
glm::vec3 enemy_position = glm::vec3(7,4,2);
glm::vec3 enemy_velocity = glm::vec3(2,7,4);

// create listener to be able to hear emitted music
Listener listener = Listener(player_position,player_velocity);

// create shooting sound effect audio for enemy
Audio sfx = Audio("./path/to/sound_effect.wav",1.0f,1.0f,enemy_position,enemy_velocity,false);

// then things get serious and you get shot at. nerdvision enabled:
sfx.play();

// but in fights things happen and change. here are a few examples how to handle that
sfx.set_pitch(.25f);				// slow motion pitch
sfx.set_position(enemy_position);	// enemy position has changed and must be updated
sfx.set_velocity(enemy_velocity);	// enemy velocity has changed and must be updated
sfx.set_loop(true);					// the rapid fire is so utterly enabled that sound loops

// boss has been defeated and the sound is not useful anymore. unloading it
sfx.remove();
```

<br>

***

<br>

## Links

[Code Source with Comments](../../ccb/aud/audio.cpp) \
[Interacts with Listener](listener.md)

<br>

***

<br>

## Bugs & Issues

- Apparently there is a weird bug when playing sounds right after construction
- Code commentary is not complete