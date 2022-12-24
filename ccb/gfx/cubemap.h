#pragma once

#include <iostream>
#ifdef __WIN32__
#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif
#else
#include <SOIL/SOIL.h>
#endif
#include "shader.h"
#include "../mat/camera3d.h"
#include "../fcn/buffer.h"

class Cubemap
{
public:
	Cubemap(std::vector<const char*> tp);
	void prepare();
	void prepare_wcam(Camera3D* c); // !!collapse prepare methods
	void set_cubemap();
	void render();
private:
	Buffer buffer;
	unsigned int tex;
public:
	Shader s;
};
