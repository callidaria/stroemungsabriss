# Listener

## Purpose

The Listener represents the position, velocity and orientation of the "camera's microphone". \
The object acts as a reciever for emitted [Audio](audio.md).
This is obviously virtual and a simulation, but the necessary attributes for physical audio capturing are uploaded here.

<br>

***

<br>

## Construction

```c++
// variable setup
glm::vec3 position = glm::vec3(0);			// position of listener object in 3D space
glm::vec3 velocity = glm::vec3(0);			// velocity of listener object in 3D space
glm::vec3 orientation = glm::vec3(0,0,-1);	// orientation of the pseudo-microphone

// construction
Listener test_listener0 = Listener();	// same constructor as the following
Listener test_listener1 = Listener(position,velocity,orientation);
```
*test_listener0 and test_listener1 are identical because variables hold default values*

<br>

***

<br>

## Features

### Setter

- set_position(vec3) -> void : update position of audio listener
- set_velocity(vec3) -> void : set velocity of audio listener
- set_orientation(vec3) -> void : change listening direction

<br>

***

<br>

## Usage

### Changing Attributes On the Fly
```c++
// important pseudo variables
glm::vec3 camera_position;
glm::vec3 camera_velocity;
glm::vec3 camera_direction;

// create listener object
Listener listener = Listener(camera_position,camera_velocity,camera_direction);

// while game is running some attributes change
listener.set_position(camera_position);			// camera has been moved
listener.set_velocity(camera_velocity);			// camera is speeding across the world
listener.set_orientation(camera_direction);		// camera has been rotated
```
*a more in-depth usage example can be found in [Audio](audio.md)*

<br>

***

<br>

## Links

[Code Source with Comments](../../ccb/aud/listener.cpp) \
[Interacts with Audio](audio.md)

<br>

***

<br>

## Bugs & Issues

- Code commentary is incomplete
- Could be elegantly solved with static functions