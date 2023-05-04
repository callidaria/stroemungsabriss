#include "cubemap.h"

/*
	TODO
*/
Cubemap::Cubemap(const char* path)
{
	// buffer setup
	init_buffer();
	glGenTextures(1,&irr_tex);
	glGenTextures(1,&imap);
	glGenTextures(1,&smap);

	// shader compile
	approx_irr.compile("./shader/vapprox_irradiance.shader","./shader/fapprox_irradiance.shader");
	approx_irr.def_attributeF("position",3,0,3);
	approx_ref.compile("./shader/vcubed_irradiance.shader","./shader/fspec_montecarlo.shader");
	irrs.compile("./shader/virradiance_map.shader","./shader/firradiance_map.shader");
	irrs.def_attributeF("position",3,0,3);
	s.compile("./shader/vcubed_irradiance.shader","./shader/fcubed_irradiance.shader");
	s.def_attributeF("position",3,0,3);
	s.upload_int("irradiance_map",0);

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
void Cubemap::render_irradiance_to_cubemap(int32_t resolution)
{
	// prepare pre-render framebuffer
	uint32_t cmfbo,cmrbo;
	glGenFramebuffers(1,&cmfbo);
	glGenRenderbuffers(1,&cmrbo);
	glBindFramebuffer(GL_FRAMEBUFFER,cmfbo);
	glBindRenderbuffer(GL_RENDERBUFFER,cmrbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,resolution,resolution);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,cmrbo);
	source_res = resolution;

	// setup cubemap texture
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	for (uint8_t i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,resolution,resolution,0,GL_RGB,
				GL_FLOAT,nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// prepare cubemap render & camera projection
	glm::mat4 proj = glm::perspective(glm::radians(90.0f),1.0f,1.0f,3.0f);
	irrs.enable();
	buffer.bind();
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

	// draw equirectangular to cubed
	glViewport(0,0,resolution,resolution);
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
void Cubemap::approximate_irradiance(int32_t ri_res,uint32_t re_res,uint8_t lod_count,
		uint16_t sample_count)
{
	// prepare diffusion approximation
	uint32_t cmfbo,cmrbo;
	glGenFramebuffers(1,&cmfbo);
	glGenRenderbuffers(1,&cmrbo);
	glBindFramebuffer(GL_FRAMEBUFFER,cmfbo);
	glBindRenderbuffer(GL_RENDERBUFFER,cmrbo);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,ri_res,ri_res);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,cmrbo);

	// setup irradiance map texture
	glBindTexture(GL_TEXTURE_CUBE_MAP,imap);
	for (uint8_t i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,ri_res,ri_res,0,GL_RGB,
				GL_FLOAT,nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

	// prepare irradiance approximation map render & camera projection
	glm::mat4 proj = glm::perspective(glm::radians(90.0f),1.0f,1.0f,3.0f);
	approx_irr.enable();
	buffer.bind();
	approx_irr.upload_matrix("proj",proj);

	// setup camera's worldly attributes
	glm::mat4 cam_attrib[] = {
		glm::lookAt(glm::vec3(0),glm::vec3(1,0,0),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(-1,0,0),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,1,0),glm::vec3(0,0,1)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,-1,0),glm::vec3(0,0,-1)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,0,1),glm::vec3(0,-1,0)),
		glm::lookAt(glm::vec3(0),glm::vec3(0,0,-1),glm::vec3(0,-1,0))
	};

	// draw precise to convoluted
	glViewport(0,0,ri_res,ri_res);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);
	for (uint8_t i=0;i<6;i++) {
		approx_irr.upload_matrix("view",cam_attrib[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				imap,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES,0,36);
	} glBindFramebuffer(GL_FRAMEBUFFER,0);

	// setup specular approximation
	glBindTexture(GL_TEXTURE_CUBE_MAP,smap);
	for (uint8_t i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,re_res,re_res,0,GL_RGB,
				GL_FLOAT,nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_R,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	// filter multidetailed
	approx_ref.enable();
	approx_ref.upload_int("source_resolution",source_res);
	approx_ref.upload_int("sample_count",sample_count);
	approx_ref.upload_matrix("proj",proj);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glBindFramebuffer(GL_FRAMEBUFFER,cmfbo);
	for (uint8_t j=0;j<lod_count;j++) {

		// scale towards current level of detail
		uint16_t lod_resolution = re_res*glm::pow(.5f,j);
		glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT16,lod_resolution,lod_resolution);
		glViewport(0,0,lod_resolution,lod_resolution);

		// draw filter for current level of detail & roughness
		float roughness = (float)j/(lod_count-1);
		approx_ref.upload_float("roughness",roughness);
		for (uint8_t i=0;i<6;i++) {
			approx_ref.upload_matrix("view",cam_attrib[i]);
			glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,
					GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,smap,j);
			glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
			glDrawArrays(GL_TRIANGLES,0,36);
		}
	} glBindFramebuffer(GL_FRAMEBUFFER,0);
}
// FIXME: code repetitions
// FIXME: structurize depth function setting

/*
	TODO
*/
void Cubemap::prepare()
{
	glActiveTexture(GL_TEXTURE0);
	s.enable();
	buffer.bind();
}

/*
	TODO
*/
void Cubemap::prepare(Camera3D cam3D)
{
	prepare();
	s.upload_matrix("view",glm::mat4(glm::mat3(cam3D.view3D)));
	s.upload_matrix("proj",cam3D.proj3D); // !!render with inverted face culling
}

/*
	TODO
*/
void Cubemap::render_irradiance()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	glDrawArrays(GL_TRIANGLES,0,36);
}
void Cubemap::render_diffusion_approximated()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,imap);
	glDrawArrays(GL_TRIANGLES,0,36);
}
void Cubemap::render_specular_approximated()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,smap);
	glDrawArrays(GL_TRIANGLES,0,36);
}

/*
	TODO
*/
uint32_t Cubemap::get_irradiance_map()
{ return tex; }
uint32_t Cubemap::get_diffusion_approximation()
{ return imap; }
uint32_t Cubemap::get_specular_approximation()
{ return smap; }

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