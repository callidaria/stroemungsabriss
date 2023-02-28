# 2D Camera

## Purpose

Calculates orthogonal camera for 2D elements & UI. \
Creates view & projection matrices, that can be uploaded to the shader.

<br>

***

<br>

## Construction

```c++
// parameter definitions
float resolution_width = 1280.0f;	// coordinate system x-axis capacity
float resolution_height = 720.0f;	// coordinate system y-axis capacity

// construction
Camera2D cam2D = Camera2D(resolution_width,resolution_height);
```

<br>

***

<br>

## Values

- (*) view -> mat4 : viewpoint direction matrix
- (*) proj -> mat4 : vertex perspective projection matrix

<br>

***

<br>

## Links

[Source Code with Comments](../../ccb/mat/camera2d.cpp)

<br>

***

<br>

## Bugs & Issues

! nothing to fix !