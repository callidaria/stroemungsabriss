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
	pentity.dists = std::vector<glm::vec3>(count,glm::vec3(0));
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
	// update individual particles
	for (auto pie : entity_list) {

		// stall activations according to spawn timeout
		pie.spwn_delta += delta_time;
		bool spawn = pie.spwn_delta>pie.spwn_timeout;

		// reset spawn index should maximum entities have been spawned
		if (spawn) {
			pie.cactive += pie.cactive<pie.count;
			pie.spwn_delta -= pie.spwn_timeout;
			pie.sindex -= (pie.sindex>=pie.count)
					* pie.count;
			pie.indices[pie.sindex*5] = pie.origin_pos.x;
			pie.indices[pie.sindex*5+1] = pie.origin_pos.y;
			pie.indices[pie.sindex*5+2] = pie.origin_pos.z;
			pie.indices[pie.sindex*5+3] = .0f;
			pie.indices[pie.sindex*5+4] = .0f;
			pie.anim_timing[pie.sindex] = 0;
			pie.sindex++;
		}

		// write updates for active instances
		for (uint32_t i=0;i<pie.cactive;i++) {

			// individual positions
			pie.indices[i*5] += pie.drive_dir[i].x;
			pie.indices[i*5+1] += pie.drive_dir[i].y;
			pie.indices[i*5+2] += pie.drive_dir[i].z;

			// individual animation frames
			pie.anim_timing[i] += delta_time;
			bool reset_timing = pie.anim_timing[i]>pie.anim_duration;
			pie.anim_timing[i] -= reset_timing
					* (pie.anim_duration*pie.loop_anim
					+ (pie.anim_timing[i]-pie.anim_duration)
					* !pie.loop_anim);
			uint16_t anim_index = (pie.anim_timing[i]/pie.anim_duration)
					* pie.cframes;
			pie.indices[i*5+3] = anim_index%pie.cols;
			pie.indices[i*5+4] = anim_index/pie.cols;
			// FIXME: ??maybe do this in shader and only upload timing value & duration uniform

			// calculate instance distances for render queue
			glm::vec3 inst_pos = glm::vec3(pie.indices[i*5],pie.indices[i*5+1],pie.indices[i*5+2]);
			pie.dists[i] = glm::length(inst_pos-cam3D.pos);
			// TODO: test for performance badness, see if it is almost fine and then leave it
		}

		// order instances by depth
		// TODO: store position ids and restore every frame based on renderqueue (sort based on dist)
	}

	// binding
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_DEPTH_TEST);
	shader.enable();
	buffer.bind();
	shader.upload_camera(cam3D);
	// TODO: cleanup and structurize
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