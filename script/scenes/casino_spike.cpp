#include "casino_spike.h"

/*
	constructor(CascabelBaseFeature*)
	ccbf: all of cascabel's most prized features
	purpose: create dilapidated casino scene
*/
CasinoSpike::CasinoSpike(CascabelBaseFeature* ccbf)
	: m_ccbf(ccbf)
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

	// material definitions
	m0 = Material3D(m_ccbf->r3d,2.5f,8,.25f);
	m1 = Material3D(m_ccbf->r3d,1.5f,64,2);
}

/*
	TODO
*/
void CasinoSpike::shadow()
{
	m_ccbf->r3d->render_mesh(index_r3D+2,index_r3D+4);
	m_ccbf->r3d->s3d.upload_matrix("model",glm::translate(glm::mat4(1),mv_pos));
	m_ccbf->r3d->render_mesh(index_r3D+4,index_r3D+5);
	m_ccbf->r3d->s3d.upload_matrix("model",glm::mat4(1));
}

/*
	render() -> void (virtual)
	purpose: render casino scene
*/
void CasinoSpike::render(Camera3D &cam3D)
{
	// camera front handling
	pitch += m_ccbf->frame->kb.ka[SDL_SCANCODE_I];
	pitch -= m_ccbf->frame->kb.ka[SDL_SCANCODE_K];
	yaw += m_ccbf->frame->kb.ka[SDL_SCANCODE_L];
	yaw -= m_ccbf->frame->kb.ka[SDL_SCANCODE_J];
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
	cam3D.pos = cp_pos,cam3D.front = cp_dir;
	cam3D.update();

	// render flooring
	// m_ccbf->r3d->prepare(cam3D);
	//m0.upload();
	m_ccbf->r3d->render_mesh(index_r3D,index_r3D+2);

	// render wordly objects
	//m1.upload();
	m_ccbf->r3d->render_mesh(index_r3D+2,index_r3D+4);
	//m_ccbf->r3d->s3d.upload_matrix("model",glm::translate(glm::mat4(1),mv_pos));
	m_ccbf->r3d->render_mesh(index_r3D+4,index_r3D+5);
	//m_ccbf->r3d->s3d.upload_matrix("model",glm::mat4(1));
}