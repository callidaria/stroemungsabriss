#include "particle_system.h"

/*
	TODO
*/
ParticleSystem::ParticleSystem()
{ buffer.add_buffer(); }

/*
	TODO
*/
void ParticleSystem::add(const char* panim,glm::vec3 opos,glm::vec3 ddir,float scl,uint32_t count)
{
	// entity creation and data
	ParticleEntity pentity;
	pentity.panim = panim;
	pentity.origin_pos = opos;
	pentity.drive_dir = ddir;
	pentity.count = count;

	// setup gpu data
	glGenTextures(1,&pentity.texture);
	pentity.indices = std::vector<float>(count*3);

	// add vertex information
	float hs = -scl/2.0f;
	std::vector<float> arverts = {
		-hs,-hs+scl,0,0.0f,0.0f, -hs+scl,-hs,0,1.0f,1.0f, -hs+scl,-hs+scl,0,1.0f,0.0f,
		-hs+scl,-hs,0,1.0f,1.0f, -hs,-hs+scl,0,0.0f,0.0f, -hs,-hs,0,0.0f,1.0f
	}; verts.insert(verts.end(),arverts.begin(),arverts.end());

	// save entity
	entity_list.push_back(pentity);
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
void ParticleSystem::prepare(Camera3D cam3D)
{
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
	glDrawArraysInstanced(GL_TRIANGLES,i*6,6,entity_list[i].count);
}

/*
	TODO
*/
void ParticleSystem::set_wind_direction(glm::vec3 wdir)
{ wind_direction = wdir; }