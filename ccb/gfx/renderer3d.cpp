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
	TODO
*/
uint16_t Renderer3D::add_inst(const char* m,const char* t,const char* sm,const char* nm,
		const char* em,glm::vec3 p,float s,glm::vec3 r)
{
	iml.push_back(Mesh(m,t,sm,nm,em,p,s,r,&imofs));
	std::vector<float> cmesh_index;
	for (uint8_t i=0;i<128;i++) {
		cmesh_index.push_back(0);
		cmesh_index.push_back(i*.15f);
		cmesh_index.push_back(i*.15f);
	} mesh_indices.push_back(cmesh_index);
	return iml.size()-1;
}

/*
	load_vertex() -> void
	purpose: upload all added mesh vertices to buffer
*/
void Renderer3D::load_vertex()
{
	// combine all mesh vertices to master vertex list & upload
	std::vector<float> v;
	for (uint16_t i=0;i<ml.size();i++) v.insert(v.end(),ml[i].v.begin(),ml[i].v.end());
	buffer.bind();
	buffer.upload_vertices(v);

	// combine all added instance vertices to master instance vertex list & upload
	/*std::vector<float> iv;
	for (uint16_t i=0;i<iml.size();i++) iv.insert(iv.end(),iml[i].v.begin(),iml[i].v.end());
	ibuffer.bind();
	ibuffer.upload_vertices(iv);*/
}

/*
	load_texture() -> void
	purpose: texture all meshes & define texture locations in shader program
*/
void Renderer3D::load_texture()
{
	// run texturing process for all meshes
	for(uint16_t i=0;i<ml.size();i++) ml[i].texture();
	//for(uint16_t i=0;i<iml.size();i++) iml[i].texture();
}

/*
	load(Camera3D*) -> void
	cam3d: camera to relate mesh objects to
	purpose: combine texture and vertex loading, define gl settings & compile shader program
*/
void Renderer3D::load(Camera3D* cam3d)
{
	// compile shadow shader
	shs.compile3d("shader/fbv_shadow.shader","shader/fbf_shadow.shader");

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
	s3d.upload_camera(*cam3d);

	// combine all added instance vertices to master instance vertex list & upload
	std::vector<float> iv;
	for (uint16_t i=0;i<iml.size();i++) iv.insert(iv.end(),iml[i].v.begin(),iml[i].v.end());
	ibuffer.bind();
	ibuffer.upload_vertices(iv);

	// compile instance shader & load textures
	is3d.compile3d("shader/vertexi3d.shader","shader/fragmenti3d.shader");
	ibuffer.bind_index();
	is3d.def_indexF(ibuffer.get_indices(),"offset",3,0,3);
	for(uint16_t i=0;i<iml.size();i++) iml[i].texture();
	is3d.upload_int("tex",0);
	is3d.upload_int("sm",1);
	is3d.upload_int("emit",2);
	is3d.upload_int("shadow_map",3);
	is3d.upload_int("nmap",4);
	is3d.upload_camera(*cam3d);
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
	prepare(Camera3D*) -> void
	cam3d: camera to relate mesh visuals to
	purpose: additionally to the function of prepare, this method updates and uploads the camera
*/
void Renderer3D::prepare(Camera3D* cam3d)
{
	// run normal preparations
	prepare();

	// update and upload camera & position for normal mapping
	s3d.upload_camera(*cam3d);
	s3d.upload_vec3("view_pos",cam3d->pos);
}

/*
	TODO
*/
void Renderer3D::prepare_inst(Camera3D* cam3d)
{
	// gl settings
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);

	// prepare shader & buffer
	is3d.enable();
	ibuffer.bind();

	// update camera
	is3d.upload_camera(*cam3d);
	is3d.upload_vec3("view_pos",cam3d->pos);
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
	TODO
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