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
	// attributes
	const char* panim;
	uint32_t texture;
	uint8_t rows,cols;
	uint16_t cframes;
	float anim_duration;
	bool loop_anim;

	// transform
	glm::vec3 origin_pos;
	float scale;
	std::vector<glm::vec3> curr_pos,drive_dir;
	std::vector<glm::vec2> curr_aloc;
	std::vector<float> indices;

	// timing & order
	uint32_t count,cactive=0,sindex=0;
	double spwn_timeout,spwn_delta=.0;
	std::vector<float> anim_timing,dists;
	std::vector<uint32_t> queue_order;
};

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

	// update
	void update(Camera3D cam3D,double delta_time);

	// draw
	void prepare(Camera3D cam3D);
	void render(uint16_t i);
	void auto_render(Camera3D cam3D);

	// queue interaction
	inline void enable_update(uint16_t id) { particle_update_ids.push_back(id); }
	inline void disable_update(uint16_t id) {
		particle_update_ids.erase(
				std::remove(particle_update_ids.begin(),particle_update_ids.end(),id),
				particle_update_ids.end()
			);
	}

private:

	// index upload
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
	std::vector<uint16_t> particle_update_ids;
};

#endif
