#include "cubemap.h"

/*
	TODO
*/
Cubemap::Cubemap(const char* path)
{
	// buffer setup
	init_buffer();
	glGenTextures(1,&irr_tex);

	// shader compile
	s.compile("./shader/vcubed_irradiance.shader","./shader/fcubed_irradiance.shader");
	s.def_attributeF("position",3,0,3);
	irrs.compile("./shader/virradiance_map.shader","./shader/firradiance_map.shader");
	irrs.def_attributeF("position",3,0,3);

	// load hdr irradiance map
	int32_t width,height;
	glBindTexture(GL_TEXTURE_2D,irr_tex);
	float* data = stbi_loadf(path,&width,&height,0,0);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,width,height,0,GL_RGB,GL_FLOAT,data);
	stbi_image_free(data);
	Toolbox::set_texture_parameter_clamp_to_edge();
	Toolbox::set_texture_parameter_linear_unfiltered();
}

/*
	TODO
*/
Cubemap::Cubemap(std::vector<const char*> tp) // !!description && maybe stack ?
{
	// buffer setup
	init_buffer();

	// shader compile
	s.compile("shader/vertex_skybox.shader","shader/fragment_skybox.shader");
	s.def_attributeF("position",3,0,3);

	// cubemap textures
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	int32_t width,height;
	for (int i=0;i<tp.size();i++) {
		unsigned char* image = stbi_load(tp.at(i),&width,&height,0,STBI_rgb); // ??alpha needed
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,width,height,0,GL_RGB,GL_UNSIGNED_BYTE,image);
		stbi_image_free(image);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
}

/*
	TODO
*/
void Cubemap::render_irradiance_to_cubemap()
{
	// prepare pre-render buffer
	uint32_t cmfbo,cmrbo;
	glGenFramebuffers(1,&cmfbo);
	glGenRenderbuffers(1,&cmrbo);
	glBindFramebuffer(GL_FRAMEBUFFER,cmfbo);
	glBindRenderbuffer(GL_RENDERBUFFER,cmrbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,1024,1024);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,cmrbo);

	// setup cubemap texture
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	for (uint8_t i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,1024,1024,0,
				GL_RGB,GL_FLOAT,nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// prepare irradiance map render & camera projection
	glm::mat4 proj = glm::perspective(glm::radians(90.0f),1.0f,.1f,10.0f);
	prepare_irradiance();
	irrs.upload_matrix("proj",proj);

	// setup camera's worldly attributes
	glm::mat4 cam_attrib[] = {
		glm::lookAt(glm::vec3(0),glm::vec3(1,0,0),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(-1,0,0),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,1,0),glm::vec3(0,0,1)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,-1,0),glm::vec3(0,0,-1)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,0,1),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,0,-1),glm::vec3(0,-1,0))
	};

	// draw cubed
	glViewport(0,0,1024,1024);
	glBindTexture(GL_TEXTURE_2D,irr_tex);
	for (uint8_t i=0;i<6;i++) {
		irrs.upload_matrix("view",cam_attrib[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				tex,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES,0,36);
	} glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	TODO
*/
void Cubemap::prepare()
{
	s.enable();
	buffer.bind();
}

/*
	TODO
*/
void Cubemap::prepare_irradiance()
{
	irrs.enable();
	buffer.bind();
}

/*
	TODO
*/
void Cubemap::prepare_wcam(Camera3D* c)
{
	prepare();
	s.upload_matrix("view",glm::mat4(glm::mat3(c->view3D)));
	s.upload_matrix("proj",c->proj3D); // !!render with inverted face culling
}

/*
	TODO
*/
void Cubemap::prepare_irradiance_wcam(Camera3D cam3D)
{
	prepare_irradiance();
	irrs.upload_camera(cam3D);
}

/*
	TODO
*/
void Cubemap::render()
{
	glDisable(GL_CULL_FACE);
	glDepthFunc(GL_LEQUAL);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glDrawArrays(GL_TRIANGLES,0,36);
	glEnable(GL_CULL_FACE);
}

/*
	TODO
*/
void Cubemap::render_irradiance()
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,irr_tex);
	glDrawArrays(GL_TRIANGLES,0,36);
}

/*
	TODO
*/
void Cubemap::init_buffer()
{
	// texture setup
	glGenTextures(1,&tex);

	// buffer data
	buffer.bind();
	float verts[] = {
		-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,
		1.0f,-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,
		-1.0f,-1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,-1.0f,1.0f,
		-1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f,-1.0f,
		-1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,-1.0f,1.0f,-1.0f,-1.0f,-1.0f,-1.0f,1.0f,1.0f,-1.0f,1.0f
	}; buffer.upload_vertices(verts,sizeof(verts));
}