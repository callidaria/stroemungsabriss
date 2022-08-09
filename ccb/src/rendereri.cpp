#include "../gfx/rendereri.h"

/*
	constructor()
	[...]
*/
RendererI::RendererI()
{
	buffer.add_buffer();
} RendererI::~RendererI() {  }

/*
	add(vec2,float,float,const char*) -> void
	[...]
*/
void RendererI::add(glm::vec2 p,float w,float h,const char* t)
{
	Instance tmp = Instance(p,w,h,t);  // FIXME: fuse creation into push_back
	il.push_back(tmp);
}

/*
	load_vertex() -> void
	[...]
*/
void RendererI::load_vertex()
{
	// setup
	int li = il.size();
	float v[li*24];

	// load all vertices to master array
	for (int j=0;j<li;j++) {
		for (int i=0;i<24;i++)
			v[j*24+i] = il.at(j).v[i];
	}

	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(v,sizeof(v));
}

/*
	load_texture() -> void
	[...]
*/
void RendererI::load_texture()
{
	for (int i=0;i<il.size();i++)
		il.at(i).texture();
}

/*
	load() -> void
	[...]
*/
void RendererI::load()
{
	load_vertex();
	sI.compile2d("shader/vertex_inst.shader","shader/fragment_inst.shader");
	buffer.bind_index();
	sI.def_indexF(buffer.get_indices(),"offset",2,0,2);
	load_texture();
}

/*
	load_wcam(Camera2D*) -> void
	[...]
*/
void RendererI::load_wcam(Camera2D* c)
{
	load();

	// upload camera to shader program
	upload_view(c->view2D);
	upload_proj(c->proj2D);
}

/*
	prepare() -> void
	[...]
*/
void RendererI::prepare()
{
	sI.enable();
	buffer.bind();
}

/*
	render(uint16_t,uint16_t) -> void
	[...]
*/
void RendererI::render(uint16_t i, uint16_t amt)
{
	glBindTexture(GL_TEXTURE_2D,il.at(i).tex);
	buffer.upload_indices(il.at(i).o,sizeof(glm::vec2)*4096);
	glDrawArraysInstanced(GL_TRIANGLES,i*6,i*6+6,amt);
}

/*
	set_offset(uint16_t,uint16_t,vec2) -> void
	[...]
*/
void RendererI::set_offset(uint16_t i, uint16_t j, glm::vec2 o)
{
	il.at(i).o[j] = o;
}

/*
	upload_model(mat4) -> void
	[...]
*/
void RendererI::upload_model(glm::mat4 m)
{
	sI.upload_matrix("model",m);
}

/*
	DEPRECATED: this will be a joined shader function in the future
*/
void RendererI::upload_view(glm::mat4 m) { sI.upload_matrix("view",m); }
void RendererI::upload_proj(glm::mat4 m) { sI.upload_matrix("proj",m); }