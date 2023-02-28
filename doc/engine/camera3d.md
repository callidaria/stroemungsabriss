# 3D Camera

## Purpose

Calculates camera for 3D elements. \
Creates view & projection matrices, that can be uploaded to the shader.

<br>

***

<br>

## Construction

### Construction of Orthogonal Camera

```c++
// parameter definitions
float resolution_width = 1280.0f;	// coordinate system x-axis capacity
float resolution_height = 720.0f;	// coordinate system y-axis capacity

// construction
Camera3D cam3D = Camera3D(resolution_width,resolution_height);
```

### Construction of Perspective Camera

```c++
// parameter definitions
glm::vec3 camera_position = glm::vec3(0,10,0);	// position elevated 10 units above origin
float resolution_width = 1280.0f;				// after perspective manipulations
float resolution_height = 720.0f;				// after perspective manipulations
float field_of_view = 90.0f;					// in degrees

// construction
Camera3D cam3D = Camera3D(camera_position,resolution_width,resolution_height,field_of_view);
```

<br>

***

<br>

## Features

- update() -> void : recalculate view matrix (necessary if position, front or up vector has changed)

<br>

***

<br>

## Values

### Camera Directions

- (*) pos -> vec3 : camera position
- (*) front -> vec3 : camera view direction
- (*) up -> vec3 : specification of where "upwards" is for camera

### Camera Matrices

- (*) view3D -> mat4 : viewpoint direction matrix
- (*) proj3D -> mat4 : vertex perspective projection matrix

<br>

***

<br>

## Usage

### Example of Camera Movement Updates

```c++
// placeholder: camera construction as cam3D
// placeholder: scene creation

// placeholder: start main loop

// imagine the scene as train tracks in z-axis direction
// visually, we want to constantly drive forward on those tracks
pos.z++;
cam3D.update();

// placeholder: upload camera & draw scene
```
*supposed train moves at 1 unit per frame*

<br>

***

<br>

## Links

[Source Code with Comments](../../ccb/mat/camera3d.cpp)

<br>

***

<br>

## Bugs & Issues

- maybe use setters instead of making pos, front & up public
- code duplications