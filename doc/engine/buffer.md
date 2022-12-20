# Buffer

## Purpose

A Buffer allows storage of vertex information in vertex array & vertex buffer objects.
Buffer creation is absolutely necessary to use stored vertices for rendering,
it is one of the most important components. \
Optionally, an index or element buffer object can be added to either render by elements instead of
arrays, or to index a renderable object.

<br>

***

<br>

## Construction

```c++
Buffer buffer = Buffer();
```

<br>

***

<br>

## Features

### Usage

- add_buffer() -> void : create additional buffer for elements or indices
- bind() -> void : bind the buffer before rendering uploaded vertices
- bind_index() -> bind the index or element buffer, if necessary and previously added

### Upload

- upload_vertices(float*,size_t) -> void : upload a vertex array by given size
- upload_vertices(std::vector<float>) -> void : upload a list of vertices
- upload_elements(unsigned int*,size_t) -> void : upload an element array, if buffer was added
- upload_elements(std::vector<unsigned int>)) -> void : upload element list, if buffer was added
- upload_indices(std::vector<float>) -> void : upload indices array according to shader pattern
- upload_indices(vec2*,size_t) -> void : upload indices list according to shader pattern

### Getter

- get_indices() -> GLuint : returns the either the index or the element buffer depending on usage

<br>

***

<br>

## Usage

### Buffer for Stereotypical Render

```c++
// creating vertices for the sake of this example
float verts0[] = { /* insert vertices here */ };				// array vertices
std::vector<float> verts1 = Toolbox::create_sprite_canvas();	// list vertices

// verts0 to test_buffer as array upload example
test_buffer.bind();
test_buffer.upload_vertices(verts0,sizeof(verts0));

// OR: upload vertices in a list instead of an array
test_buffer.bind();
test_buffer.upload_vertices(verts1);

// right before rendering
test_buffer.bind();

// placeholder: shader bind & uniforms
// placeholder: renderer code here with array draw
```
*[Example Uses Toolbox](toolbox.md)*

### Buffer with Elements

```c++
// vertex buffer
std::vector<float> verts0 = Toolbox::create_sprite_canvas(/* insert position, with & height */);
test_buffer.upload_vertices(verts0);

// element buffer upload with array
GLuint elements0[] = { /*insert elements here*/ };
test_buffer.add_buffer();	// add element buffer if not already exists
test_buffer.upload_elements(elements0,sizeof(elements0));

// OR: element buffer upload with list
std::vector<GLuint> elements1 = { /*insert elements here*/ };
test_buffer.add_buffer();	// add element buffer if not already exists
test_buffer.upload_elements(elements1);

// right before rendering
test_buffer.bind();
test_buffer.bind_index();

// placeholder: shader bind & uniforms
// placeholder: renderer code here with element draw

// IF the element buffer is needed for something outside buffer code
GLuint ebo = test_buffer.get_indices();
```
*example uses list upload for vertices, but usage of array upload holds equal validity* \
*warning: *

### Buffer for Indexing

```c++
// vertex buffer
std::vector<float> verts = Toolbox::create_sprite_canvas();
test_buffer.upload_vertices(verts);

// index buffer upload with array
float indices0[] = { /* insert mapped index values here */ };
test_buffer.add_buffer();
test_buffer.upload_indices(indices0,sizeof(indices0));

// OR: index buffer upload with list
std::vector<float> indices1 = { /* insert mapped index values here */ };
test_buffer.add_buffer();
test_buffer.upload_indices(indices1);

// placeholder: shader input mapping for indices

// right before rendering
test_buffer.bind();
test_buffer.bind_index();

// placeholder: shader bind & uniforms
// placeholder: renderer code here with instanced array draw

// IF the index buffer is needed for something outside buffer code
GLuint ibo = test_buffer.get_indices();
```
*example uses list upload for vertices, but usage of array upload holds equal validity* \
*warning: indexing only works with array draw, so no vertices that depend on elements will work* \
*buffer construction has been omitted from all examples due to triviality*

<br>

***

<br>

## Links

[Code Source with Comments](../../ccb/fcn/buffer.cpp) \
[Often Used in Combination with Shader](shader.md) \
[Examples Use Toolbox](toolbox.md)

<br>

***

<br>

## Bugs & Issues

- "unsigned int" has been used for element upload, which does not resonate with the other code.
- both element & index buffer are referred to inaccurately as "index"