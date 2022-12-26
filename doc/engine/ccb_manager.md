# Cascabel Engine Manager

## Purpose

This manager handles automatic environment loading, as well as the developer console. \
Adding this for debug mode, enables the possibility to change environments while the game is running.

<br>

***

<br>

## Construction

```c++
// parameter setup
Frame* frame = &example_frame;          // frame entity, handling input for dev console
Renderer2D* r2d = &example_renderer;    // renderer to handle loaded & added 2D entities
Camera2D* cam2d = &example_camera;      // reference to 2D camera defining coordinate system

// construction
CCBManager ccbm = CCBManager(frame,r2d,cam2d);
```
*Construction depends on [Frame](frame.md), [Renderer2D](renderer2d.md) & [Camera2D](camera2d.md)*

<br>

***

<br>

## Features

- add_lv(const char*) -> uint16_t : loads a cascabel level environment file & returns start address
- dev_console(uint32_t&,bool&) -> void : renders and breaths life into developer console

<br>

***

<br>

## Usage

### Load Levels

```c++
// construction
CCBManager ccb_manager = CCBManager(&frame,&r2d,&cam2d);

// load level environment file
uint16_t amount_of_file_entites = 6;				// placeholder variable for entity count
uint16_t renderer_start = ccb_manager.add_lv();		// load level environment file

// render environment
// placeholder: setup buffer, shader & renderer
r2d.render_sprite(renderer_start,renderer_start+amount_of_file_entites);
```
*construction recieves variables, that have not been defined prior. please use logical deduction* \
*example assumes that level environment file contains load instructions of exactely 6 sprites*

### Add Cascabel Developer Console

```c++
// defining development build (optional)
#define DEV_BUILD 1

// placeholder: do normal things

// set runstate variable
uint32_t running = 1;
while (running) {

// ask for development build at compile (optional)
#ifdef DEV_BUILD

// placeholder: define variable "dactive" through console activation process of choice

	ccb_manager.dev_console(running,dactive);

// close development build exclusion (optional)
#endif

// placeholder: all other main loop stuff
}
```
*construction of manager has been omitted due to triviality, entity is ccb_manager*

<br>

***

<br>

## Links

[Source Code](../../ccb/fcn/ccb_manager.cpp) \
Construction depends on [Frame](frame.md), [Renderer2D](renderer2d.md) & [Camera2D](camera2d.md)

<br>

***

<br>

## Bugs & Issues

- bad source documentation
- unreadable code
- developer console has not a lot of functions yet
- no way of returning how many entities a renderer recieves per level environment file