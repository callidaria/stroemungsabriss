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
	void render_irradiance_to_cubemap(int32_t resolution);
	void approximate_reflectance_integral(int32_t resolution);

	// setup
	void prepare();
	void prepare(Camera3D cam3D);

	// draw
	void render_irradiance();
	void render_approximated();

	// getter
	uint32_t get_irradiance_map();
	uint32_t get_approximation();

private:

	// helper
	void init_buffer();

public:

	// cascabel
	Shader s,approx_irr,irrs;

private:

	// graphics
	Buffer buffer = Buffer();
	uint32_t tex,irr_tex,imap;
};