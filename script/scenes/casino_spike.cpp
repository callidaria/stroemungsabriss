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
	index_r3D = m_ccbf->r3d->add("./res/casino/test_floor.obj","./res/casino/paquet_colour.png",
			"./res/casino/paquet_spec.png","./res/casino/paquet_normal.png","./res/none.png",
			glm::vec3(),1,glm::vec3(),false);
	m_ccbf->r3d->add("./res/casino/test_entrance.obj","./res/casino/entrance_colour.png",
			"./res/casino/entrance_spec.png","./res/casino/entrance_normal.png","./res/none.png",
			glm::vec3(),1,glm::vec3(),false);
	m_ccbf->r3d->add("./res/casino/test_table.obj","./res/casino/table_colour.png",
			"./res/casino/table_spec.png","./res/casino/table_normal.png","./res/none.png",
			glm::vec3(-2,0,-1),1,glm::vec3(),true);
	m_ccbf->r3d->add("./res/casino/tobject.obj","./res/casino/tobj1_colour.png",
			"./res/casino/tobj1_spec.png","./res/casino/tobj1_normal.png","./res/none.png",
			glm::vec3(2,0,1),1,glm::vec3(),true);
	m_ccbf->r3d->add("./res/casino/rolling.obj","./res/casino/rolling_colour.png",
			"./res/casino/rolling_spec.png","./res/casino/rolling_normal.png","./res/none.png",
			glm::vec3(0,.5f,0),.5f,glm::vec3(),true);
	m_ccbf->r3d->ml[index_r3D+4].model = glm::translate(glm::mat4(1),mv_pos);

	// lighting
	m_ccbf->r3d->create_shadow(glm::vec3(100,150,150),glm::vec3(0),25,25,10,4096);
	set_rigs->lighting.add_sunlight({ glm::vec3(100,150,150),glm::vec3(1),.1f });
	set_rigs->lighting.add_pointlight({ glm::vec3(0,2,0),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(3,2,-3),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(7,.5f,-7),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(7,2,-3),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(7,1.5f,1),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(7,1,5),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(-4.5f,1,4),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(-2.5f,4,-2),glm::vec3(1),1,1,.1f,1 });
	set_rigs->lighting.add_pointlight({ glm::vec3(-2.3f,5,-1.3f),glm::vec3(1),1,1,.1f,1 });
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

	// render flooring
	m_ccbf->r3d->prepare(m_setRigs->cam3D[0]);
	m_ccbf->r3d->upload_shadow();
	m_ccbf->r3d->render_mesh(index_r3D,index_r3D+2);

	// render wordly objects
	m_ccbf->r3d->render_mesh(index_r3D+2,index_r3D+5);
}