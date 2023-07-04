#include "particle_system.h"

/*
	TODO
*/
ParticleSystem::ParticleSystem()
{ buffer.add_buffer(); }

/*
	TODO
*/
uint16_t ParticleSystem::add(const char* panim,uint8_t rows,uint8_t cols,uint16_t acnt,float dur,
		bool loop,glm::vec3 opos,float scl,glm::vec3 ddir,float spwn_timeout,uint32_t count)
{
	// entity creation and data
	ParticleEntity pentity;
	pentity.panim = panim;
	pentity.rows = rows;
	pentity.cols = cols;
	pentity.cframes = acnt-1;
	pentity.anim_duration = dur;
	pentity.loop_anim = loop;
	pentity.origin_pos = opos;
	pentity.scale = scl;
	pentity.drive_dir = std::vector<glm::vec3>(count,ddir);
	pentity.count = count;
	pentity.spwn_timeout = spwn_timeout;

	// setup gpu data
	glGenTextures(1,&pentity.texture);
	pentity.indices = std::vector<float>(count*5);
	pentity.anim_timing = std::vector<float>(count);

	// add vertex information
	float hs = scl/2.0f;
	std::vector<float> arverts = {
		-hs,-hs+scl,0,0.0f,0.0f, -hs+scl,-hs,0,1.0f,1.0f, -hs+scl,-hs+scl,0,1.0f,0.0f,
		-hs+scl,-hs,0,1.0f,1.0f, -hs,-hs+scl,0,0.0f,0.0f, -hs,-hs,0,0.0f,1.0f
	}; verts.insert(verts.end(),arverts.begin(),arverts.end());

	// save entity
	entity_list.push_back(pentity);
	return entity_list.size()-1;
}

/*
	TODO
*/
void ParticleSystem::load()
{
	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	shader.compile("./shader/vertex_particle.shader","./shader/fragment_particle.shader");
	shader.def_attributeF("position",3,0,5);
	shader.def_attributeF("texCoords",2,3,5);
	buffer.bind_index();
	shader.def_indexF(buffer.get_indices(),"particle_position",3,0,5);
	shader.def_indexF(buffer.get_indices(),"anim_index",2,3,5);

	// load textures
	for (auto entity : entity_list) Toolbox::load_texture(entity.texture,entity.panim,false);
	shader.upload_int("tex",0);
}

/*
	TODO
*/
void ParticleSystem::prepare(Camera3D cam3D,float delta_time)
{
	// update
	for (uint32_t j=0;j<entity_list.size();j++) {

		// stall activations according to spawn timeout
		entity_list[j].spwn_delta += delta_time;
		bool spawn = entity_list[j].spwn_delta>entity_list[j].spwn_timeout;

		// reset spawn index should maximum entities have been spawned
		if (spawn) {
			entity_list[j].cactive += entity_list[j].cactive<entity_list[j].count;
			entity_list[j].spwn_delta -= entity_list[j].spwn_timeout;
			entity_list[j].sindex -= (entity_list[j].sindex>=entity_list[j].count)
					* entity_list[j].count;
			entity_list[j].indices[entity_list[j].sindex*5] = entity_list[j].origin_pos.x;
			entity_list[j].indices[entity_list[j].sindex*5+1] = entity_list[j].origin_pos.y;
			entity_list[j].indices[entity_list[j].sindex*5+2] = entity_list[j].origin_pos.z;
			entity_list[j].indices[entity_list[j].sindex*5+3] = .0f;
			entity_list[j].indices[entity_list[j].sindex*5+4] = .0f;
			entity_list[j].anim_timing[entity_list[j].sindex] = 0;
			entity_list[j].sindex++;
		}

		// write updates for active instances
		for (uint32_t i=0;i<entity_list[j].cactive;i++) {

			// individual positions
			entity_list[j].indices[i*5] += entity_list[j].drive_dir[i].x;
			entity_list[j].indices[i*5+1] += entity_list[j].drive_dir[i].y;
			entity_list[j].indices[i*5+2] += entity_list[j].drive_dir[i].z;

			// individual animation frames
			entity_list[j].anim_timing[i] += delta_time;
			bool reset_timing = entity_list[j].anim_timing[i]>entity_list[j].anim_duration;
			entity_list[j].anim_timing[i] -= reset_timing
					* (entity_list[j].anim_duration*entity_list[j].loop_anim
					+ (entity_list[j].anim_timing[i]-entity_list[j].anim_duration)
					* !entity_list[j].loop_anim);
			uint16_t anim_index = (entity_list[j].anim_timing[i]/entity_list[j].anim_duration)
					* entity_list[j].cframes;
			entity_list[j].indices[i*5+3] = anim_index%entity_list[j].cols;
			entity_list[j].indices[i*5+4] = anim_index/entity_list[j].cols;
			// FIXME: ??maybe do this in shader and only upload timing value & duration uniform
		}
	}

	// binding
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
	shader.enable();
	buffer.bind();
	shader.upload_camera(cam3D);
}

/*
	TODO
*/
void ParticleSystem::render(uint16_t i)
{
	// setup
	glBindTexture(GL_TEXTURE_2D,entity_list[i].texture);
	buffer.upload_indices(entity_list[i].indices);
	shader.upload_int("anim_rows",entity_list[i].rows);
	shader.upload_int("anim_cols",entity_list[i].cols);
	shader.upload_float("scale",entity_list[i].scale);

	// render particles
	glDrawArraysInstanced(GL_TRIANGLES,i*6,6,entity_list[i].cactive);
}