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
	uint8_t rows,cols;
	uint16_t cframes;
	float anim_duration;
	std::vector<float> anim_timing;
	glm::vec3 origin_pos;
	std::vector<glm::vec3> drive_dir;
	std::vector<float> indices;
	uint32_t count,cactive=0,sindex=0;
	float spwn_timeout,spwn_delta=.0f;
};

class ParticleSystem
{
public:

	// construction
	ParticleSystem();
	~ParticleSystem() {  }

	// load
	uint16_t add(const char* panim,uint8_t rows,uint8_t cols,uint16_t acnt,float dur,glm::vec3 opos,
			float scl,glm::vec3 ddir,float spwn_timeout,uint32_t count);
	void load();

	// draw
	void prepare(Camera3D cam3D,float delta_time);
	void render(uint16_t i);

public:

	// entities
	std::vector<ParticleEntity> entity_list;

private:

	// engine
	Buffer buffer = Buffer();
	Shader shader = Shader();

	// data
	std::vector<float> verts;
};

#endif