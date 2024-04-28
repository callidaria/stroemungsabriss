#include "renderer3d.h"

/*
	constructor()
	purpose: create renderer object to subsequently add 3D objects and draw them
*/
Renderer3D::Renderer3D()
{
	ibuffer.add_buffer();
	abuffer.add_buffer();
}

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,vec3,bool) -> uint16_t
	m: path to .obj file to read mesh vertices from
	t: path to file to read texture from
	sm: path to specular map file
	nm: path to normal map file
	em: path to emission map file
	p: origin position of the mesh
	s: initial mesh scaling
	r: initial mesh rotation
	cast_shadow (default false): true if object should cast a shadow
	purpose: add mesh object to renderer
	returns: memory index to refer to the created mesh object by when drawing
*/
uint16_t Renderer3D::add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
		glm::vec3 p,float s,glm::vec3 r,bool cast_shadow)
{
	// load mesh
	uint16_t mesh_id = ml.size();
	ml.push_back(Mesh(m,t,sm,nm,em,p,s,r,v_mesh,mofs));

	// check shadow cast request & output mesh id
	if (cast_shadow) scast_mesh_ids.push_back(mesh_id);
	return mesh_id;
}

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,vec3,uint16_t,bool)
			-> uint16_t
	overloads previous add()
	dcap: maximum amount of duplicates created from added instance
	purpose: add instanced object to renderer
	returns: memory index to refer to the created instanced object by when drawing
*/
uint16_t Renderer3D::add(const char* m,const char* t,const char* sm,const char* nm,
		const char* em,glm::vec3 p,float s,glm::vec3 r,uint16_t dcap,bool cast_shadow)
{
	// load mesh
	uint16_t mesh_id = iml.size();
	iml.push_back(Mesh(m,t,sm,nm,em,p,s,r,v_instance,imofs));

	// create related index upload pattern
	std::vector<float> temp = std::vector<float>(dcap*INST_VERTEX_REPEAT);
	for (uint16_t i=0;i<dcap;i++) {
		uint32_t r = i*INST_VERTEX_REPEAT;

		// write offset
		temp[r+INST_POSITION_X] = -10000;
		temp[r+INST_POSITION_Y] = -10000;
		temp[r+INST_POSITION_Z] = -10000;

		// write sine rotation
		temp[r+INST_SINROT_X] = 0;
		temp[r+INST_SINROT_Y] = 0;
		temp[r+INST_SINROT_Z] = 0;

		// write cosine rotation
		temp[r+INST_COSROT_X] = 1;
		temp[r+INST_COSROT_Y] = 1;
		temp[r+INST_COSROT_Z] = 1;
	}
	mesh_indices.push_back(temp);

	// check shadow cast request & output mesh id
	if (cast_shadow) scast_instance_ids.push_back(mesh_id);
	return mesh_id;
}

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,bool)
			-> uint16_t !O(1)b
	.overloads previous add()
	purpose: add animated object to the renderer
	\param a: path to collada (.dae) animation file
	\param t: path to colour texture
	\param n: path to normal map
	\param m: path to physical based material map
	\param e: path to emission texture
	\param p: origin position translated by model matrix in shader
	\param s: object scaling translated by model matrix in shader
	\param cast_shadow: (default false) determines if object projects shadow
	\returns: memory index to refer to the created animated object by when drawing
*/
uint16_t Renderer3D::add(const char* a,const char* t,const char* n,const char* m,const char* e,
		glm::vec3 p,float s,bool cast_shadow)
{
	// load animated mesh
	uint16_t animation_id = mal.size();
	MeshAnimation proc = MeshAnimation(a,t,n,m,e,v_animation,e_animation,amofs);

	// transform model matrix & store animated mesh
	proc.model = glm::translate(glm::mat4(1),p)*glm::scale(glm::mat4(1),glm::vec3(s));
	mal.push_back(proc);

	// check shadow cast request & return mesh id
	if (cast_shadow) scast_animation_ids.push_back(animation_id);
	return animation_id;
}

/*
	add(const char*,const char*,const char*,const char*,vec3,float,vec3,bool) -> uint16_t
	purpose: create mesh, which can be used for physical based rendering
	\param mm: path to the information about surface materials
	\returns memory index to refer to the created physical based mesh by when drawing
*/
uint16_t Renderer3D::add_physical(const char* m,const char* t,const char* nm,const char* mm,
		const char* em,glm::vec3 p,float s,glm::vec3 r,bool cast_shadow)
{
	// load mesh
	uint16_t mesh_id = pml.size();
	pml.push_back(PhysicalMesh(m,t,nm,mm,em,p,s,r,v_pbm,pmofs));

	// check shadow cast request & output mesh id
	if (cast_shadow) scast_physical_ids.push_back(mesh_id);
	return mesh_id;
}

/*
	create_shadow(vec3,vec3,float,float,float,uint16_t) -> void
	pos: position of light, which is supposed to project the shadow map
	center: central look-at point for projected shadow map
	mwidth: width of orthogonal camera space
	mheight: height of orthogonal camera space
	fdiv: breakdown divisor of real light position towards a more managable value
	res: squared resolution of shadow map
	purpose: create shadow map to later project scene objects onto
*/
void Renderer3D::create_shadow(glm::vec3 pos,glm::vec3 center,float mwidth,float mheight,
		float fdiv,uint16_t res)
{
	// save shadow resolution
	shadow_res = res;

	// setup shadow map texture
	glGenTextures(1,&shadow_map);
	float border_colour[4] = { 1 };
	glBindTexture(GL_TEXTURE_2D,shadow_map);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,res,res,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	Toolbox::set_texture_parameter_nearest_unfiltered();
	Toolbox::set_texture_parameter_clamp_to_border();
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,border_colour);

	// setup depth sensitive framebuffer object
	glGenFramebuffers(1,&depth_fbo);
	glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,shadow_map,0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	// FIXME: framebuffer handling for shadow depth map

	// calculate shadow projection
	slight_pos = pos;
	float hwidth = mwidth/2,hheight = mheight/2;
	shadow_view = glm::lookAt(pos/fdiv+center,center,glm::vec3(0,1,0));
	shadow_proj = glm::ortho(-hwidth,hwidth,-hheight,hheight,.1f,100.0f);
	scam_projection = shadow_proj*shadow_view;
}
// FIXME: i know of the commutative properties of matrix multiplication, maybe generally precalc cam?

/*
	load(Camera3D) -> void
	cam3d: camera to relate mesh objects to
	purpose: combine texture and vertex loading, define gl settings & compile shader program
*/
void Renderer3D::load(Camera3D cam3d,float &progress,float pseq)
{
	// load meshes
	// mesh vertex upload & mapping
	buffer.bind();
	buffer.upload_vertices(v_mesh);
	s3d.compile3d("./shader/obj/mesh.vs","./shader/obj/mesh.fs");

	// mesh texturing & coordinate system
	for (auto im : ml) im.texture();
	s3d.upload_int("tex",0);
	s3d.upload_int("sm",1);
	s3d.upload_int("emit",2);
	s3d.upload_int("nmap",3);
	s3d.upload_camera(cam3d);

	// load instances
	// instance vertex upload & mapping
	ibuffer.bind();
	ibuffer.upload_vertices(v_instance);
	is3d.compile3d("./shader/obj/instance.vs","./shader/obj/mesh.fs");
	ibuffer.bind_index();
	is3d.def_indexF("offset",3,INST_POSITION_X,INST_VERTEX_REPEAT);
	is3d.def_indexF("rotation_sin",3,INST_SINROT_X,INST_VERTEX_REPEAT);
	is3d.def_indexF("rotation_cos",3,INST_COSROT_X,INST_VERTEX_REPEAT);

	// instance texturing & coordinate system
	for (auto im : iml) im.texture();
	is3d.upload_int("tex",0);
	is3d.upload_int("sm",1);
	is3d.upload_int("emit",2);
	is3d.upload_int("nmap",3);
	is3d.upload_camera(cam3d);

	// load animations
	// animation vertex upload & mapping
	abuffer.bind();
	abuffer.upload_vertices(v_animation),abuffer.upload_elements(e_animation);
	as3d.compile("./shader/obj/animation.vs","./shader/obj/physical.fs");
	as3d.def_attributeF("position",3,ANIM_POSITION_X,ANIM_VERTEX_REPEAT);
	as3d.def_attributeF("texCoords",2,ANIM_TCOORD_X,ANIM_VERTEX_REPEAT);
	as3d.def_attributeF("normals",3,ANIM_NORMALS_X,ANIM_VERTEX_REPEAT);
	as3d.def_attributeF("tangent",3,ANIM_TANGENT_X,ANIM_VERTEX_REPEAT);
	as3d.def_attributeF("boneIndex",4,ANIM_BONE0,ANIM_VERTEX_REPEAT);
	as3d.def_attributeF("boneWeight",4,ANIM_WEIGHT0,ANIM_VERTEX_REPEAT);

	// animation texturing & coordinate system
	for (auto am : mal) am.texture();
	as3d.upload_int("colour_map",0);
	as3d.upload_int("normal_map",1);
	as3d.upload_int("material_map",2);
	as3d.upload_int("emission_map",3);
	as3d.upload_camera(cam3d);

	// load physical based meshes
	// physical vertex upload & mapping
	pbuffer.bind();
	pbuffer.upload_vertices(v_pbm);
	pbms.compile3d("./shader/obj/mesh.vs","./shader/obj/physical.fs");

	// physical texturing & coordinate system
	for (auto im : pml) im.texture();
	pbms.upload_int("colour_map",0);
	pbms.upload_int("normal_map",1),
	pbms.upload_int("material_map",2);
	pbms.upload_int("emission_map",3);
	pbms.upload_camera(cam3d);

	// compile shadow shader
	Buffer::unbind();
	shs.compile("./shader/lighting/shadow.vs","./shader/lighting/shadow.fs");
	shs.def_attributeF("position",3,0,3);
}
// TODO: vertex batches have to be cleaned manually now due to list write at add() call

/*
	add_target(Frame*) -> uint8_t !O(1)
	purpose: add deferred shading target to render 3D scenes to
	\param frame: current window to get maximum resolution from
	\returns target id
*/
uint8_t Renderer3D::add_target()
{
	// gbuffer setup
	GBuffer gbuffer = GBuffer(
			Init::iConfig[FRAME_RESOLUTION_WIDTH],Init::iConfig[FRAME_RESOLUTION_HEIGHT]);
	// TODO: add standard constructor gbuffer using full screen range
	gbuffer.add_colour_component();
	gbuffer.add_colour_component(true);
	gbuffer.add_colour_component(true);
	gbuffer.add_colour_component(true);
	gbuffer.add_depth_component();
	gbuffer.finalize_buffer();

	// deferred shading buffer setup
	FrameBuffer cbuffer = FrameBuffer(
			"./shader/standard/framebuffer.vs","./shader/lighting/pbr.fs",false);
	cbuffer.s.upload_int("gbuffer_colour",0);
	cbuffer.s.upload_int("gbuffer_position",1);
	cbuffer.s.upload_int("gbuffer_normals",2);
	cbuffer.s.upload_int("gbuffer_materials",3);
	cbuffer.s.upload_int("irradiance_map",4);
	cbuffer.s.upload_int("specular_map",5);
	cbuffer.s.upload_int("specular_brdf",6);
	cbuffer.s.upload_int("shadow_map",7);
	cbuffer.s.upload_int("transparency_buffer",8);
	cbuffer.s.upload_int("transparency_depth",9);
	cbuffer.s.upload_int("world_depth",10);

	// transparency buffer
	FrameBuffer tbuffer = FrameBuffer(
			"./shader/standard/framebuffer.vs","./shader/standard/direct.fs",false,true);

	// store & return
	rtargets.push_back({ gbuffer,cbuffer,tbuffer });
	return rtargets.size()-1;
}
// TODO: specify different shaders for different deferred shaded targets
// TODO: specify different resolutions independent from full frame resolution

/*
	upload_target_static_lighting(uint8_t,Lighting*) -> void !O(1)
	purpose: upload lightmaps and static simulated lightsources
	\param id: id of target to upload static lighting to
	\param lighting: pointer to lighting containing scenes lightmaps and static lights
*/
void Renderer3D::upload_target_static_lighting(uint8_t id,Lighting* lighting)
{
	// simulated lights
	rtargets[id].dbuffer.s.enable();
	lighting->upload(&rtargets[id].dbuffer.s);

	// light mapping
	glActiveTexture(GL_TEXTURE4);
	lighting->upload_diffusion_map();
	glActiveTexture(GL_TEXTURE5);
	lighting->upload_specular_map();
	glActiveTexture(GL_TEXTURE6);
	lighting->upload_specular_brdf();
	glActiveTexture(GL_TEXTURE0);
}

/*
	start_target(uint8_t) -> void !O(1)
	purpose: start rendering to target, to store results in targets gbuffer
	\param id: id of target to render to
	\note GL_DEPTH_TEST should be enabled and GL_BLEND should be forbidden
	\note to stop rendering to target call FrameBuffer::close();
*/
void Renderer3D::start_target(uint8_t id)
{
	rtargets[id].gbuffer.bind();
	Frame::clear();
}

/*
	switch_target_transparency(uint8_t) -> void !O(1)
	purpose: switch to transparency buffer of specified target
	\param id: id of target to switch transparency mode of
	\note GL_BLEND will be automatically enabled
*/
void Renderer3D::switch_target_transparency(uint8_t id)
{
	glEnable(GL_BLEND);
	rtargets[id].tbuffer.bind();
	Frame::clear();
}

/*
	render_target(uint8_t,Camera3D,Lighting*) -> void !O(1)
	purpose: render deferred shading of requested target based on camera and dynamic lighting
	\param id: index of target to render
	\param cam3D: camera to draw scene in perspective to
	\param lighting: pointer to lighting containing the simulated dynamic lights
*/
void Renderer3D::render_target(uint8_t id,Camera3D& cam3D,Lighting* lighting)
{
	// upload buffers elements
	rtargets[id].dbuffer.prepare();
	glBindTexture(GL_TEXTURE_2D,rtargets[id].gbuffer.t_colour_components[GBufferComponentID::COLOUR]);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,rtargets[id].gbuffer.t_colour_components[GBufferComponentID::POSITION]);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,rtargets[id].gbuffer.t_colour_components[GBufferComponentID::NORMALS]);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,rtargets[id].gbuffer.t_colour_components[GBufferComponentID::MATERIALS]);
	glActiveTexture(GL_TEXTURE7);
	glBindTexture(GL_TEXTURE_2D,shadow_map);
	glActiveTexture(GL_TEXTURE0);

	// uniform uploads
	rtargets[id].dbuffer.s.upload_vec3("view_pos",cam3D.pos);
	rtargets[id].dbuffer.s.upload_vec3("light_position",slight_pos);
	rtargets[id].dbuffer.s.upload_matrix("shadow_matrix",scam_projection);
	lighting->upload(&rtargets[id].dbuffer.s);

	// process deferred shading
	glDrawArrays(GL_TRIANGLES,0,6);
}

/*
	prepare() -> void
	purpose: prepare shader and buffer for rendering
*/
void Renderer3D::prepare()
{
	s3d.enable();
	buffer.bind();
}

/*
	PARAMETER DEFINITIONS:
	cam3d: camera to relate mesh/instance visuals to
*/

/*
	prepare(Camera3D) -> void
	overloads previous prepare()
	purpose: additionally to the function of prepare, this method updates and uploads the camera
*/
void Renderer3D::prepare(Camera3D cam3d)
{
	prepare();
	s3d.upload_camera(cam3d);
}

/*
	prepare_inst() -> void
	purpose: prepare shader & buffer for instanced rendering
*/
void Renderer3D::prepare_inst()
{
	is3d.enable();
	ibuffer.bind();
}
// FIXME: detach gl settings from basic preparations

/*
	prepare_inst(Camera3D) -> void
	overloads previous prepare_inst()
	purpose: not only prepare instanced rendering, but also upload camera
*/
void Renderer3D::prepare_inst(Camera3D cam3d)
{
	prepare_inst();
	is3d.upload_camera(cam3d);
}

/*
	prepare_anim() -> void !O(1)
	purpose: prepare shader & buffer for animation rendering
*/
void Renderer3D::prepare_anim()
{
	as3d.enable();
	abuffer.bind();
}

/*
	prepare_anim(Camera3D) -> void !O(1)
	overloads previous prepare_anim
	purpose: not only prepare animation rendering, but also upload camera
*/
void Renderer3D::prepare_anim(Camera3D cam3d)
{
	prepare_anim();
	as3d.upload_camera(cam3d);
}

/*
	prepare_pmesh() -> void
	purpose: prepare shader & buffer for physical based rendering
*/
void Renderer3D::prepare_pmesh()
{
	pbms.enable();
	pbuffer.bind();
}

/*
	prepare_pmesh(Camera3D) -> void
	purpose: not only prepare physical based rendering, but also upload camera
	.overloads previous prepare_pmesh()
*/
void Renderer3D::prepare_pmesh(Camera3D cam3d)
{
	prepare_pmesh();
	pbms.upload_camera(cam3d);
}

/*
	prepare_shadow() -> void
	purpose: prepare shadow map projection render
*/
void Renderer3D::prepare_shadow()
{
	glCullFace(GL_FRONT);
	glViewport(0,0,shadow_res,shadow_res);
	glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
	Frame::clear();
}

/*
	close_shadow(uint16_t,uint16_t) -> void
	w_res: x-axis resolution reset
	h_res: y-axis resolution reset
	purpose: end shadow map projection render
*/
void Renderer3D::close_shadow(uint16_t w_res,uint16_t h_res)
{
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0,0,w_res,h_res);
	glCullFace(GL_BACK);
}

/*
	clear_memory() -> void
	purpose: removes all extern shadow geometry from memory
*/
void Renderer3D::clear_memory()
{
	for (auto geometry : shadow_geometry)
		delete geometry;
}

/*
	update_animations(float) -> void !O(n)
	purpose: update all animations that have been started and thus added to the update id list
	\param dt: time delta since last frame
*/
void Renderer3D::update_animations(double dt)
{
	for (auto id : update_animation_ids)
		mal[id].interpolate(dt);
}

/*
	update_shadows(float,float) -> void
	purpose: run all shadow projection stages without exceptions
	\param swidth: screen width as stored in frame for viewport reset
	\param sheight: screen height as stored in frame for viewport reset
*/
void Renderer3D::update_shadows(float swidth,float sheight)
{
	prepare_shadow();
	render_mesh_shadow();
	render_instance_shadow();
	render_animation_shadow();
	render_physical_shadow();
	render_geometry_shadow();
	close_shadow(swidth,sheight);
}

/*
	render_mesh_shadow() -> void
	purpose: project mesh shadow onto shadow map
	NOTE: call in-between of prepare_shadow() & close_shadow()
*/
void Renderer3D::render_mesh_shadow()
{
	// prepare mesh buffer & shader to render shadow map
	prepare();
	s3d.upload_matrix("view",shadow_view);
	s3d.upload_matrix("proj",shadow_proj);

	// project casting objects to shadow map
	for (auto id : scast_mesh_ids) {
		s3d.upload_matrix("model",ml[id].model);
		glDrawArrays(GL_TRIANGLES,ml[id].ofs,ml[id].size);
	}
}

/*
	render_instance_shadow() -> void
	purpose: project shadow of instanced meshes onto shadow map
	NOTE: call in-between of prepare_shadow() & close_shadow()
*/
void Renderer3D::render_instance_shadow()
{
	// prepare instance buffer & shader to render shadow map
	prepare_inst();
	is3d.upload_matrix("view",shadow_view);
	is3d.upload_matrix("proj",shadow_proj);

	// project casting instanced objects to shadow map
	for (auto id : scast_instance_ids) {
		ibuffer.upload_indices(mesh_indices[id]);
		is3d.upload_matrix("model",iml[id].model);
		glDrawArraysInstanced(GL_TRIANGLES,iml[id].ofs,iml[id].size,inst_counts[id]);
	}
}

/*
	render_animation_shadow() -> void !O(n)
	purpose: project shadow of animated meshes onto shadow map
	NOTE: call in-between of prepare_shadow() & close_shadow()
*/
void Renderer3D::render_animation_shadow()
{
	// prepare animation buffer & shader to render shadow map
	prepare_anim();
	as3d.upload_matrix("view",shadow_view);
	as3d.upload_matrix("proj",shadow_proj);

	// project casting animated objects to shadow map
	for (auto id : scast_animation_ids) {
		as3d.upload_matrix("model",mal[id].model);
		mal[id].upload_interpolation(&as3d);
		glDrawElements(GL_TRIANGLES,mal[id].size,GL_UNSIGNED_INT,
				(void*)(mal[id].ofs*sizeof(uint32_t)));
	}
}

/*
	render_physical_shadow() -> void
	purpose: project shadow of physical based meshes onto shadow map
	NOTE: call in-between of prepare_shadow() & close_shadow()
*/
void Renderer3D::render_physical_shadow()
{
	// prepare physical mesh buffer & shader to render shadow map
	prepare_pmesh();
	pbms.upload_matrix("view",shadow_view);
	pbms.upload_matrix("proj",shadow_proj);

	// project casting physical meshes to shadow map
	for (auto id : scast_physical_ids) {
		pbms.upload_matrix("model",pml[id].model);
		glDrawArrays(GL_TRIANGLES,pml[id].offset,pml[id].size);
	}
}

/*
	render_geometry_shadow() -> void
	purpose: project shadows, originating from additional geometry onto shadow map
	NOTE: call in-between of prepare_shadow() & close_shadow()
*/
void Renderer3D::render_geometry_shadow()
{
	for (auto geometry : shadow_geometry)
		geometry->render_shadow();
}

/*
	render_mesh(uint16_t,uint16_t) -> void
	b: starting index of mesh that is to be drawn
	e: index to stop rendering at (excluding this index)
	purpose: render meshes until ending index without e value as mesh index
*/
void Renderer3D::render_mesh(uint16_t b,uint16_t e)
{
	for (int i=b;i<e;i++) {
		glBindTexture(GL_TEXTURE_2D,ml[i].tex);
		glActiveTexture(GL_TEXTURE1),glBindTexture(GL_TEXTURE_2D,ml[i].specmap);
		glActiveTexture(GL_TEXTURE2),glBindTexture(GL_TEXTURE_2D,ml[i].emitmap);
		glActiveTexture(GL_TEXTURE3),glBindTexture(GL_TEXTURE_2D,ml[i].normap);
		s3d.upload_matrix("model",ml[i].model);
		glDrawArrays(GL_TRIANGLES,ml[i].ofs,ml[i].size);
		glActiveTexture(GL_TEXTURE0);
	}
}
// FIXME: please finally choose if TEXTURE0 should be set at the beginning or reset after drawing
// FIXME: try to remove loop from this

/*
	PARAMETER DEFINITIONS:
	i: memory index of object that is to be drawn
*/

/*
	render_inst(uint16_t i,uint16_t) -> void
	purpose: render instanced object by given memory index
	\param i: memory index of instanced object that is to be drawn
	\note to show x amount of instances, Renderer3D::inst_counts[id] = x has to be set
*/
void Renderer3D::render_inst(uint16_t i)
{
	ibuffer.upload_indices(mesh_indices[i]);
	glBindTexture(GL_TEXTURE_2D,iml[i].tex);
	glActiveTexture(GL_TEXTURE1),glBindTexture(GL_TEXTURE_2D,iml[i].specmap);
	glActiveTexture(GL_TEXTURE2),glBindTexture(GL_TEXTURE_2D,iml[i].emitmap);
	glActiveTexture(GL_TEXTURE3),glBindTexture(GL_TEXTURE_2D,iml[i].normap);
	is3d.upload_matrix("model",iml[i].model);
	glDrawArraysInstanced(GL_TRIANGLES,iml[i].ofs,iml[i].size,inst_counts[i]);
	glActiveTexture(GL_TEXTURE0);
}

/*
	render_anim(uint16_t) -> void !O(1)
	purpose: render animation object by given memory index
	\param i: memory index of animated object that is to be drawn
*/
void Renderer3D::render_anim(uint16_t i)
{
	glBindTexture(GL_TEXTURE_2D,mal[i].t_colour);
	glActiveTexture(GL_TEXTURE1),glBindTexture(GL_TEXTURE_2D,mal[i].t_normals);
	glActiveTexture(GL_TEXTURE2),glBindTexture(GL_TEXTURE_2D,mal[i].t_material);
	glActiveTexture(GL_TEXTURE3),glBindTexture(GL_TEXTURE_2D,mal[i].t_emission);
	as3d.upload_matrix("model",mal[i].model);
	mal[i].upload_interpolation(&as3d);
	glDrawElements(GL_TRIANGLES,mal[i].size,GL_UNSIGNED_INT,(void*)(mal[i].ofs*sizeof(uint32_t)));
	glActiveTexture(GL_TEXTURE0);
}

/*
	render_pmsh(uint16_t) -> void
	purpose: render desired physical based mesh
	\param i: memory index of physically shaded object that is to be drawn
	FIXME: naming for physical prepare & render is unintuitive
*/
void Renderer3D::render_pmsh(uint16_t i)
{
	glBindTexture(GL_TEXTURE_2D,pml[i].tex_colour);
	glActiveTexture(GL_TEXTURE1),glBindTexture(GL_TEXTURE_2D,pml[i].tex_normal);
	glActiveTexture(GL_TEXTURE2),glBindTexture(GL_TEXTURE_2D,pml[i].tex_material);
	glActiveTexture(GL_TEXTURE3),glBindTexture(GL_TEXTURE_2D,pml[i].tex_emission);
	pbms.upload_matrix("model",pml[i].model);
	glDrawArrays(GL_TRIANGLES,pml[i].offset,pml[i].size);
	glActiveTexture(GL_TEXTURE0);
}

/*
	PARAMETER DEFINITIONS:
	id: index of instanced object the duplicate to transform is from
	mid: index of duplicate to transform
*/

/*
	inst_position(uint8_t,uint8_t,vec3) -> void
	pos: position to translate the duplicate to
	purpose: move desired duplicate to given position
*/
void Renderer3D::inst_position(uint8_t id,uint8_t mid,glm::vec3 pos)
{
	uint16_t rid = mid*INST_VERTEX_REPEAT;
	mesh_indices[id][rid] = pos.x,mesh_indices[id][rid+1] = pos.y,mesh_indices[id][rid+2] = pos.z;
}

/*
	inst_rotation(uint8_t,uint8_t,vec3) -> void
	rot: new rotation of given duplicate (radians)
	purpose: rotate desired duplicate by given rotation
*/
void Renderer3D::inst_rotation(uint8_t id,uint8_t mid,glm::vec3 rot)
{
	// rasterize id jumps
	uint16_t rid = mid*INST_VERTEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	mesh_indices[id][rid+3] = glm::sin(rot.x),mesh_indices[id][rid+6] = glm::cos(rot.x),
	mesh_indices[id][rid+4] = glm::sin(rot.y),mesh_indices[id][rid+7] = glm::cos(rot.y),
	mesh_indices[id][rid+5] = glm::sin(rot.z),mesh_indices[id][rid+8] = glm::cos(rot.z);
}
