#include "particle_system.h"

/*
	TODO
*/
ParticleSystem::ParticleSystem()
{ buffer.add_buffer(); }

/*
	TODO
*/
uint16_t ParticleSystem::add(const char* panim,glm::vec3 opos,float scl,glm::vec3 ddir,
		float spwn_timeout,uint32_t count)
{
	// entity creation and data
	ParticleEntity pentity;
	pentity.panim = panim;
	pentity.origin_pos = opos;
	pentity.drive_dir = ddir;
	pentity.count = count;
	pentity.spwn_timeout = spwn_timeout;

	// setup gpu data
	glGenTextures(1,&pentity.texture);
	pentity.indices = std::vector<float>(count*3);
	for (uint32_t i=0;i<count;i++) {
		pentity.indices[i*3] = opos.x;
		pentity.indices[i*3+1] = opos.y;
		pentity.indices[i*3+2] = opos.z;
	}

	// add vertex information
	float hs = -scl/2.0f;
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
	shader.def_indexF(buffer.get_indices(),"particle_position",3,0,3);

	// load textures
	for (auto entity : entity_list) Toolbox::load_texture(entity.texture,entity.panim,true);
	shader.upload_int("tex",0);
}

/*
	TODO
*/
void ParticleSystem::prepare(Camera3D cam3D,float delta_time)
{
	// update
	for (uint32_t j=0;j<entity_list.size();j++) {

		// TODO: reset spawn index when cactive > count

		// stall activations according to spawn timeout
		entity_list[j].spwn_delta += delta_time;
		bool spawn = entity_list[j].spwn_delta>entity_list[j].spwn_timeout;
		entity_list[j].cactive += spawn;
		entity_list[j].spwn_delta -= entity_list[j].spwn_timeout*spawn;

		// write position & animation updates for active instances
		for (uint32_t i=0;i<entity_list[j].cactive;i++) {
			entity_list[j].indices[i*3] += entity_list[j].drive_dir.x;
			entity_list[j].indices[i*3+1] += entity_list[j].drive_dir.y;
			entity_list[j].indices[i*3+2] += entity_list[j].drive_dir.z;
		}
	}

	// binding
	glActiveTexture(GL_TEXTURE0);
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

	// render particles
	glDrawArraysInstanced(GL_TRIANGLES,i*6,6,entity_list[i].cactive);
}

/*
	TODO
*/
void ParticleSystem::set_wind_direction(glm::vec3 wdir)
{ wind_direction = wdir; }