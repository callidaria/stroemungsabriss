# Frame

## Purpose

Everything depends on Frame. \
Window creation, render context, initialization of graphical environment & even input. \
Use Frame before everything else except initialization is needed.

<br>

***

<br>

## Construction

<br>

***

<br>

## Features

### Modification:

- clear(float(0),float(0),float(0)) -> void : clears the screen in given rgb colour
- update() -> void : update screen to show everything that has been drawn

### Frame Update Timing

- print_fps() -> void : prints fps count to terminal every second
- vsync(uint8_t(60)) -> void : spreads out frame timing so the desired frame count is achieved

### Time

- calc_time_delta() -> void : updates time delta between frames for time dependent features
- set_tmod(float) -> void : time modification, <1 causes slow motion while >1 causes fast forward
- change_tmod(float,float) -> void : ease into target time modification at given rate, e.g. slowdown

### Controlling

- input(uint32_t&,tinput(false)) -> void : processes user input towards being used by a human

### Termination

- vanish() -> void : close window and delete contexts

### Text Input

- input_start() -> void : start text input mode
- input_stop() -> void : stops text input mode

### Getter

- get_time_delta() -> float : returns if frame was faster or slower than tick rate

<br>

***

<br>

## Values

### Properties

- (*) w_res -> int : frame resolution width
- (*) h_res -> int : frame resolution height
- (*) event_active -> bool : identifies if event poll has succeeded at current frame

### Input

- (*) m_gc -> std::vector<SDL_GameController*> : list of controllers sdl found
- (*) m_fe -> SDL_Event : frame event by sdl handling input
- (*) kb -> Keyboard : keyboard structure, holding key array
- (*) mouse -> Mouse : mouse structure, holding buttons, axis and movements
- (*) xb -> std::vector<XBox> : list of controllers sdl found, parsed into xbox feature list
- (*) tline -> std::string : holds current text input until mode ends

<br>

***

<br>

## Pipeline

<br>

***

<br>

## Usage

<br>

***

<br>

## Links

[Source Code](../../ccb/frm/frame.cpp)

<br>

***

<br>

## Bugs & Issues

- a lot of many things are public which would be better off private
- input_start()/input_stop() is a glorified and overly verbose setter
- DEPRECATIONS in exsitance. good, because tick time derivative in get_time_delta() is wrong anyways
- generalized integers used
- NO! SDLGC CAN BE USED OUTSIDE OF THIS CLASS! JUST USE xb.size() FOR CONNECTION APPROVAL