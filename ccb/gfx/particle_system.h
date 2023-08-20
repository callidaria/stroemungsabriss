#ifndef CCB_GRAPHICS_PARTICLE_SYSTEM
#define CCB_GRAPHICS_PARTICLE_SYSTEM

#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>

#include <glm/glm.hpp>

#include "../fcn/buffer.h"
#include "../mat/toolbox.h"
#include "shader.h"

constexpr uint8_t PARTICLE_SYSTEM_INDEX_RANGE = 5;

// holds data, offsets & behaviour of particle emission
struct ParticleEntity
{
	const char* panim;
	uint32_t texture;
	uint8_t rows,cols;
	uint16_t cframes;
	float anim_duration;
	std::vector<float> anim_timing,dists;
	std::vector<uint32_t> queue_order;
	bool loop_anim;
	glm::vec3 origin_pos;
	float scale;
	std::vector<glm::vec3> curr_pos,drive_dir;
	std::vector<glm::vec2> curr_aloc;
	std::vector<float> indices;
	uint32_t count,cactive=0,sindex=0;
	double spwn_timeout,spwn_delta=.0;
};
// TODO: clean this up!

class ParticleSystem
{
public:

	// construction
	ParticleSystem();
	~ParticleSystem() {  }

	// load
	uint16_t add(const char* panim,uint8_t rows,uint8_t cols,uint16_t acnt,float dur,bool loop,
			glm::vec3 opos,float scl,glm::vec3 ddir,double spwn_timeout,uint32_t count);
	void load();

	// draw
	void prepare(Camera3D cam3D,double delta_time);
	void render(uint16_t i);

private:

	// index uploads
	void upload_position(uint16_t eid,uint32_t iid,glm::vec3 pos);
	void upload_animloc(uint16_t eid,uint32_t iid,glm::vec2 aid);

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