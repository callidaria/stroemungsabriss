# callidaria's yomisensei

## BUILD

### linux
```
# only the first time to build the ccb library
./ccb_console
build
exit

# then
make
```

### windows
```
make winlib
make win
```

### DISCLAIMER
*certain libraries are required to satisfy the compiler*

***

## RUN

### linux
```
./yomisensei
```

### windows
double click yomisensei.exe

### DISCLAIMER
*building is not required to run the game*

***

## v0.0.3 DEV

### features:
- preview practice & arcade position in menu
- spawn player controlled bullets
- health bar system

### optimizations:
- improved documentation
- structured buffer creation
- removing all planning and progression scripts from repo
- reconceptualizing the basic menu colour scheme
- new dynamic shader program concept
- new renderer code structure
- simplifications of vertex loading

### fixes:
- FIXED: title animation leaks into camera view when not desired
- FIXED: autocreation of initialization file, if not present
- FIXED: title splash geometry gets modified by sepia vignette
- FIXED: standard splash's idle geometry gets modified by selection
- FIXED: menu dialogue has deprecated functions
- FIXED: collision sphere center off object center

<br>

## v0.0.2
- Build in Dev Mode & Engine Terminal
- Dynamic Entity Loader & Interpreter
- Windows Port
- Test Builds
- Main Menu Prototype
- Options Configuration Save
- First Player Movement Prototype

***

## Trivia
- Elapsed Worktime After Commit 120: 19h 19min
    + 46: 38h 55min
    + 54: 12h 40min
    + 67: 1h 27min
- Total Code Lines: 6564