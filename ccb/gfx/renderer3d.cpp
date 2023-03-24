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
	purpose: add mesh object to renderer
	returns: memory index to refer to the created mesh object by when drawing
*/
uint16_t Renderer3D::add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
		glm::vec3 p,float s,glm::vec3 r)
{
	ml.push_back(Mesh(m,t,sm,nm,em,p,s,r,&mofs));
	return ml.size()-1;
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
		const char* em,glm::vec3 p,float s,glm::vec3 r,uint16_t dcap)
{
	iml.push_back(Mesh(m,t,sm,nm,em,p,s,r,&imofs));
	std::vector<float> cmesh_index;
	for (uint16_t i=0;i<dcap;i++) {
		cmesh_index.push_back(-10000),cmesh_index.push_back(-10000),cmesh_index.push_back(-10000),
		cmesh_index.push_back(0),cmesh_index.push_back(0),cmesh_index.push_back(0),
		cmesh_index.push_back(1),cmesh_index.push_back(1),cmesh_index.push_back(1);
	} mesh_indices.push_back(cmesh_index);
	return iml.size()-1;
}

/*
	load(Camera3D*) -> void
	cam3d: camera to relate mesh objects to
	purpose: combine texture and vertex loading, define gl settings & compile shader program
*/
void Renderer3D::load(Camera3D cam3d)
{
	// combine all mesh vertices to master vertex list & upload
	std::vector<float> v;
	for (uint16_t i=0;i<ml.size();i++) v.insert(v.end(),ml[i].v.begin(),ml[i].v.end());
	buffer.bind();
	buffer.upload_vertices(v);

	// compile shader & load textures
	s3d.compile3d("shader/vertex3d.shader","shader/fragment3d.shader");
	for(uint16_t i=0;i<ml.size();i++) ml[i].texture();
	s3d.upload_int("tex",0);
	s3d.upload_int("sm",1);
	s3d.upload_int("emit",2);
	s3d.upload_int("shadow_map",3);
	s3d.upload_int("nmap",4);
	s3d.upload_camera(cam3d);

	// combine all added instance vertices to master instance vertex list & upload
	std::vector<float> iv;
	for (uint16_t i=0;i<iml.size();i++) iv.insert(iv.end(),iml[i].v.begin(),iml[i].v.end());
	ibuffer.bind();
	ibuffer.upload_vertices(iv);

	// compile instance shader
	is3d.compile3d("shader/vertexi3d.shader","shader/fragmenti3d.shader");
	ibuffer.bind_index();
	is3d.def_indexF(ibuffer.get_indices(),"offset",3,0,R3D_INDEX_REPEAT);
	is3d.def_indexF(ibuffer.get_indices(),"rotation_sin",3,3,R3D_INDEX_REPEAT);
	is3d.def_indexF(ibuffer.get_indices(),"rotation_cos",3,6,R3D_INDEX_REPEAT);

	// load textures
	for(uint16_t i=0;i<iml.size();i++) iml[i].texture();
	is3d.upload_int("tex",0);
	is3d.upload_int("sm",1);
	is3d.upload_int("emit",2);
	is3d.upload_int("shadow_map",3);
	is3d.upload_int("nmap",4);
	is3d.upload_camera(cam3d);

	// compile shadow shader
	shs.compile("shader/fbv_shadow.shader","shader/fbf_shadow.shader");
	shs.def_attributeF("position",3,0,3);
}

/*
	prepare() -> void
	purpose: prepare shader and buffer for rendering
*/
void Renderer3D::prepare()
{
	// gl settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// prepare shader & buffer
	s3d.enable();
	buffer.bind();
}

/*
	PARAMETER DEFINITIONS:
	cam3d: camera to relate mesh/instance visuals to
*/

/*
	prepare(Camera3D*) -> void
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
	glEnable(GL_CULL_FACE);

	// prepare shader & buffer
	is3d.enable();
	ibuffer.bind();
}

/*
	prepare_inst(Camera3D*) -> void
	overloads previous prepare
	purpose: not only prepare instanced rendering, but also upload camera
*/
void Renderer3D::prepare_inst(Camera3D cam3d)
{
	//run normal preparations
	prepare_inst();

	// update camera
	is3d.upload_camera(cam3d);
	is3d.upload_vec3("view_pos",cam3d.pos);
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
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D,ml[i].normap);
		glDrawArrays(GL_TRIANGLES,ml[i].ofs,ml[i].size);
	} glActiveTexture(GL_TEXTURE0);
}

/*
	render_inst(uint16_t i,uint16_t) -> void
	i: memory index of instanced object that is to be drawn
	c: amount of duplicates, in order of instance buffer upload, to be drawn
	purpose: render given amount of desired instance's duplicates
*/
void Renderer3D::render_inst(uint16_t i,uint16_t c)
{
	ibuffer.upload_indices(mesh_indices[i]);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,iml[i].tex);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,iml[i].specmap);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D,iml[i].emitmap);
	glActiveTexture(GL_TEXTURE4);
	glBindTexture(GL_TEXTURE_2D,iml[i].normap);
	glDrawArraysInstanced(GL_TRIANGLES,iml[i].ofs,iml[i].size,c);
	glActiveTexture(GL_TEXTURE0);
}

/*
	upload_shadow(mat4) -> void
	DEPRECATED: this will be removed after completing #135
*/
void Renderer3D::upload_shadow(glm::mat4 shadow_matrix)
{ s3d.upload_matrix("light_trans",shadow_matrix); }

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