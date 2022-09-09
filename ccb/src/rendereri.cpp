#include "../gfx/rendereri.h"

/*
	constructor()
	purpose: purpose: create renderer object to subsequently add instances to and draw them
*/
RendererI::RendererI()
{
	buffer.add_buffer();
} RendererI::~RendererI() {  }

/*
	add(vec2,float,float,const char*) -> uint16_t
	p: origin position of indexed 2D object
	w: x-axis scaling resulting in initial object width
	h: y-axis scaling resulting in initial object height
	t: path to file holding the indexed objects texture
	purpose: creating and adding an indexable 2D object to render memory
	returns: memory index the created instanceable object is to referred by when drawing
*/
uint16_t RendererI::add(glm::vec2 p,float w,float h,const char* t)
{
	uint16_t out = il.size();
	il.push_back(Instance(p,w,h,t));
	return out;
}

/*
	load_vertex() -> void
	purpose: upload all added vertices to buffer
*/
void RendererI::load_vertex()
{
	// clear memory for vertex list
	float v[il.size()*24];

	// load all vertices to master array
	for (int j=0;j<il.size();j++) {
		for (int i=0;i<24;i++)
			v[j*24+i] = il.at(j).v[i];
	}

	// upload to buffer
	buffer.bind();
	buffer.upload_vertices(v,sizeof(v));
}

/*
	load_texture() -> void
	purpose: texture all indexable objects
*/
void RendererI::load_texture()
{
	for (int i=0;i<il.size();i++) il.at(i).texture();
}

/*
	load() -> void
	purpose: combine vertex upload and texturing and compile shader
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
	load(Camera2D*) -> void
	cam2d: camera to render indexable 2D objects in relation to
	purpose: in addition to using the features of load the camera matrices get uploaded
*/
void RendererI::load(Camera2D* cam2d)
{
	load();

	// upload camera to shader program
	sI.upload_camera(*cam2d);
}

/*
	prepare() -> void
	purpose: enable shader program and bind buffer to use its data
*/
void RendererI::prepare()
{
	sI.enable();
	buffer.bind();
}

/*
	render(uint16_t,uint16_t) -> void
	i: memory index of previously added indexable object
	amt: amount of identical objects to render from indexed source
	purpose: draw object the target amount after uploading the position transforms to index buffer
*/
void RendererI::render(uint16_t i,uint16_t amt)
{
	// setup
	glBindTexture(GL_TEXTURE_2D,il.at(i).tex);
	buffer.upload_indices(il.at(i).o,sizeof(glm::vec2)*4096);

	// render instanced
	glDrawArraysInstanced(GL_TRIANGLES,i*6,i*6+6,amt);
}

/*
	set_offset(uint16_t,uint16_t,vec2) -> void
	i: memory index of indexable object
	j: memory index of position transform vector to change
	o: vector value to set the selected position transform vector to
	purpose: set a certain position transform vector of a specific object
*/
void RendererI::set_offset(uint16_t i,uint16_t j,glm::vec2 o)
{
	il.at(i).o[j] = o;
}