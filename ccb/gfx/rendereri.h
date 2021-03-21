#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "instance.h"
#include "../mat/camera2d.h"

class RendererI
{
public:
	RendererI()
	{
		glGenVertexArrays(1,&vao);
		glGenBuffers(1,&vbo); glGenBuffers(1,&ibo);
	}
	void add(glm::vec2 p, float w, float h, const char* t)
	{ Instance proc = Instance(p,w,h,t); il.push_back(proc); }
	void load_vertex()
	{
		int li = il.size(); float v[li*24];
		for (int j = 0; j < li; j++) {
			for (int i = 0; i < 24; i++) v[j*24+i] = il.at(j).v[i];
		}
		glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER, sizeof(v), v, GL_STATIC_DRAW);
	}
	void load_texture()
	{
		for (int i = 0; i < il.size(); i++) il.at(i).texture();
	}
	void load()
	{
		load_vertex();
		sI.compile2d("shader/vertex_inst.shader",
				"shader/fragment_inst.shader");
		sI.load_index(ibo);
		load_texture();
	}
	void load_wcam(Camera2D* c)
	{
		load_vertex();
		sI.compile2d("shader/vertex_inst.shader",
				"shader/fragment_inst.shader");
		sI.load_index(ibo);
		load_texture();
		upload_view(c->view2D); upload_proj(c->proj2D);
	}
	void prepare() { sI.enable(); glBindVertexArray(vao); }
	void render(int i, int amt)
	{
		glBindTexture(GL_TEXTURE_2D, il.at(i).tex);
		glBindBuffer(GL_ARRAY_BUFFER,ibo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*1000,
				il.at(i).o,GL_DYNAMIC_DRAW);
		glDrawArraysInstanced(GL_TRIANGLES, i*6, i*6+6, amt);
	}
	void set_offset(int i, int j, glm::vec2 o)
	{ il.at(i).o[j] = o; }
	void upload_model(glm::mat4 m) { sI.upload_matrix("model",m); }
	void upload_view(glm::mat4 m) { sI.upload_matrix("view",m); }
	void upload_proj(glm::mat4 m) { sI.upload_matrix("proj",m); }
private:
	unsigned int vao, vbo, ibo;
public:
	Shader sI;
	std::vector<Instance> il;
};
