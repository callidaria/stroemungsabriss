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

### optimizations:
- menu dialogue documentation
- structured buffer creation
- removing all planning and progression scripts from repo
- reconceptualizing the basic menu colour scheme
- new dynamic shader program concept

### fixes:
- FIXED: title animation leaks into camera view when not desired
- FIXED: autocreation of initialization file, if not present
- FIXED: title splash geometry gets modified by sepia vignette
- FIXED: standard splash's idle geometry gets modified by selection
- FIXED: menu dialogue has deprecated functions

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

## Additional Information
- Elapsed Worktime After Commit 120: 16h
    + 23: 1h 21min 43sec
    + 17: 27min
    + 42: 1h 30min
- Total Code Lines: 5143