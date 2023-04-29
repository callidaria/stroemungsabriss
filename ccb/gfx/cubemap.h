#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../mat/camera3d.h"
#include "../mat/toolbox.h"

#include "../fcn/buffer.h"
#include "shader.h"

class Cubemap
{
public:

	// construction
	Cubemap(const char* path);
	Cubemap(std::vector<const char*> tp);
	~Cubemap() {  }

	// preprocess
	void render_irradiance_to_cubemap();

	// setup
	void prepare();
	void prepare_irradiance();
	void prepare_wcam(Camera3D* c); // !!collapse prepare methods
	void prepare_irradiance_wcam(Camera3D cam3D);

	// draw
	void render();
	void render_irradiance();

private:

	// helper
	void init_buffer();

public:

	// cascabel
	Shader s,irrs;

private:

	// graphics
	Buffer buffer = Buffer();
	uint32_t tex,irr_tex;
};