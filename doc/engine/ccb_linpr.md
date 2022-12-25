# Cascabel Language Interpreter

## Purpose

Reads a .ccb file and sets up environment as specified within that file. \
Objects will be created at given position with given attributes based on referenced resources.

<br>

***

<br>

## Construction

```c++
// parameter setup
Renderer2D* r2d = &example_renderer2D;				// 2D renderer handling all 2D requests
const char* path = "./path/to/environment.ccb";		// path to environment cascabel file

// construction
CCBLInterpreter ccb_linterpreter = CCBLInterpreter(r2d,path);
```
*Construction depends on [Renderer2D](renderer2d.md)
 
<br>

***

<br>

## Features

- load_level() -> uint16_t : process all creation requests held by the cascabel environment file
- write_level() -> void : used by the developer console; writes changes to the creation list

<br>

***

<br>

## Values

- m_pos -> std::vector<vec2> : list of created sprite object position
- a_pos -> std::vector<vec2> : list of created animation object position
- m_width -> std::vector<float> : list of created sprite object widths
- a_width -> std::vector<float> : list of created animation object widths
- m_height -> std::vector<float> : list of created sprite object heights
- a_height -> std::vector<float> : list of created animation object heights
- a_row -> std::vector<float> : list of rows uploaded for animations in order of creation
- a_column -> std::vector<float> : list of columns uploaded for animations in order of creation
- a_frames -> std::vector<float> : list of frames uploaded for animations in order of creation
- a_ts -> std::vector<float> : list of texture counts uploaded for animations in order of creation
- m_tex -> std::vector<const char*> : list of created sprite texture paths
- a_tex -> std::vector<const char*> : list of created animation texture paths
- spritesCount -> int : amount of all loaded sprites

<br>

***

<br>

## Usage

### Load Level

```c++
// variable setup
Renderer2D* r2d = &example_renderer2D;
const char* path = "./path/to/environment.ccb"

// load referenced level into renderer space
CCBLInterpreter ccbl_interpreter = CCBLInterpreter(r2d,path);
uint16_t memory_reference = ccbl_interpreter.load_level();

// placeholder: further renderer & shader setup

// render level
// placeholder: buffer, shader & renderer setup
r2d->render_sprite(memory_reference,memory_reference+ccbl_interpreter.spritesCount);
```

### Write Level

```c++
// an object has to be changed
ccbl_interpreter.m_pos[4].x += 100;		// modify position of 4th sprite by 100 units
ccbl_interpreter.a_height[2] = 50;		// set the 2nd animations height to 50 units

// upload changes
ccbl_interpreter.write_level();

// now a new sprite has to be added
ccbl_interpreter.m_pos.push_back(glm::vec2(100,100));			// add sprite at position
ccbl_interpreter.m_width.push_back(250);						// with a width of 250 units
ccbl_interpreter.m_height.push_back(400);						// and a height of 400 units
ccbl_interpreter.m_tex.push_back("./path/to/new_texture.png");	// add the sprite's texture

// optional: add other objects or make changes to different entites

// upload new object
ccbl_interpreter.write_level();
```
*when adding a new object, all object related lists must be extended by exactely one value* \
*it is possible to also add an animation this way, but all values are to be added to the anim lists* \
*construction has been omitted from this example due to triviality*

<br>

***

<br>

## Links

[Source Code](../../ccb/fcn/ccb_linpr.cpp) \
[Utilizes Renderer2D in Construction](renderer2d.md)

<br>

***

<br>

## Bugs & Issues

- code not in the slightest readable
- does not recognize many creation commands yet
- also please listen to the introduction comment in the [header file](../../ccb/fcn/ccb_linpr.h)
- usage of "int" type