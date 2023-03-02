# Animation

## Purpose

Animations hold information of 2D objects to be rendered by a [Renderer2D](renderer2d.md). \
Normally animations do not have to be constructed by hand, because this will be automatically done by the renderer,
but information & values can be accessed & changed through renderer object list. \
A common usage of knowledge about this class is to modify the model matrix, which will be uploaded as a uniform variable whenever the renderer wishes.

<br>

***

<br>

## Construction

### Traditional Construction

```c++
// variable setup
glm::vec2 position = glm::vec2(0);				// animation position
float width = 100;								// animation scaling width
float height = 100;								// animation scaling height
const char* path = "./path/to/texture.png";		// path to animation sprite sheet
uint8_t rows = 4;								// subtexture rows on animation sprite sheet
uint8_t columns = 7;							// subtexture cols on animation sprite sheet
uint8_t number_of_iterations = 24;				// total number of textures on sprite sheet
uint8_t frames_to_loop = 30;					// number of frames to complete animation

// construction
Anim anim
    = Anim(position,widht,height,path,rows,columns,number_of_iterations,frames_to_loop);
```

### Ideal Construction

```c++
// predefinition
Renderer2D r2d = Renderer2D();

// construction
r2d.add(position,width,height,path,rows,columns,number_of_iterations,frames_to_loop);
```
*for common usage of sprites and correct usage of shown features check out [Renderer2D](renderer2d.md)*

<br>

***

<br>

## Features

### Setup

- texture() -> void : load animation sprite sheet
- setup() -> uint8_t : setup texture & update animation ticks. return current texture substage

### Transformation

- scale(float,float) -> void : change model matrix scaling

<br>

***

<br>

## Values

### System

- (*) v -> std::vector<float> : animation vertices to combine in renderer
- (*) tex -> GLuint : bindable animation sprite sheet

### Values

- (*) model -> mat4 : model matrix as uniform variable upload value
- (*) pos -> vec2 : origin position of animation object
- (*) sclx -> float : x-axis parallel scale of animation object
- (*) scly -> float : y-axis parallel scale of animation object
- (*) r -> uint8_t : equidistant subtexture rows on sprite sheet
- (*) c -> uint8_t : equidistant subtexture columns on sprite sheet

<br>

***

<br>

## Usage

### Scaling Animations

```c++
// set existing model matrix scale components to the desired scale
glm::vec2 desired_scale = glm::vec2(4,4);
anim.scale(desired_scale);
```

### Common Way of Addressing Animation Objects

```c++
// variable definition
Renderer2D r2d;				// details have been cut, for correct usage follow link below
uint8_t desired_anim = 4;	// animation the user wants to transform somehow

// example scaling of desired animation
r2d.al[desired_anim].scale(4,7);
```
*example uses [Renderer2D](renderer2d.md)*

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/anim.cpp) \
[Main Feature Component of Renderer2D](renderer2d.md)

<br>

***

<br>

## Bugs & Issues

- class is very featureless
- open todos and fixmes
- terrible naming: r,c instead of rows,columns? please! and try explaining to a 5 year old what a ft or fl is...