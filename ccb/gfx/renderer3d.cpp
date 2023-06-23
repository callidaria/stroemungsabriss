#include "renderer3d.h"

/*
	constructor()
	purpose: create renderer object to subsequently add 3D objects to and draw them
*/
Renderer3D::Renderer3D()
{ ibuffer.add_buffer(); }

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,vec3) -> uint16_t
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
	ml.push_back(Mesh(m,t,sm,nm,em,p,s,r,mofs));

	// check shadow cast request & output mesh id
	if (cast_shadow) scast_mesh_ids.push_back(mesh_id);
	return mesh_id;
}

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,vec3,uint16_t)
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
	iml.push_back(Mesh(m,t,sm,nm,em,p,s,r,imofs));

	// create related index upload pattern
	std::vector<float> cmesh_index;
	for (uint16_t i=0;i<dcap;i++) {
		cmesh_index.push_back(-10000),cmesh_index.push_back(-10000),cmesh_index.push_back(-10000),
		cmesh_index.push_back(0),cmesh_index.push_back(0),cmesh_index.push_back(0),
		cmesh_index.push_back(1),cmesh_index.push_back(1),cmesh_index.push_back(1);
	} mesh_indices.push_back(cmesh_index);

	// check shadow cast request & output mesh id
	if (cast_shadow) scast_instance_ids.push_back(mesh_id);
	return mesh_id;
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
	pml.push_back(PhysicalMesh(m,t,nm,mm,em,p,s,r,pmofs));

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
	// combine all mesh vertices to master vertex list & upload
	float ptarget = (pseq/6.0f)/(ml.size()+iml.size()+pml.size());
	std::vector<float> v;
	for (uint16_t i=0;i<ml.size();i++) {
		v.insert(v.end(),ml[i].v.begin(),ml[i].v.end());
		progress += ptarget;
	} buffer.bind();
	buffer.upload_vertices(v);

	// compile shader & load textures
	s3d.compile3d("shader/gvertex.shader","shader/gfragment.shader");
	for(uint16_t i=0;i<ml.size();i++) {
		ml[i].texture();
		progress += ptarget;
	} s3d.upload_int("tex",0);
	s3d.upload_int("sm",1);
	s3d.upload_int("emit",2);
	s3d.upload_int("nmap",3);
	s3d.upload_camera(cam3d);

	// combine all added instance vertices to master instance vertex list & upload
	std::vector<float> iv;
	for (uint16_t i=0;i<iml.size();i++) {
		iv.insert(iv.end(),iml[i].v.begin(),iml[i].v.end());
		progress += ptarget;
	} ibuffer.bind();
	ibuffer.upload_vertices(iv);

	// compile instance shader
	is3d.compile3d("shader/givertex.shader","shader/gfragment.shader");
	ibuffer.bind_index();
	is3d.def_indexF(ibuffer.get_indices(),"offset",3,0,R3D_INDEX_REPEAT);
	is3d.def_indexF(ibuffer.get_indices(),"rotation_sin",3,3,R3D_INDEX_REPEAT);
	is3d.def_indexF(ibuffer.get_indices(),"rotation_cos",3,6,R3D_INDEX_REPEAT);

	// load textures
	for(uint16_t i=0;i<iml.size();i++) {
		iml[i].texture();
		progress += ptarget;
	} is3d.upload_int("tex",0);
	is3d.upload_int("sm",1);
	is3d.upload_int("emit",2);
	is3d.upload_int("nmap",3);
	is3d.upload_camera(cam3d);

	// combine physical mesh vertices to master vertex list & upload
	std::vector<float> pv;
	for (uint16_t i=0;i<pml.size();i++) {
		pv.insert(pv.end(),pml[i].verts.begin(),pml[i].verts.end());
		progress += ptarget;
	} pbuffer.bind();
	pbuffer.upload_vertices(pv);

	// compile physical mesh shader & load textures
	pbms.compile3d("shader/gvertex.shader","shader/gpfragment.shader");
	for (uint16_t i=0;i<pml.size();i++) {
		pml[i].texture();
		progress += ptarget;
	} pbms.upload_int("colour_map",0);
	pbms.upload_int("normal_map",1);
	pbms.upload_int("material_map",2);
	pbms.upload_int("emission_map",3);
	pbms.upload_camera(cam3d);

	// compile shadow shader
	Buffer::unbind();
	shs.compile("shader/fbv_shadow.shader","shader/fbf_shadow.shader");
	shs.def_attributeF("position",3,0,3);
}
// TODO: check validity of this loading approach. processing vertex lists ?twice

/*
	prepare() -> void
	purpose: prepare shader and buffer for rendering
*/
void Renderer3D::prepare()
{
	// gl settings
	glEnable(GL_DEPTH_TEST);

	// prepare shader & buffer
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
	// run normal preparations
	prepare();

	// update and upload camera & position for normal mapping
	s3d.upload_camera(cam3d);
	s3d.upload_vec3("view_pos",cam3d.pos);
}

/*
	prepare_inst() -> void
	purpose: prepare shader & buffer for instanced rendering
*/
void Renderer3D::prepare_inst()
{
	// gl settings
	glEnable(GL_DEPTH_TEST);

	// prepare shader & buffer
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
	// run normal preparations
	prepare_inst();

	// update camera
	is3d.upload_camera(cam3d);
	is3d.upload_vec3("view_pos",cam3d.pos);
}

/*
	prepare_pmesh() -> void
	purpose: prepare shader & buffer for physical based rendering
*/
void Renderer3D::prepare_pmesh()
{
	// gl settings
	glEnable(GL_DEPTH_TEST);

	// prepare shader & buffer
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
	// run normal preparations
	prepare_pmesh();

	// update camera
	pbms.upload_camera(cam3d);
	pbms.upload_vec3("camera_pos",cam3d.pos);
}

/*
	prepare_shadow() -> void
	purpose: prepare shadow map projection render
*/
void Renderer3D::prepare_shadow()
{
	// set front face culling to avoid peter panning
	glCullFace(GL_FRONT);

	// prepare shadow framebuffer
	glViewport(0,0,shadow_res,shadow_res);
	glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
	Frame::clear();
}

/*
	register_geometry(ShadowGeometry*) -> void
	geometry: geometry which is capable of casting shadows
	purpose: add shadow casting geometry to shadow projection routine
*/
void Renderer3D::register_geometry(ShadowGeometry* geometry)
{ shadow_geometry.push_back(geometry); }

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
		glDrawArraysInstanced(GL_TRIANGLES,iml[id].ofs,iml[id].size,iml[id].inst_count);
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
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,ml[i].tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,ml[i].specmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,ml[i].emitmap);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,ml[i].normap);
		s3d.upload_matrix("model",ml[i].model);
		glDrawArrays(GL_TRIANGLES,ml[i].ofs,ml[i].size);
	} glActiveTexture(GL_TEXTURE0);
}
// FIXME: please finally choose if TEXTURE0 should be set at the beginning or reset after drawing
// FIXME: try to remove loop from this

/*
	PARAMETER DEFINITIONS:
	i: memory index of object that is to be drawn
*/

/*
	render_inst(uint16_t i,uint16_t) -> void
	i: memory index of instanced object that is to be drawn
	purpose: render given amount of desired instance's duplicates
*/
void Renderer3D::render_inst(uint16_t i)
{
	ibuffer.upload_indices(mesh_indices[i]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,iml[i].tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,iml[i].specmap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,iml[i].emitmap);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,iml[i].normap);
	is3d.upload_matrix("model",iml[i].model);
	glDrawArraysInstanced(GL_TRIANGLES,iml[i].ofs,iml[i].size,iml[i].inst_count);
	glActiveTexture(GL_TEXTURE0);
}

/*
	render_pmsh(uint16_t) -> void
	purpose: render desired physical based mesh
*/
void Renderer3D::render_pmsh(uint16_t i)
{
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,pml[i].tex_colour);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,pml[i].tex_normal);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,pml[i].tex_material);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D,pml[i].tex_emission);
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
	uint16_t rid = mid*R3D_INDEX_REPEAT;
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
	uint16_t rid = mid*R3D_INDEX_REPEAT;

	// precalculate sine & cosine for rotation matrix in GPU
	mesh_indices[id][rid+3] = glm::sin(rot.x),mesh_indices[id][rid+6] = glm::cos(rot.x),
	mesh_indices[id][rid+4] = glm::sin(rot.y),mesh_indices[id][rid+7] = glm::cos(rot.y),
	mesh_indices[id][rid+5] = glm::sin(rot.z),mesh_indices[id][rid+8] = glm::cos(rot.z);
}