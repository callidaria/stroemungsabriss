# Shader

## Purpose

Compiles vertex & fragment shader (no support for geometry shader yet) and combines them as a shader program, which can be enabled before rendering related structures.

<br>

***

<br>

## Construction

Trivial, construction at definition.

<br>

***

<br>

## Features

### Compilation

- compile(const char*,const char*) -> void : compiles the vertex and fragment shaders at given path
- compile2d(const char*,const char*) -> void : compiles the shaders and defines common 2d upload
- compile3d(const char*,const char*) -> void : compiles the shaders and defines common 3d upload

### Pattern Definition

- def_attributeF(const char*,uint8_t,uint8_t,uint8_t) -> void : define attribute location in upload structure
- def_indexF(unsigned int,const char*,uint8_t,uint8_t,uint8_t) -> void : define attribute location in index upload

### Setup

- enable() -> void : enables shader program for rendering or modification

### Upload

- upload_int(const char*,int) -> void : upload integer value as uniform to shader program
- upload_float(const char*,float) : upload float value as uniform to shader program
- upload_vec2(const char*,vec2||vec3||vec4) : upload vector as uniform to shader program
- upload_matrix(const char*,mat4) : upload matrix as uniform to shader program
- upload_camera(const char*,Camera2D||Camera3D) : upload view & projection matrices to shader program

<br>

***

<br>

## Pipeline

### Bind Index Buffer Object Before Defining Index Pattern

```c++
buffer.bind_index();
shader.def_indexF(buffer.get_indices(),"test_location",var_dim,pattern_offset,pattern_cap);
```
*pipeline depends on [buffer](buffer.md)*

<br>

***

<br>

## Usage

### Compile & Define Upload Structure

```c++
// setup for structure
// [pos,pos,pos,pos,col,col,col,scale,scale]
// for shader in vec4 pos, in vec3 col, in vec2 scale
shader.compile("./path/to/vertex.shader","./path/to/fragment.shader");
shader.def_attributeF("pos",4,0,9);
shader.def_attributeF("col",3,4,9);
shader.def_attributeF("scale",2,7,9);

// OR
// setup for general 2D structure
// [position,position,texture coordinate,texture coordinate]
shader.compile2d("./path/to/vertex.shader","./path/to/fragment.shader");

// OR
// setup for general 3D structure
// [position,position,position,texture coordinate,texture coordinate,normal,normal,normal,
//  tangent,tangent,tangent,bitangent,bitangent,bitangent]
shader.compile3d("./path/to/vertex.shader","./path/to/fragment.shader");
```

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/shader.cpp) \
[Parameter Represented By Camera2D](camera2d.md) \
[Parameter Represented By Camera3D](camera3d.md) \
[Pipeline Specifics Relate to Buffer](buffer.md)

<br>

***

<br>

## Bugs & Issues

- join compile & construction & split common attribute definitions from standard process || save enable
- delete shaders after programm attachment
- lingering unsigned integer as return value
- colour setting for console output only viable for linux, returns gibberish on windows
- feature for optional geometry shader has to be added