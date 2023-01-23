# Mesh

## Purpose

Meshes hold information for 3D objects, read from .obj files and in future will hopefully take over managing their positioning in world space as well. \
Ideally they are created through their respective [Renderer3D](renderer3d.md), and later accessed through the renderers mesh list array.

<br>

***

<br>

## Construction

### Traditional Construction

```c++
// variable setup
const char* path_obj = "./path/to/mesh.obj";		// path to .obj file, holding vertex info
const char* path_texture = "./path/to/mesh.obj";	// path to meshes texture
const char* path_specular = "./path/to/mesh.obj";	// path to meshes specular map
const char* path_normals = "./path/to/mesh.obj";	// path to meshes normal map
const char* path_emission = "./path/to/mesh.obj";	// path to meshes emission map
glm::vec3 position = glm::vec3(10,4,-7);			// set origin position of mesh
float scaling = 4;									// scale mesh to four times it's scale
glm::vec3 rotation = glm::vec3(0,90,0);				// rotate 90 degrees around y-axis
unsigned int render_start;							// variable to index starting vertex

// construction
Mesh mesh = Mesh(path_obj,path_texture,path_specular,path_normals,path_emission,
		position,scaling,rotation,&render_start);
```

### Ideal Construction

```c++
// predefinition
Renderer3D r3d = Renderer3D();

// construction
uint16_t mem_index = r3d.add(path_obj,path_texture,path_specular,path_normals,path_emission,
		position,scaling,rotation);
```
*construction through the [Renderer3D](renderer3d.md) will automatically store the correct vertex index for each loaded object* \
*mem_index holds the index the loaded mesh is to be referred by in object list & when rendering*

<br>

***

<br>

## Features

- texture() -> void : load texture and all manipulation maps

<br>

***

<br>

## Values

### Upload

- (*) v -> std::vector<float> : mesh vertices, extracted from the object file
- (*) tex -> GLuint : bindable texture, holding texture information
- (*) specmap -> GLuint : bindable texture, holding specular map
- (*) normap -> GLuint : bindable texture, holding normal map
- (*) emitmap -> GLuint : bindable texture, holding emission map

### Transformation

- (*) pos -> vec3 : origin position of the mesh, and support vector to all action
- (*) rot -> vec3 : meshes starting rotation
- (*) scl -> float : scaling of the mesh, original scale would be scl=1

### Information

- (*) ofs -> unsigned int : vertex offset to allocate object start when drawcalling
- (*) size -> unsigned int : amount of extracted and stored vertices

<br>

***

<br>

## Usage

*for intended usage and drawing of meshes please direct yourself [here](renderer3d.md)*

<br>

***

<br>

## Links

[Source Code](../../ccb/gfx/mesh.cpp) \
[Main Feature Component of Renderer3D](renderer3D.md)

<br>

***

<br>

## Bugs & Issues

- modifying offset vectors is fine and good but not very convenient or efficient. add dynamic model matrix modification
- uses "unsigned int" and even as !pointer!, this has to be changed
- open fixmes & todos