#include "cubemap.h"

/*
	constructor(const char*)
	purpose: construct a cubemap from .hdr file, that can be used as irradiance map
	\param path: path to irradiance map in .hdr format
*/
Cubemap::Cubemap(const char* path)
{
	// buffer setup
	init_buffer();
	glGenTextures(1,&irr_tex);
	glGenTextures(1,&imap);
	glGenTextures(1,&smap);
	glGenTextures(1,&pcsmap);

	// shader compile
	approx_irr.compile("./shader/vapprox_irradiance.shader","./shader/fapprox_irradiance.shader");
	approx_irr.def_attributeF("position",3,0,3);
	approx_ref.compile("./shader/vcubed_irradiance.shader","./shader/fspec_montecarlo.shader");
	approx_ref.def_attributeF("position",3,0,3);
	irrs.compile("./shader/virradiance_map.shader","./shader/firradiance_map.shader");
	irrs.def_attributeF("position",3,0,3);
	s.compile("./shader/vcubed_irradiance.shader","./shader/fcubed_irradiance.shader");
	s.def_attributeF("position",3,0,3);
	s.upload_int("irradiance_map",0);

	// setup specular brdf integral precalculation
	glBindTexture(GL_TEXTURE_2D,pcsmap);
	Toolbox::load_texture_unfiltered(pcsmap,"./res/brdf_pcintegral.png",false);

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
	constructor(std::vector<const char*>)
	purpose: construct a simple skybox without irradiance capabilities
	\param tp: vector holding all six directories to the six skybox textures for different angles
	TODO: allow skyboxes to also be used for irradiance mapping
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
	} Toolbox::set_cubemap_texture_parameters();
}
// TODO: untested after recent changes. it remains a question if this is the way things work

/*
	render_irradiance_to_cubemap(int32_t) -> void
	purpose: project .hdr irradiance onto a cube, which can be used as skybox
	\param resolution: resolution of each cubemap view
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
	Toolbox::set_cubemap_texture_parameters();

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

	// setup image write buffer
	uint32_t bffsize = 3*resolution*resolution;
	unsigned char buffer_data[bffsize];

	// draw equirectangular to cubed
	glViewport(0,0,resolution,resolution);
	glBindTexture(GL_TEXTURE_2D,irr_tex);
	for (uint8_t i=0;i<6;i++) {
		irrs.upload_matrix("view",cam_attrib[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				tex,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES,0,36);

		// store prerendered maps in file
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(0,0,resolution,resolution,GL_RGB,GL_UNSIGNED_BYTE,buffer_data);
		stbi_write_png(("./dat/precalc/irradiance"+std::to_string(i)+".png").c_str(),
				resolution,resolution,3,buffer_data,3*resolution);
	} glBindFramebuffer(GL_FRAMEBUFFER,0);
}

/*
	approximate_irradiance(int32_t,uint32_t,uint8_t,uint16_t) -> void
	purpose: precalculate diffusion convolution and specular reflection integral irradiance maps
	\param ri_res: diffusion map resolution, per cubemap view
	\param re_res: specular map resolution, per cubemap view
	\param lod_count: amount of level of detail stages for irradiance-map-mipmap
	\param sample_count: amount of samples to use for specular irradiance map precalculation
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
	Toolbox::set_cubemap_texture_parameters();

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

	// setup image write buffers
	uint32_t ibffsize = 3*ri_res*ri_res,ebffsize = 4*re_res*re_res;
	float ibuffer_data[ibffsize],ebuffer_data[ebffsize];

	// draw precise to convoluted
	glViewport(0,0,ri_res,ri_res);
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	for (uint8_t i=0;i<6;i++) {
		approx_irr.upload_matrix("view",cam_attrib[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
				imap,0);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		glDrawArrays(GL_TRIANGLES,0,36);

		// store prerendered convolution in file
		glPixelStorei(GL_PACK_ALIGNMENT,1);
		glReadPixels(0,0,ri_res,ri_res,GL_RGB,GL_UNSIGNED_BYTE,ibuffer_data);
		stbi_write_png(("./dat/precalc/convolution"+std::to_string(i)+".png").c_str(),ri_res,ri_res,
				3,ibuffer_data,3*ri_res);
	} glBindFramebuffer(GL_FRAMEBUFFER,0);

	// setup specular approximation
	glBindTexture(GL_TEXTURE_CUBE_MAP,smap);
	for (uint8_t i=0;i<6;i++)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGBA16F,re_res,re_res,0,GL_RGBA,
				GL_FLOAT,nullptr);
	Toolbox::set_cubemap_texture_parameters_mipmap();

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

			// store prerendered specular multidetailed
			glPixelStorei(GL_PACK_ALIGNMENT,1);
			glReadPixels(0,0,lod_resolution,lod_resolution,GL_RGBA,GL_FLOAT,ebuffer_data);
			stbi_write_hdr(("./dat/precalc/specular"+std::to_string(i)+"_lod"+std::to_string(j)
					+".hdr").c_str(),lod_resolution,lod_resolution,4,ebuffer_data);
		}
	} glBindFramebuffer(GL_FRAMEBUFFER,0);
}
// FIXME: code repetitions

/*
	TODO
*/
void Cubemap::load_irradiance_cube()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP,tex);
	int32_t width,height;
	for (uint8_t i=0;i<6;i++) {
		unsigned char* image = stbi_load(("./dat/precalc/irradiance"+std::to_string(i)+".png")
				.c_str(),&width,&height,0,STBI_rgb);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB,
				GL_UNSIGNED_BYTE,image);
		stbi_image_free(image);
	} Toolbox::set_cubemap_texture_parameters();
}

/*
	TODO
*/
void Cubemap::load_irradiance_maps(uint8_t lod_count)
{
	// load convoluted diffusion map
	glBindTexture(GL_TEXTURE_CUBE_MAP,imap);
	int32_t width,height;
	for (uint8_t i=0;i<6;i++) {
		unsigned char* image = stbi_load(("./dat/precalc/convolution"+std::to_string(i)+".png")
				.c_str(),&width,&height,0,STBI_rgb);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB,width,height,0,GL_RGB,
				GL_UNSIGNED_BYTE,image);
		stbi_image_free(image);
	} Toolbox::set_cubemap_texture_parameters();

	// load multidetailed specular irradiance map
	glBindTexture(GL_TEXTURE_CUBE_MAP,smap);
	for (uint8_t i=0;i<6;i++) {
		for (uint8_t lod=0;lod<lod_count;lod++) {
			float* image = stbi_loadf(("./dat/precalc/specular"+std::to_string(i)+"_lod"
					+std::to_string(lod)+".hdr").c_str(),&width,&height,0,STBI_rgb);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,lod,GL_RGB16F,width,height,0,GL_RGB,
					GL_FLOAT,image);
			stbi_image_free(image);
		}
	} glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_BASE_LEVEL,lod_count-1);
	Toolbox::set_cubemap_texture_parameters_mipmap();
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_BASE_LEVEL,0);
}

/*
	prepare() -> void
	purpose: prepare cubemap visual representation as skybox
*/
void Cubemap::prepare()
{
	glActiveTexture(GL_TEXTURE0);
	s.enable();
	buffer.bind();
}

/*
	prepare(Camera3D) -> void
	purpose: prepare cubemap visual representation as skybox and update camera position
	\param cam3D: 3D camera, that should record the rendered skybox in relation to itself
*/
void Cubemap::prepare(Camera3D cam3D)
{
	prepare();
	s.upload_matrix("view",glm::mat4(glm::mat3(cam3D.view3D)));
	s.upload_matrix("proj",cam3D.proj3D); // !!render with inverted face culling
}

/*
	render_<skybox_version>() -> void
	purpose: render skybox representation of requested cubemap version
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
	get_<irradiance_map_component>() -> uint32_t
	\returns requested component of irradiance map
*/
uint32_t Cubemap::get_irradiance_map()
{ return tex; }
uint32_t Cubemap::get_diffusion_approximation()
{ return imap; }
uint32_t Cubemap::get_specular_approximation()
{ return smap; }
uint32_t Cubemap::get_specular_brdf()
{ return pcsmap; }

/*
	init_buffer() -> void (private)
	purpose: standard initialization of cubemap buffer & generation of base texture
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