# Framebuffer

## Purpose

Allows to render an environment to a texture and save for later after applying effects.

<br>

***

<br>

## Construction

```c++
// variable setup
uint32_t fr_width = 1920;						// frame buffer width
uint32_t fr_height = 1080;						// frame buffer height
uint32_t fr_wres = 1280;						// frame buffer resolution width
uint32_t fr_hres = 720;							// frame buffer resolution height
const char* vsp = "./path/to/vertex.shader";	// vertex shader path
const char* fsp = "./path/to/fragment.shader";	// framebuffer shader path
bool float_buffer = false;						// definition of float buffer (e.g. for hdr)

// construction
FrameBuffer test_framebuffer0 = FrameBuffer(fr_width,fr_height,vsp,fsp,float_buffer);
FrameBuffer test_framebuffer1 =
		FrameBuffer(fr_width,fr_height,fr_wres,fr_hres,vsp,fsp,float_buffer);
```
*the only difference between the two framebuffers is the texture resolution (1920x1080 <-> 1280x720)*

<br>

***

<br>

## Features

### Read

- bind() -> void : bind framebuffer to start rendering to texture
- close() -> void (static) : unbind any framebuffer and stop rendering to texture

### Render

- render() -> void : render previously rendered-to texture
- render(float) -> void (exclusive) : transition between sepia vignette & modern style

### Getter

- get_fbo() -> GLuint : return frame buffer object
- get_tex() -> GLuint : return frame buffer texture

<br>

***

<br>

## Usage

```c++
// an environment has to be rendered to a texture, to start this the framebuffer is bound
fbuffer.bind();

// placeholder: render environment that belongs onto the framebuffer texture

// the environment has been rendered completely, ending render to texture
FrameBuffer::close();

// another feature has to use the framebuffer, returning so this can happen
GLuint framebuffer_object = fbuffer.get_fbo();
// placeholder: feature that uses framebuffer_object

// the framebuffer texture plays a role in a shader effect and has to be used externally
GLuint framebuffer_texture = fbuffer.get_tex();
// placeholder: feature that uses framebuffer_texture

// placeholder: opening main loop

// the framebuffer texture we previously have rendered to has to be rendered itself now
fbuffer.render();
```
*fbuffer is the product of FrameBuffer construction, which has been omitted for the sake of brevity*

<br>

***

<br>

## Links

[Source Code](../../ccb/frm/framebuffer.cpp)

<br>

***

<br>

## Bugs & Issues

- opened todos and fixmes