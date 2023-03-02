# Frame

## Purpose

Everything depends on Frame. \
Window creation, render context, initialization of graphical environment & even input. \
Use Frame before everything else except initialization is needed.

<br>

***

<br>

## Construction

```c++
// variable setup
const char* title = "callidaria-program";	// window title displayed in frame's titlebar
int8_t screen = 0;							// target screen, will fall back to default if over
int16_t width = 1280;						// frame resolution pixel width
int16_t height = 720;						// frame resolution pixel height
SDL_WindowFlags fs = (SDL_WindowFlags)0;	// full screen status through SDL_WindowFlags

// construction
Frame test_frame0 = Frame();
Frame test_frame1 = Frame(title,screen,fs);
Frame test_frame2 = Frame(title,width,height,fs);
Frame test_frame3 = Frame(title,screen,width,height,fs);
```
*all constructed frames will create the same window, because variables hold default values* \
*heads up: don't be startled, this will suffice to instantly create a window*

<br>

***

<br>

## Features

### Modification

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

- (\*) m_gc -> std::vector<SDL_GameController*> : list of controllers sdl found
- (*) m_fe -> SDL_Event : frame event by sdl handling input
- (*) kb -> Keyboard : keyboard structure, holding key array
- (*) mouse -> Mouse : mouse structure, holding buttons, axis and movements
- (*) xb -> std::vector<XBox> : list of controllers sdl found, parsed into xbox feature list
- (*) tline -> std::string : holds current text input until mode ends

<br>

***

<br>

## Usage

### Create Basic Updating Frame

```c++
// open window
Frame frame = Frame();

// placeholder: setup all pre-loop necessities

// start main loop
uint32_t loop_state = 1;
while(loop_state) {

	// optional: fps features
	frame.print_fps();	// shows current frames per second in console
	frame.vsync();		// reduces maximum frames per second to 60
	// OR
	frame.vsync(144);	// does the same but maximum frame count can be changed (e.g. 144fps)

	// update frame independent time
	frame.calc_time_delta();

	// process input and frame closing
	frame.input(loop_state);

	// clear buffer
	frame.clear();			// normally clear buffer to black
	// OR
	frame.clear(0,1,0);		// alternatively clear buffer to a specific colour (e.g. green)

	// swap buffer
	frame.update();
}

// close frame
frame.vanish();
```

### Make Things Depend on Time

```c++
// creating values
glm::vec3 position = glm::vec3(0);	// pseudo vector we want to manipulate later
float certain_amount = 10.41f;		// amount of constant manipulation

// placeholder: open main loop

// you want to add certain_amount per actual second to the x value of the created vector
position.x += certain_amount*frame.get_time_delta();
```
*"frame" is product of Frame constructor, construction has been omitted for the sake of brevity*

### Manipulate Time

```c++
// creating values
float time_modificator = 1.0f;	// variable holds current time factor (default: unmodified)
float time_grad = 0.1f;			// rate of gradual change towards target modificator

// placeholder: open main loop
// placeholder: events changing time_modificator due to slow motion effect and speedups

// update time modification
frame.set_tmod(time_modificator);				// instantly set time modification to target
// OR
frame.change_tmod(time_modificator,time_grad);	// gradual change achieving slowdown/speedup
```

### Enable/Disable Text Input Mode

```c++
// creating values
bool text_input;	// value representing if text input mode should be enabled

// placeholder: open main loop
// placeholder: events changing status of text_input

// set/unset text input mode
if (text_input) frame.input_start();
else frame.input_stop();
```
*because it is so unfathomably important: !avoid branching, do not "all copy, no brain" these examples!*

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
- DEPRECATIONS exist. good, because tick time derivative in get_time_delta() is wrong anyways
- generalized integers used
- NO! SDLGC CAN BE USED OUTSIDE OF THIS CLASS! JUST USE !!xb.size() FOR CONNECTION APPROVAL