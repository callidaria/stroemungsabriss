#include "casino_spike.h"

/*
	constructor(CascabelBaseFeature*,StageSetup*)
	ccbf: all of cascabel's most prized features
	set_rigs: stage utility setup
	purpose: create dilapidated casino scene
*/
CasinoSpike::CasinoSpike(CascabelBaseFeature* ccbf,StageSetup* set_rigs)
	: m_ccbf(ccbf),m_setRigs(set_rigs)
{
	// object loading
	index_p3D = m_ccbf->r3d->add("./res/casino/test_floor.obj","./res/casino/paquet_colour.png",
			"./res/casino/paquet_normal.png","./res/casino/paquet_material.png",glm::vec3(),
			1,glm::vec3(),false);
	texture_repeat.push_back(1);
	m_ccbf->r3d->add("./res/casino/test_entrance.obj","./res/casino/entrance_colour.png",
			"./res/casino/entrance_normal.png","./res/casino/entrance_material.png",glm::vec3(),
			1,glm::vec3(),false);
	texture_repeat.push_back(1);
	m_ccbf->r3d->add("./res/casino/test_table.obj","./res/casino/table_colour.png",
			"./res/casino/table_normal.png","./res/casino/table_material.png",glm::vec3(-2,0,-1),
			1,glm::vec3(),true);
	texture_repeat.push_back(10);
	m_ccbf->r3d->add("./res/casino/rolling.obj","./res/casino/tobj1_colour.png",
			"./res/casino/tobj1_normal.png","./res/casino/tobj1_material.png",glm::vec3(2,1,1),
			1,glm::vec3(),true);
	texture_repeat.push_back(4);
	m_ccbf->r3d->add("./res/casino/rolling.obj","./res/casino/pbr/colour.png",
			"./res/casino/pbr/normals.png","./res/casino/pbr/material.png",glm::vec3(2,.5f,-3),.5f,
			glm::vec3(),true);
	texture_repeat.push_back(1);
	m_ccbf->r3d->add("./res/casino/rolling.obj","./res/casino/fabric_colour.png",
			"./res/casino/fabric_normals.png","./res/casino/fabric_material.png",
			glm::vec3(2.5f,.5f,-1.5f),.5f,glm::vec3(),true);
	texture_repeat.push_back(1);

	// lighting
	glm::vec3 light_pos[] = {
		glm::vec3(0,2,0),glm::vec3(3,2,-3),glm::vec3(7,.25f,-7),glm::vec3(7,.25f,-3),
		glm::vec3(7,.25f,1),glm::vec3(7,.25f,5),glm::vec3(-4.5f,1,4),glm::vec3(-2.5f,4,-2),
		glm::vec3(-2.3f,5,-1.3f)
	}; index_r3D = m_ccbf->r3d->ml.size();
	for (uint8_t i=0;i<9;i++) {
		m_ccbf->r3d->add("./res/casino/rolling.obj","./res/all.png","./res/none.png",
			"./res/dnormal.png","./res/all.png",light_pos[i],.25f,glm::vec3(),true);
		set_rigs->lighting.add_pointlight({ light_pos[i],glm::vec3(1),1,1,.1f,1 });
	} oheights[0] = 5.5f,oheights[1] = 7,oheights[2] = 6.5f,oheights[3] = 6;

	// lighting
	m_ccbf->r3d->create_shadow(glm::vec3(150,125,-100),glm::vec3(0),25,25,10,4096);

#ifdef PRERENDER_IRRADIANCE_MAPPING
	irradiance_map.render_irradiance_to_cubemap(1024);
	irradiance_map.approximate_irradiance(32,128,7,2048);
#else
	irradiance_map.load_irradiance_cube();
	irradiance_map.load_irradiance_maps(7);
#endif
	//set_rigs->lighting.add_sunlight({ glm::vec3(100,150,150),glm::vec3(1),1.0f });
	//set_rigs.lighting.add_spotlight({ glm::vec3(0,2,0),glm::vec3(1),glm::vec3(0,-1,0),.5f,.2f });
}

/*
	render() -> void (virtual)
	purpose: render casino scene
*/
void CasinoSpike::render()
{
	// camera front handling
	pitch += m_ccbf->frame->kb.ka[SDL_SCANCODE_I],pitch -= m_ccbf->frame->kb.ka[SDL_SCANCODE_K];
	yaw += m_ccbf->frame->kb.ka[SDL_SCANCODE_L],yaw -= m_ccbf->frame->kb.ka[SDL_SCANCODE_J];
	cp_dir.x = glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw));
	cp_dir.y = glm::sin(glm::radians(pitch));
	cp_dir.z = glm::cos(glm::radians(pitch))*glm::sin(glm::radians(yaw));

	// camera position handling
	cp_pos += cp_dir*glm::vec3(.05f*m_ccbf->frame->kb.ka[SDL_SCANCODE_W]);
	cp_pos -= cp_dir*glm::vec3(.05f*m_ccbf->frame->kb.ka[SDL_SCANCODE_S]);
	cp_pos += glm::normalize(glm::cross(cp_dir,glm::vec3(0,1,0)))
			* glm::vec3(.05f*m_ccbf->frame->kb.ka[SDL_SCANCODE_D]);
	cp_pos -= glm::normalize(glm::cross(cp_dir,glm::vec3(0,1,0)))
			* glm::vec3(.05f*m_ccbf->frame->kb.ka[SDL_SCANCODE_A]);
	cp_pos.y += m_ccbf->frame->kb.ka[SDL_SCANCODE_R]*.05f;
	cp_pos.y -= m_ccbf->frame->kb.ka[SDL_SCANCODE_F]*.05f;

	// camera update
	m_setRigs->cam3D[0].pos = cp_pos,m_setRigs->cam3D[0].front = cp_dir;
	m_setRigs->cam3D[0].update();

	// update physics
	for (uint8_t i=0;i<4;i++) {
		m_ccbf->r3d->ml[index_r3D+2+i].model
				= glm::translate(glm::mat4(1),glm::vec3(0,oheights[i],0));
		m_setRigs->lighting.pointlights[2+i].position.y = oheights[i]+.25f;
		ospeed[i] *= 1-2*(oheights[i]<0);
		oheights[i] += ospeed[i];
		ospeed[i] -= .0009f;
	}

	// render flooring
	m_ccbf->r3d->prepare(m_setRigs->cam3D[0]);
	//m_ccbf->r3d->upload_shadow();
	m_ccbf->r3d->render_mesh(index_r3D,index_r3D+9);

	// render wordly objects
	//m_ccbf->r3d->render_mesh(index_r3D+2,index_r3D+5);

	// render physical based objects
	m_ccbf->r3d->prepare_pmesh(m_setRigs->cam3D[0]);
	//m_ccbf->r3d->upload_shadow_pmsh();
	for (uint8_t i=0;i<6;i++) {
		m_ccbf->r3d->pbms.upload_float("tex_repeat",texture_repeat[i]);
		m_ccbf->r3d->render_pmsh(index_p3D+i);
	}

	// render irradiance
	irradiance_map.prepare(m_setRigs->cam3D[0]);
	irradiance_map.render_irradiance();
	m_setRigs->lighting.load_irradiance_maps(irradiance_map);
}