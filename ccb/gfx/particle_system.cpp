#include "particle_system.h"

/*
	constructor() !O(1)
	purpose: setup particle system
*/
ParticleSystem::ParticleSystem()
{ buffer.add_buffer(); }

/*
	add(const char*,uint8_t,uint8_t,uint16_t,float,bool,opos,float,vec3,double,uint32_t)
			-> uint16_t !O(1)
	purpose: add particle type to spawn instances of
	\param panim: path to animation spritesheet
	\param rows: rows on animation spritesheet
	\param cols: cols on animation spritesheet
	\param acnt: animation frame count stored in spritesheet
	\param dur: duration to fully complete animation in seconds
	\param loop: if animation should loop after completion
	\param opos: origin position to spawn particle instances from
	\param scl: scale of particle instances
	\param ddir: drift direction of particles
	\param spwn_timeout: time that passes between new particle spawns in seconds
	\param count: maximum amount of particle instances
	\returns particle type memory index, to reference particle type by, after creation
*/
uint16_t ParticleSystem::add(const char* panim,uint8_t rows,uint8_t cols,uint16_t acnt,float dur,
		bool loop,glm::vec3 opos,float scl,glm::vec3 ddir,double spwn_timeout,uint32_t count)
{
	// entity creation and data
	ParticleEntity pentity = {
		.panim = panim,
		.rows = rows,
		.cols = cols,
		.cframes = acnt-1,
		.anim_duration = dur,
		.loop_anim = loop,
		.origin_pos = opos,
		.scale = scl,
		.curr_pos = std::vector<glm::vec3>(count,opos),
		.drive_dir = std::vector<glm::vec3>(count,ddir),
		.curr_aloc = std::vector<glm::vec2>(count),
		.indices = std::vector<float>(count*PARTICLE_SYSTEM_INDEX_RANGE),
		.count = count,
		.spwn_timeout = spwn_timeout,
		.anim_timing = std::vector<float>(count),
		.dists = std::vector<float>(count),
		.queue_order = std::vector<uint32_t>(count)
	};

	// particle queue order
	std::iota(pentity.queue_order.begin(),pentity.queue_order.end(),0);
	// TODO: replace iota with something more readable & controllable than modern c++ lingo

	// create texture
	glGenTextures(1,&pentity.texture);

	// add vertex information
	float hs = scl*.5f;
	size_t t_vsize = verts.size();
	verts.resize(verts.size()+PATTERN_SPRITE_TRIANGLE_REPEAT);
	Toolbox::create_sprite_canvas_triangled(verts,t_vsize,glm::vec2(-hs,-hs),scl,scl);

	// save entity
	entity_list.push_back(pentity);
	return entity_list.size()-1;
}

/*
	load() -> void !O(n)
	purpose: load all created particle types
*/
void ParticleSystem::load()
{
	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(verts);

	// compile shader
	shader.compile("./shader/vertex_particle.shader","./shader/fbf_standard.shader");
	shader.def_attributeF("position",3,0,5);
	shader.def_attributeF("texCoords",2,3,5);
	buffer.bind_index();
	shader.def_indexF("particle_position",3,0,5);
	shader.def_indexF("anim_index",2,3,5);

	// load textures
	for (auto entity : entity_list) Toolbox::load_texture(entity.texture,entity.panim,false);
	shader.upload_int("tex",0);
}

/*
	update(Camera3D,double) -> void ~O(n)b<-O(?std::sort())
	purpose: update particle animations & positioning & setup render queue
	\param cam3D: camera, used to render the scene
	\param delta_time: time passed since last update
*/
void ParticleSystem::update(Camera3D cam3D,double delta_time)
{
	// calculate camera plane
	glm::vec3 pform = glm::cross(glm::vec3(cam3D.view3D[0][1],cam3D.view3D[1][1],cam3D.view3D[2][1]),
			glm::vec3(cam3D.view3D[0][0],cam3D.view3D[1][0],cam3D.view3D[2][0]));
	float pform_a = cam3D.pos.x*pform.x+cam3D.pos.y*pform.y+cam3D.pos.z*pform.z;

	// update individual particles
	for (auto pie_id : particle_update_ids) {
		ParticleEntity &pie = entity_list[pie_id];

		// reset spawn index should maximum entities have been spawned
		pie.spwn_delta += delta_time;
		if (pie.spwn_delta>pie.spwn_timeout) {
			pie.cactive += pie.cactive<pie.count;
			pie.spwn_delta -= pie.spwn_timeout;
			pie.sindex -= (pie.sindex>=pie.count)*pie.count;
			pie.curr_pos[pie.sindex] = pie.origin_pos;
			pie.curr_aloc[pie.sindex] = glm::vec2(0);
			pie.anim_timing[pie.sindex] = 0;
			pie.sindex++;
		}

		// updates for active instances
		for (uint32_t i=0;i<pie.cactive;i++) {

			// individual positions & instance distances for render queue
			pie.curr_pos[i] += pie.drive_dir[i];
			pie.dists[i] = glm::abs(pform.x*pie.curr_pos[i].x+pform.y*pie.curr_pos[i].y
					+ pform.z*pie.curr_pos[i].z-pform_a)
					/ glm::sqrt(pow(pform.x,2)+pow(pform.y,2)+pow(pform.z,2));
			// TODO: test for performance badness, see if it is almost fine and then leave it

			// individual animation frames
			pie.anim_timing[i] += delta_time;
			bool reset_timing = pie.anim_timing[i]>pie.anim_duration;
			pie.anim_timing[i] -= reset_timing*(pie.anim_duration*pie.loop_anim
					+ (pie.anim_timing[i]-pie.anim_duration)*!pie.loop_anim);
			uint16_t anim_index = (pie.anim_timing[i]/pie.anim_duration)*pie.cframes;
			pie.curr_aloc[i] = glm::vec2(anim_index%pie.cols,anim_index/pie.cols);
		}

		// order instances by depth
		std::sort(pie.queue_order.begin(),pie.queue_order.end(),
				[&](uint16_t n,uint16_t k) { return pie.dists[n]>pie.dists[k]; });
		for (uint32_t i=0;i<pie.cactive;i++)
			upload_position(pie_id,i,pie.curr_pos[pie.queue_order[i]]),
					upload_animloc(pie_id,i,pie.curr_aloc[pie.queue_order[i]]);
		// TODO: write a more specific implementation
	}
}

/*
	prepare(Camera3D) -> void !O(1)
	purpose: prepare particle rendering
	\param cam3D: camera, used to render the scene
*/
void ParticleSystem::prepare(Camera3D cam3D)
{
	shader.enable();
	buffer.bind();
	shader.upload_camera(cam3D);
}

/*
	render(uint16_t) -> void !O(1)
	purpose: draw particle instances of requested particle type
	\param i: particle type memory index, given at creation
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
	render(Camera3D) -> void !O(n)
	purpose: automatically prepare & render all activated particle types
	\param cam3D: camera, used to render the scene
*/
void ParticleSystem::auto_render(Camera3D cam3D)
{
	prepare(cam3D);
	for (auto id : particle_update_ids) render(id);
}

/*
	upload_position(uint16_t,uint32_t,vec3) -> void (private) !O(1)
	purpose: helper function to upload position vector to shader indices list
	\param eid: particle type entity id
	\param iid: particle instance id
	\param pos: position vector to upload
*/
void ParticleSystem::upload_position(uint16_t eid,uint32_t iid,glm::vec3 pos)
{
	entity_list[eid].indices[iid*PARTICLE_SYSTEM_INDEX_RANGE] = pos.x,
	entity_list[eid].indices[iid*PARTICLE_SYSTEM_INDEX_RANGE+1] = pos.y,
	entity_list[eid].indices[iid*PARTICLE_SYSTEM_INDEX_RANGE+2] = pos.z;
}

/*
	upload_animloc(uint16_t,uint32_t,vec2) -> void (private) !O(1)
	purpose: helper function to upload sprite location on spritesheet to shader indices list
	\param eid: particle type entity id
	\param iid: particle instance id
	\param aid: animation sprite location on animation spritesheet
*/
void ParticleSystem::upload_animloc(uint16_t eid,uint32_t iid,glm::vec2 aid)
{
	entity_list[eid].indices[iid*PARTICLE_SYSTEM_INDEX_RANGE+3] = aid.x,
	entity_list[eid].indices[iid*PARTICLE_SYSTEM_INDEX_RANGE+4] = aid.y;
}
