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
TODO

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
TODO

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