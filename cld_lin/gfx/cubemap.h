#pragma once

#include <iostream>
#include <SOIL/SOIL.h>
#include "shader.h"
#include "../mat/camera3d.h"

class Cubemap
{
public:
	Cubemap(std::vector<const char*> tp);
	void prepare();
	void prepare_wcam(Camera3D* c); // !!collapse prepare methods
	void set_cubemap();
	void render();
private:
	unsigned int vao,vbo,tex;
public:
	Shader s;
};
