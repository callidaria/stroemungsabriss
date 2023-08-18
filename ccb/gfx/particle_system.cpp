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
	pentity.curr_pos = std::vector<glm::vec3>(count,opos);
	pentity.drive_dir = std::vector<glm::vec3>(count,ddir);
	pentity.dists = std::vector<float>(count);
	pentity.queue_order = std::vector<uint16_t>(count);
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
	for (uint16_t pie_id=0;pie_id<entity_list.size();pie_id++) {
		ParticleEntity &pie = entity_list[pie_id];

		// reset spawn index should maximum entities have been spawned
		pie.spwn_delta += delta_time;
		if (pie.spwn_delta>pie.spwn_timeout) {
			pie.cactive += pie.cactive<pie.count;
			pie.spwn_delta -= pie.spwn_timeout;
			pie.sindex -= (pie.sindex>=pie.count)*pie.count;
			pie.curr_pos[pie.sindex] = pie.origin_pos;
			upload_animloc(pie_id,pie.sindex,glm::vec2(0));
			pie.anim_timing[pie.sindex] = 0;
			pie.sindex++;
		}

		// updates for active instances
		for (uint32_t i=0;i<pie.cactive;i++) {

			// individual positions & instance distances for render queue
			pie.curr_pos[i] += pie.drive_dir[i];
			pie.dists[i] = glm::length(pie.curr_pos[i]-cam3D.pos);
			// TODO: test for performance badness, see if it is almost fine and then leave it

			// individual animation frames
			pie.anim_timing[i] += delta_time;
			bool reset_timing = pie.anim_timing[i]>pie.anim_duration;
			pie.anim_timing[i] -= reset_timing*(pie.anim_duration*pie.loop_anim
					+ (pie.anim_timing[i]-pie.anim_duration)*!pie.loop_anim);
			uint16_t anim_index = (pie.anim_timing[i]/pie.anim_duration)*pie.cframes;
			upload_animloc(pie_id,i,glm::vec2(anim_index%pie.cols,anim_index/pie.cols));
			// FIXME: ??maybe do this in shader and only upload timing value & duration uniform
		}

		// order instances by depth
		std::iota(pie.queue_order.begin(),pie.queue_order.end(),0);
		std::sort(pie.queue_order.begin(),pie.queue_order.end(),
				[&](uint16_t n,uint16_t k) { return pie.dists[n]>pie.dists[k]; });
		for (auto iter : pie.queue_order) upload_position(pie_id,iter,pie.curr_pos[iter]),
				std::cout << pie.dists[iter] << ',';
		std::cout << '\n';
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

/*
	TODO
*/
void ParticleSystem::upload_position(uint16_t eid,uint32_t iid,glm::vec3 pos)
{
	entity_list[eid].indices[iid*5] = pos.x,entity_list[eid].indices[iid*5+1] = pos.y,
			entity_list[eid].indices[iid*5+2] = pos.z;
}

/*
	TODO
*/
void ParticleSystem::upload_animloc(uint16_t eid,uint32_t iid,glm::vec2 aid)
{ entity_list[eid].indices[iid*5+3] = aid.x,entity_list[eid].indices[iid*5+4] = aid.y; }