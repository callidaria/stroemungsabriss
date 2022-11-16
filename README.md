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
double click yomisensei.exe

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
select BUILD ALL COMPONENTS and run with [SPACE] to build libraries \
then, press [b] key to build executable

<br>

### windows
```
make winlib
make win
```

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

***

<br>

## v0.0.3 DEV

### features:
- preview practice & arcade position in menu
- spawn player controlled bullets
- health bar system
- engine build system
- action menu for system and informations [DEV]
- time delta disconnected from frames [DEV]

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

### fixes:
- FIXED: title animation leaks into camera view when not desired
- FIXED: autocreation of initialization file, if not present
- FIXED: title splash geometry gets modified by sepia vignette
- FIXED: standard splash's idle geometry gets modified by selection
- FIXED: menu dialogue has deprecated functions
- FIXED: collision sphere center off object center
- FIXED: scuffed geometry of globe preview
- FIXED: missing anti aliasing on menu selection splash

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
- Total Worktime: 120h
- Elapsed Worktime After v0.0.2:
	+ ??: 19h 19min
	+ 46: 38h 55min
	+ 54: 12h 40min
	+ 20: 3h 8min
	+ 67: 1h 27min
	+ 22: 5h 23min
	+ 56: 1h 5min
	+ 74: 4h 58min
    + 63: 13h 25min
	+ 79: 1h
	+ 78: 15h 22min
- Total Code Lines: 8422