#ifndef CCB_GRAPHICS_CUBEMAP
#define CCB_GRAPHICS_CUBEMAP

#include <iostream>
#include <filesystem>

#include <glm/glm.hpp>

#include "../mat/camera3d.h"
#include "../mat/toolbox.h"

#include "../fcn/buffer.h"

#include "shader.h"
#include "texture.h"

class Cubemap
{
public:

	// construction
	Cubemap(const char* path);
	Cubemap(std::vector<const char*> tp);
	~Cubemap() {  }

	// preprocess
	void render_irradiance_to_cubemap(std::string id,int32_t resolution);
	void approximate_irradiance(std::string id,int32_t ri_res,uint32_t re_res,uint8_t lod_count,
			uint16_t sample_count);

	// load preprocess
	void load_irradiance_cube(std::string id);
	void load_irradiance_maps(std::string id,uint8_t lod_count);
	void dynamic_precalculation_load_switch(std::string id,int32_t resolution,
			int32_t ri_res,uint32_t re_res,uint8_t lod_count,uint16_t sample_count);

	// setup
	void prepare();
	void prepare(Camera3D cam3D);

	// draw
	void render_irradiance();
	void render_diffusion_approximated();
	void render_specular_approximated();

private:

	// helper
	void init_buffer();
	void init_cubemap_texture(uint16_t res);

public:

	// cascabel
	Shader s,approx_irr,approx_ref,pc_specular,irrs;

	// irradiance maps
	Texture specbrdf;
	uint32_t irradiance_map,diffusion_approx,specular_approx;
	// TODO: transition from direct unsigned int usage to texture
	// TODO: combine multiple texture into an array that can be generated together

private:

	// graphics
	Buffer buffer = Buffer(),cnv_buffer = Buffer();
	uint32_t cmfbo,cmrbo;
	uint32_t irr_tex;
	uint16_t source_res;

	// math
	glm::mat4 cam_attrib[6];
	glm::mat4 cam_proj = glm::perspective(glm::radians(90.0f),1.0f,1.0f,3.0f);
};

#endif