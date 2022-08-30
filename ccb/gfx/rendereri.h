#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "instance.h"
#include "../mat/camera2d.h"
#include "../fcn/buffer.h"

class RendererI
{
public:
	RendererI()
	{
		buffer = Buffer();
		buffer.add_buffer();
	}
	void add(glm::vec2 p, float w, float h, const char* t)
	{ Instance proc = Instance(p,w,h,t); il.push_back(proc); }
	void load_vertex()
	{
		int li = il.size(); float v[li*24];
		for (int j = 0; j < li; j++) {
			for (int i = 0; i < 24; i++)
				v[j*24+i] = il.at(j).v[i];
		}
		buffer.bind();
		buffer.upload_vertices(v,sizeof(v));
	}
	void load_texture()
	{
		for (int i = 0; i < il.size(); i++) il.at(i).texture();
	}
	void load()
	{
		load_vertex();
		sI.compile2d("shader/vertex_inst.shader","shader/fragment_inst.shader");
		buffer.bind_index();
		sI.def_indexF(buffer.get_indices(),"offset",2,0,2);
		load_texture();
	}
	void load_wcam(Camera2D* c)
	{
		load_vertex();
		sI.compile2d("shader/vertex_inst.shader","shader/fragment_inst.shader");
		buffer.bind_index();
		sI.def_indexF(buffer.get_indices(),"offset",2,0,2);
		load_texture();
		upload_view(c->view2D); upload_proj(c->proj2D);
	}
	void prepare() {
		sI.enable();
		buffer.bind();
	}
	void render(int i, int amt)
	{
		glBindTexture(GL_TEXTURE_2D, il.at(i).tex);
		buffer.upload_indices(il.at(i).o,sizeof(glm::vec2)*4096);
		glDrawArraysInstanced(GL_TRIANGLES, i*6, i*6+6, amt);
	}
	void set_offset(int i, int j, glm::vec2 o)
	{ il.at(i).o[j] = o; }
	void upload_model(glm::mat4 m) { sI.upload_matrix("model",m); }
	void upload_view(glm::mat4 m) { sI.upload_matrix("view",m); }
	void upload_proj(glm::mat4 m) { sI.upload_matrix("proj",m); }
private:
	Buffer buffer;
public:
	Shader sI;
	std::vector<Instance> il;
};
