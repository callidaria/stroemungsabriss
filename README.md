# callidaria's yomisensei

## RUN

<br>

### linux
```
./yomisensei
```
or press [r] key in cascabel console

<br>

### windows
double click yomisensei.exe \
or press [r] key in cascabel console

### DISCLAIMER
*building is not required to run the game!*

<br>

***

<br>

## BUILD

<br>

### linux
```
# to open cascabel console
./ccb_console
```
select BUILD ALL COMPONENTS with arrow keys and run with [SPACE] to build libraries \
then, press [b] key to build executable

<br>

### windows
run ccb_console.exe \
select BUILD ALL COMPONENTS with [w] & [s] and run with [SPACE] to build libraries \
then, press [b] key to build executable

<br>

### DISCLAIMER
*certain libraries are required to satisfy the compiler:*
- SDL2
- SDL2_net
- GLEW
- GLM
- SOIL
- OpenAL

<br>

To see the full documentation of engine go [here](doc/index.md)

<br>

***

<br>

## v0.0.5 DEV

### features:
- deferred rendering for a much more complex lighting system
- new physical based shading system
- loading feedback screen
- dynamic world loading
- store "macro" savestate pattern
- finding all FIXMEs, TODOs & DEPRECATEDs in source code and show in ccb console by path
- store "macro" savestate pattern
- automatic compilation of all files using the originally updated source

### optimizations:
- system to remove world objects should they have been deemed obsolete
- hotplugging controllers
- switching dynamically between input methods
- autoremoving all libraries when the whole project gets rebuilt
- colour correction: high dynamic range
- colour correction: gamma correction
- improving code quality/portability by substituting pragma once usage
- constant face culling, no more disable/enable calls
- dynamic directory/file finder for line counter
- improved runtime debug output

### fixes:
- FIXED: double main loop
- FIXED: memory leaks
- FIXED: shadow transition acne on heavily sloped surfaces
- FIXED: memory issues with 3D renderer

<br>

## v0.0.4 DEV [UNX]

### features:
- individual rotations of instanced objects
- individual animations of instanced objects
- system for interactable playing card games
- communication system with conversation tree interpretation

### fixes:
- FIXED: despawned directed projectiles sometimes appear in view
- FIXED: negative jet tilt according to movement input
- FIXED: hitbox indicator over player character somehow gets covered by character model
- FIXED: bullet system renders spawned bullets twice with a slight offset
- FIXED: texture issues with opponent's flyfighter object
- FIXED: lined & misplaced rendering of emitted projectiles

<br>

## v0.0.3 [WIN]

### features:
- preview practice & arcade position in menu
- spawn player controlled bullets
- health bar system
- engine build system
- action menu for system and informations
- time delta disconnected from frames
- most basic healthbar style

### optimizations:
- improved documentation
- structured buffer creation
- removing all planning and progression scripts from repo
- reconceptualizing the basic menu colour scheme
- new dynamic shader program concept
- new renderer code structure
- readable menu code
- simplifications of vertex loading
- 3D protagonists placeholders dpilot
- sorted script source and header file

### fixes:
- FIXED: title animation leaks into camera view when not desired
- FIXED: autocreation of initialization file, if not present
- FIXED: title splash geometry gets modified by sepia vignette
- FIXED: standard splash's idle geometry gets modified by selection
- FIXED: menu dialogue has deprecated functions
- FIXED: collision sphere center off object center
- FIXED: scuffed geometry of globe preview
- FIXED: missing anti aliasing on menu selection splash
- FIXED: unused bullets get rendered at origin position
- FIXED: memory issues with font - text relationship

Worktime: 167h 51min

<br>

## v0.0.2
- Build in Dev Mode & Engine Terminal
- Dynamic Entity Loader & Interpreter
- Windows Port
- Test Builds
- Main Menu Prototype
- Options Configuration Save
- First Player Movement Prototype

Worktime: 120h

<br>

***

<br>

## Trivia
- Total Worktime: 287h 51min
- Elapsed Worktime After v0.0.3:
	+ 43: 9h 17min
	+ 114: 2min
	+ 91: 16h 37min
	+ 106: 64h 32min
	+ 82: 20h 50min
	+ 137: 15h 13min
	+ 108: 48h 4min
- Elapsed Worktime After v0.0.4:
	+ 97: 7h 2min
	+ 129: 8h 4min
	+ 132: 7min
	+ 64: 2h 20min
	+ 47: 20h 57min
	+ 164: 9h 27min
	+ 147: 50h 53min
	+ 169: 2h 1min
	+ 124: 15h 14min
	+ 125: 1h
	+ 153: 74h 20min
	+ 112: 57min
	+ 159: 5h 32min
	+ 62: 1h 52min
	+ 90: 9h 50min
	+ 83: 8h 16min
- Total Code Lines: 14.314