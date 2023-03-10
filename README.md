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

***

<br>

## v0.0.4 DEV [UNX]

### features:
- individual rotations of instanced objects
- individual animations of instanced objects
- system for interactable playing card games

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
	+ 137: 10h 59min
- Total Code Lines: 11.495