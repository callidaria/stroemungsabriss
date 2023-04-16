#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "../gfx/shader.h"

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

	// upload
	void upload(Shader* shader);

public:

	// lights
	std::vector<LightSun> sunlights;
	std::vector<LightPoint> pointlights;
	std::vector<LightSpot> spotlights;
};