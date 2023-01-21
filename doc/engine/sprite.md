# Sprite

## Purpose

Sprites hold information of 2D objects to be rendered by a [Renderer2D](renderer2d.md). \
Normally sprites do not have to be constructed by hand, because this will be automatically done by the renderer,
but information & values can be accessed & changed through renderer object list. \
A common usage of knowledge about this class is to modify the model matrix, which will be uploaded as a uniform variable whenever the renderer wishes.

<br>

***

<br>

## Construction

### Technical Construction

```c++
// variable setup
glm::vec2 position = glm::vec2(0);				// sprite position
float width = 100;								// sprite scaling width
float height = 100;								// sprite scaling height
const char* path = "./path/to/texture.png";		// path to sprite texture

// construction
Sprite sprite = Sprite(position,widht,height,path);
```

### Common Construction

```c++
// predefinition
Renderer2D r2d;		// construction & previous actions have been cut

// construction
r2d.add(position,width,height,path);
```
*for common usage of sprites and correct usage of shown features check out [Renderer2D](renderer2d.md)*

<br>

***

<br>

## Features

### Setup

- texture() -> void : load sprite texture
- setup() -> void : ready components (the texture) for render process

### Transformation

- transform(vec2,vec2,float) -> void : transform position, scaling & rotation of model matrix
- transform(vec2,vec2,float,vec2) -> void : transform model matrix around arbitrary axis
- translate(vec2) -> void : translation of sprite position through model matrix
- scale(float,float) -> void : change model matrix scaling
- scale_arbit(float,float) -> void : change model matrix scaling but reverse initial translation first
- rotate(float) -> void : change model matrix rotation

<br>

***

<br>

## Values

### System

- (*) v -> std::vector<float> : sprite vertices to combine into render buffer
- (*) tex -> GLuint : bindable sprite texture

### Position

- (*) model -> mat4 : model matrix as uniform variable upload value
- (*) pos -> vec2 : origin position of sprite object
- (*) sclx -> float : x-axis parallel scale of sprite object
- (*) scly -> float : y-axis parallel scale of sprite object

<br>

***

<br>

## Usage

### Transformations

```c++
// fully transforming the sprite
glm::vec2 position = glm::vec2(100,100);	// move sprite instantly to given position
glm::vec2 scale = glm::vec2(1,2);			// double vertical scaling as compared to origin
float rotation = 45.0f;						// rotate to 45 !degrees! from origin rotation
sprite.transform(position,scale,rotation);

// transform sprite as before, but move origin position to an arbitrary axis
glm::vec2 arbit = glm::vec2(100,100);	// rotate around, scale based on and translate from
sprite.transform(position,scale,rotation,arbit);
```

### Singular Transformation Actions

```c++
// translate existing model matrix towards desired position
glm::vec2 desired_position = glm::vec2(250,250);
sprite.translate(desired_position);

// now set scale matrix components of existing model to desired scale
glm::vec2 desired_scale = glm::vec2(4,4);
sprite.scale(desired_scale.x,desired_scale.y);

// throw away all previous changes and scale based on zeroed origin axis
sprite.scale_arbit(desired_scale.x,desired_scale.y);

// rotate existing model matrix (around z obviously, because it is a 2D entity - pac)
// (((& i can't see, temporarilyyyy - frank zappa, the only well adjusted musician)))
float resulting_rotation = 270.0f;	// (rotation is in degrees) can't stop rhymin' wth
sprite.rotate(resulting_rotation);
```
*most of the time the way of addressing sprites differs from this example. realistic showcase below* \
*also, i apologize for the insanityyyy*

### Common Way of Addressing Sprite Objects

```c++
// variable definition
Renderer2D r2d;					// details have been cut, for correct usage follow link below
uint8_t desired_sprite = 4;		// sprite the user wants to transform somehow

// example rotation of desired sprite around 90 degrees
r2d.sl[desired_sprite].rotate(90.0f);
```
*example uses [Renderer2D](renderer2d.md)*

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/sprite.cpp) \
[Main Feature Component of Renderer2D](renderer2d.md)

<br>

***

<br>

## Bugs & Issues

- existing model matrix has to be thrown away for arbitrary scaling
- rotation feature itself is a little primitive
- i'm sure we can find more poorly aged things? i'm suspicious, because i have not too many problems with this