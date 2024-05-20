#include "casino_spike.h"

/*
	constructor(StageSetup*)
	ccbf: all of cascabel's most prized features
	purpose: create dilapidated casino scene
*/
CasinoSpike::CasinoSpike(float &progress,float pseq)
{
	// camera setup
	Core::gCamera3D = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f);

	// object loading
	float sseq = pseq/3.0f;
	index_p3D = Core::gR3D.add_physical("./res/casino/test_floor.obj",
			"./res/casino/paquet_colour.png","./res/casino/paquet_normal.png",
			"./res/casino/paquet_material.png","./res/none.png",glm::vec3(),1,glm::vec3(),false);
	Core::gR3D.add_physical("./res/casino/test_entrance.obj","./res/casino/entrance_colour.png",
			"./res/casino/entrance_normal.png","./res/casino/entrance_material.png",
			"./res/none.png",glm::vec3(),1,glm::vec3(),false);
	Core::gR3D.add_physical("./res/casino/test_table.obj","./res/casino/table_colour.png",
			"./res/casino/table_normal.png","./res/casino/table_material.png","./res/none.png",
			glm::vec3(-2,0,-1),1,glm::vec3(),true);
	Core::gR3D.add_physical("./res/casino/rolling.obj","./res/casino/tobj1_colour.png",
			"./res/casino/tobj1_normal.png","./res/casino/tobj1_material.png","./res/none.png",
			glm::vec3(2,1,1),1,glm::vec3(),true);
	Core::gR3D.add_physical("./res/casino/rolling.obj","./res/casino/pbr/colour.png",
			"./res/casino/pbr/normals.png","./res/casino/pbr/material.png","./res/none.png",
			glm::vec3(2,.5f,-3),.5f,glm::vec3(),true);
	Core::gR3D.add_physical("./res/casino/rolling.obj","./res/casino/fabric_colour.png",
			"./res/casino/fabric_normals.png","./res/casino/fabric_material.png","./res/none.png",
			glm::vec3(2.5f,.5f,-1.5f),.5f,glm::vec3(),true);
	progress += sseq;

	// animation test
	uint16_t a0 = Core::gR3D.add("./res/testkaks.dae","./res/kaks.png","./res/dnormal.png",
			"./res/default_mat.png","./res/none.png",glm::vec3(4,0,4),.4f,true);
	uint16_t a1 = Core::gR3D.add("./res/test_anim.dae","./res/anim_tex.png","./res/dnormal.png",
			"./res/default_mat.png","./res/none.png",glm::vec3(7,0,-4),.7f,true);
	Core::gR3D.mal[a0].set_animation(0);
	Core::gR3D.mal[a1].set_animation(0);
	Core::gR3D.start_animation(a0);
	Core::gR3D.start_animation(a1);

	// simulated lighting
	glm::vec3 light_pos[] = {
		glm::vec3(0,2,0),glm::vec3(3,2,-3),glm::vec3(7,.25f,-7),glm::vec3(7,.25f,-3),
		glm::vec3(7,.25f,1),glm::vec3(7,.25f,5),glm::vec3(-4.5f,1,4),glm::vec3(-2.5f,4,-2),
		glm::vec3(-2.3f,5,-1.3f)
	}; index_r3D = Core::gR3D.ml.size();
	for (uint8_t i=0;i<9;i++) {
		Core::gR3D.add("./res/casino/rolling.obj","./res/all.png","./res/none.png",
			"./res/dnormal.png","./res/all.png",light_pos[i],.25f,glm::vec3(),true);
		Core::gLighting.add_pointlight({ light_pos[i],glm::vec3(1),1,1,.1f,1 });
	} oheights[0] = 5.5f,oheights[1] = 7,oheights[2] = 6.5f,oheights[3] = 6;
	progress += sseq;
	Core::gR3D.upload_target_static_lighting(0,&Core::gLighting);
	// TODO: structurize again and again until this shit makes any sense!
	//	maybe approaching a worldly stage lighting is the wrong idea & privatize lights for scenes

	// lighting maps
	Core::gR3D.create_shadow(glm::vec3(150,125,-100),glm::vec3(0),25,25,10,4096);
	irradiance_map.dynamic_precalculation_load_switch("tcasino",1024,32,128,7,2048);
	Core::gLighting.load_irradiance_maps(irradiance_map);
	progress += sseq;
}

/*
	render() -> void (virtual)
	purpose: render casino scene
*/
void CasinoSpike::render()
{
	// camera front handling
	pitch += Core::gFrame.kb.ka[SDL_SCANCODE_I],pitch -= Core::gFrame.kb.ka[SDL_SCANCODE_K];
	yaw += Core::gFrame.kb.ka[SDL_SCANCODE_L],yaw -= Core::gFrame.kb.ka[SDL_SCANCODE_J];
	cp_dir.x = glm::cos(glm::radians(pitch))*glm::cos(glm::radians(yaw));
	cp_dir.y = glm::sin(glm::radians(pitch));
	cp_dir.z = glm::cos(glm::radians(pitch))*glm::sin(glm::radians(yaw));

	// camera position handling
	cp_pos += cp_dir*glm::vec3(.05f*Core::gFrame.kb.ka[SDL_SCANCODE_W]);
	cp_pos -= cp_dir*glm::vec3(.05f*Core::gFrame.kb.ka[SDL_SCANCODE_S]);
	cp_pos += glm::normalize(glm::cross(cp_dir,glm::vec3(0,1,0)))
			* glm::vec3(.05f*Core::gFrame.kb.ka[SDL_SCANCODE_D]);
	cp_pos -= glm::normalize(glm::cross(cp_dir,glm::vec3(0,1,0)))
			* glm::vec3(.05f*Core::gFrame.kb.ka[SDL_SCANCODE_A]);
	cp_pos.y += Core::gFrame.kb.ka[SDL_SCANCODE_R]*.05f;
	cp_pos.y -= Core::gFrame.kb.ka[SDL_SCANCODE_F]*.05f;

	// camera update
	Core::gCamera3D.pos = cp_pos, Core::gCamera3D.front = cp_dir;
	Core::gCamera3D.update();

	// update physics
	for (uint8_t i=0;i<4;i++) {
		Core::gR3D.ml[index_r3D+2+i].model
				= glm::translate(glm::mat4(1),glm::vec3(0,oheights[i],0));
		Core::gLighting.pointlights[2+i].position.y = oheights[i]+.25f;
		ospeed[i] *= 1-2*(oheights[i]<0);
		oheights[i] += ospeed[i];
		ospeed[i] -= .0009f;
	}

	// render flooring
	Core::gR3D.prepare(Core::gCamera3D);
	Core::gR3D.render_mesh(index_r3D,index_r3D+9);

	// render physical based objects
	Core::gR3D.prepare_pmesh(Core::gCamera3D);
	for (uint8_t i=0;i<6;i++) {
		Core::gR3D.pbms.upload_float("tex_repeat",texture_repeat[i]);
		Core::gR3D.render_pmsh(index_p3D+i);
	}

	// animation render
	Core::gR3D.prepare_anim(Core::gCamera3D);
	Core::gR3D.render_anim(0);
	Core::gR3D.render_anim(1);

	// render irradiance
	irradiance_map.prepare(Core::gCamera3D);
	irradiance_map.render_irradiance();
}
