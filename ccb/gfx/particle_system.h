#ifndef CCB_GRAPHICS_PARTICLE_SYSTEM
#define CCB_GRAPHICS_PARTICLE_SYSTEM

#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../fcn/buffer.h"
#include "../mat/toolbox.h"
#include "shader.h"

// holds data, offsets & behaviour of particle emission
struct ParticleEntity
{
	const char* panim;
	uint32_t texture;
	glm::vec3 origin_pos,drive_dir;
	std::vector<float> indices;
	uint32_t count;
};

class ParticleSystem
{
public:

	// construction
	ParticleSystem();
	~ParticleSystem() {  }

	// load
	void add(const char* panim,glm::vec3 opos,glm::vec3 ddir,float scl,uint32_t count);
	void load();

	// draw
	void prepare(Camera3D cam3D);
	void render(uint16_t i);

	// setter
	void set_wind_direction(glm::vec3 wdir);

public:

	// entities
	std::vector<ParticleEntity> entity_list;

private:

	// engine
	Buffer buffer = Buffer();
	Shader shader = Shader();

	// data
	std::vector<float> verts;

	// information
	glm::vec3 wind_direction = glm::vec3(0);
};

#endif