#include "../gfx/renderer3d.h"

/*
	constructor()
	[...]
*/
Renderer3D::Renderer3D() {  }
Renderer3D::~Renderer3D() {  }

/*
	add(const char*,const char*,const char*,const char*,const char*,vec3,float,vec3)
	[...]
*/
void Renderer3D::add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
		glm::vec3 p,float s,glm::vec3 r)
{
	Mesh tmp = Mesh(m,t,sm,nm,em,p,s,r,&mofs);  // FIXME: let tmp fall into push back call
	ml.push_back(tmp);
}

/*
	load_vertex() -> void
	[...]
*/
void Renderer3D::load_vertex()
{
	// writing all mesh vertices to a giant vertex list
	std::vector<float> v;
	for (int j=0;j<ml.size();j++) {
		for (int i=0;i<ml.at(j).v.size();i++)
			v.push_back(ml.at(j).v[i]);
	}

	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(v);
}

/*
	load_texture() -> void
	[...]
*/
void Renderer3D::load_texture()
{
	// run texturing process for all meshes
	for(int i=0;i<ml.size();i++)
		ml.at(i).texture();

	// texture references for shader program
	s3d.upload_int("tex",0);
	s3d.upload_int("sm",1);
	s3d.upload_int("emit",2);
	s3d.upload_int("shadow_map",3);
	s3d.upload_int("nmap",4);
}

/*
	load(Camera3D*) -> void
	[...]
*/
void Renderer3D::load(Camera3D* c)
{
	// opengl setup
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);
	// FIXME: do this in frame and change less rigidy

	// load and shader compilation
	load_vertex();
	shs.compile3d("shader/fbv_shadow.shader","shader/fbf_shadow.shader");
	s3d.compile3d("shader/vertex3d.shader","shader/fragment3d.shader");
	load_texture();

	// upload camera specifications to shader
	upload_view(c->view3D);
	upload_proj(c->proj3D);
}

/*
	prepare() -> void
	[...]
*/
void Renderer3D::prepare()
{
	s3d.enable();
	buffer.bind();
	glActiveTexture(GL_TEXTURE0);
}

/*
	prepare_wcam(Camera3D*) -> void
	[...]
*/
void Renderer3D::prepare_wcam(Camera3D* c)
{
	// run normal preparations
	prepare();

	// update and upload camera
	c->update();
	upload_view(c->view3D);
	upload_proj(c->proj3D);

	// upload shader specifications for normal mapping
	s3d.upload_vec3("view_pos",c->pos);
}

/*
	render_mesh(uint16_t,uint16_t) -> void
	[...]
*/
void Renderer3D::render_mesh(uint16_t b,uint16_t e)
{
	for (int i=b;i<e;i++) {
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).tex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).specmap);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).emitmap);
		glActiveTexture(GL_TEXTURE4);
		glBindTexture(GL_TEXTURE_2D,ml.at(i).normap);
		glDrawArrays(GL_TRIANGLES,ml.at(i).ofs,ml.at(i).size);
	}
}

/*
	upload_model(mat4) -> void
	[...]
*/
void Renderer3D::upload_model(glm::mat4 m)
{
	s3d.upload_matrix("model",m);
}

/*
	upload_view(mat4) -> void
	[...]
*/
void Renderer3D::upload_view(glm::mat4 m)
{
	s3d.upload_matrix("view",m);
}
void Renderer3D::upload_proj(glm::mat4 m)
{
	s3d.upload_matrix("proj",m);
}
// FIXME: fuse both into upload_camera(mat4)

/*
	upload_shadow(mat4) -> void
	[...]
*/
void Renderer3D::upload_shadow(glm::mat4 m)
{
	s3d.upload_matrix("light_trans",m);
}