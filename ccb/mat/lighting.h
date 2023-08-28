#ifndef CCB_MATHEMATICS_LIGHTING
#define CCB_MATHEMATICS_LIGHTING

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../gfx/shader.h"
#include "../gfx/cubemap.h"

// structure parallel to sunlight in deferred shader
struct LightSun {
	glm::vec3 position,colour;
	float intensity;
};

// structure parallel to pointlight in deferred shader
struct LightPoint {
	glm::vec3 position,colour;
	float constant,linear,quadratic,intensity;
};

// structure parallel to spotlight in deferred shader
struct LightSpot {
	glm::vec3 position,colour,direction;
	float cut_in,cut_out;
};

// structure holding globally mapped lighting precalculations
struct LightIrradianceMap {
	uint32_t diff_component;
	uint32_t spec_component;
	uint32_t brdf_component;
};

class Lighting
{
public:

	// construction
	Lighting() {  }
	~Lighting() {  }

	// creation
	void add_sunlight(LightSun sunlight);
	void add_pointlight(LightPoint pointlight);
	void add_spotlight(LightSpot spotlight);

	// extraction
	void load_irradiance_maps(Cubemap imap);

	// upload
	void upload(Shader* shader);
	inline void upload_diffusion_map()
		{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.diff_component); }
	inline void upload_specular_map()
		{ glBindTexture(GL_TEXTURE_CUBE_MAP,irradiance_map.spec_component); }
	inline void upload_specular_brdf()
		{ glBindTexture(GL_TEXTURE_2D,irradiance_map.brdf_component); }

public:

	// lights
	std::vector<LightSun> sunlights;
	std::vector<LightPoint> pointlights;
	std::vector<LightSpot> spotlights;

	// mapping
	LightIrradianceMap irradiance_map;
};

#endif