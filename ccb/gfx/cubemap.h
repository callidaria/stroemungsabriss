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
	void approximate_irradiance(int32_t ri_res,uint32_t re_res,uint8_t lod_count,
			uint16_t sample_count);

	// setup
	void prepare();
	void prepare(Camera3D cam3D);

	// draw
	void render_irradiance();
	void render_diffusion_approximated();
	void render_specular_approximated();

	// getter
	uint32_t get_irradiance_map();
	uint32_t get_diffusion_approximation();
	uint32_t get_specular_approximation();

private:

	// helper
	void init_buffer();

public:

	// cascabel
	Shader s,approx_irr,approx_ref,irrs;

private:

	// graphics
	Buffer buffer = Buffer();
	uint32_t tex,irr_tex,imap,smap;
	uint16_t source_res;
};