# Material

## Purpose

A Material is supposed to save relevant uniform variables to primitively try to approximate a materials reflectional behaviour. \
This is a pre-modern simplification to render light reflection effects without PBR.

<br>

***

<br>

## Construction

```c++
// parameter definitions
Renderer3D* renderer3D;		// pointer to rendererer holding related object(s)
int texture_repeat = 1;		// factor for texture repitition
int refl_specular = 32;		// specular reflection value
int refl_intensity = 4;		// reflection intensity

// construction
Material3D mat3D = Material3D(renderer3D,texture_repeat,refl_specular,refl_intensity);
```

<br>

***

<br>

## Features

- upload() -> void : upload material values before draw

<br>

***

<br>

## Pipeline

### Upload Before Draw But After Preparation:

```c++
renderer3D.prepare();
material3D.upload();
renderer3D.render_mesh(0,1);
```
*naturally, 3D shader must be enabled before uploading* \
*renderer3D is a [Renderer3D](renderer3d.md)*

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/material3d.h) \
[Includes Renderer3D as Parameter](renderer3d.md)

<br>

***

<br>

## Bugs & Issues

- contained in header
- highly exclusive
- uses unspecific integer