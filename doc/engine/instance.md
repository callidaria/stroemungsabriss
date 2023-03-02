# Instance

## Purpose

Instances hold information of 2D objects to be rendered by a [RendererI](rendereri.md). \
Instances should be created through the respective renderer, they require a lot of streamlined handling due to their uniqueness. \
Ultimately creating an instance allows the user to create a sprite and duplicate it a bunch of times, saving a lot of slow draw calls, by using the same vertex structures & textures. \
Can be used for particle effects, asteroids, bullets, grass etc... \
<b>It is not intended for the user to directly mess around with this class, this is purely functional.</b>

<br>

***

<br>

## Construction

```c++
// variable setup
RendererI rI = RendererI();						// create instance renderer, if none exists
glm::vec2 position = glm::vec2(240,500);		// origin position for duplicates of instance
float width = 100;								// width of instanced sprite
float height = 200;								// height of instanced sprite
const char* path = "./path/to/texture.png";		// path to texture

// construction
rI.add(position,width,height,path);				// construction through instance renderer
```

<br>

***

<br>

## Features

- texture() -> void : load instance texture
- setup() -> void : ready instance properties for upcoming draw

<br>

***

<br>

## Values

- (*)[24] v -> float\* : vertex array to combine in renderer
- (*) tex -> GLuint : bindable instance texture
- (*)[4096] o -> vec2 : position translation offsets for all duplicates to upload to shader

<br>

***

<br>

## Usage

*for intended usage and drawing of instances please direct yourself [here](rendereri.md)*

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/instance.cpp) \
[Main Feature Component of Instance Renderer](rendereri.md)

<br>

***

<br>

## Bugs & Issues

- features have been added while the creation branch 82 has been in development