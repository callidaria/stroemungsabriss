#pragma once

#include <iostream>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader2d.h"
#include "sprite.h"
#include "anim.h"
#include "camera2d.h"

class Renderer2D
{
public:
	Renderer2D()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"generating 2D buffers\n";
		glGenVertexArrays(1,&vao);
		glGenBuffers(1,&vbo); glGenBuffers(1,&ebo);
	}
	void add(glm::vec2 p, float w, float h, const char* t)
	{ Sprite proc = Sprite(p,w,h,t); sl.push_back(proc); }
	void add(glm::vec2 p,float w,float h,std::vector<const char*> t,unsigned int f) { Anim proc = Anim(p,w,h,t,f); al.push_back(proc); }
	void load_vertex()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 112);
		std::cout<<"\nloading 2D renderer\n\n";
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"loading 2D vertices\n";
		int ls = sl.size(); int la = al.size();
		float v[10000]; unsigned int e[10000];

		for (int j = 0; j < ls+la; j++) {
			for (int i = 0; i < 16; i++) {
				if (j<ls) v[j*16+i] = sl.at(j).v[i];
				else v[j*16+i] = al.at(j-ls).v[i];
			}
			e[j*6] = j*4; e[j*6+1] = j*4+1; e[j*6+2] = j*4+2;
			e[j*6+3] = j*4+2; e[j*6+4] = j*4+3; e[j*6+5] = j*4+0;
		}
		std::cout<<"uploading 2D vertices\n";
		glBindVertexArray(vao); glBindBuffer(GL_ARRAY_BUFFER,vbo);
		glBufferData(GL_ARRAY_BUFFER,sizeof(v),v,GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER,sizeof(e),e,
				GL_STATIC_DRAW);
	}
	void load_texture()
	{
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), 9);
		std::cout<<"texturing 2D objects\n";
		for (int i = 0; i < sl.size(); i++) sl.at(i).texture();
		for (int i = 0; i < al.size(); i++) al.at(i).texture();
	}
	void load() { load_vertex(); s2d.compile(); load_texture(); }
	void load_wcam(Camera2D* c)
	{ 
		load_vertex(); s2d.compile(); load_texture();
		upload_view(c->view2D); upload_proj(c->proj2D);
	}
	void prepare() { s2d.enable(); glBindVertexArray(vao); }
	void render_sprite(int b, int e)
	{
		for (int i = b; i < e; i++) {
			glBindTexture(GL_TEXTURE_2D, sl.at(i).tex);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
					(void*)(i*6*sizeof(int)));
		}
	}
	void render_state(int s, int i)
	{
		int ls = sl.size();
		glBindTexture(GL_TEXTURE_2D,al.at(s).tex[i]);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
				(void*)((s+ls)*6*sizeof(int)));
	}
	void render_anim(int b, int e)
	{
		int ls = sl.size();
		for (int i = b; i < e; i++) {
			al.at(i).setup();
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,
					(void*)((i+ls)*6*sizeof(int)));
		}
	}
	int get_max_sprite() { return sl.size(); }
	int get_max_anim() { return al.size(); }
	void upload_model(glm::mat4 m)
	{ glUniformMatrix4fv(s2d.modelUni, 1, GL_FALSE, glm::value_ptr(m)); }
	void upload_view(glm::mat4 m)
	{ glUniformMatrix4fv(s2d.viewUni, 1, GL_FALSE, glm::value_ptr(m)); }
	void upload_proj(glm::mat4 m)
	{ glUniformMatrix4fv(s2d.projUni, 1, GL_FALSE, glm::value_ptr(m)); }
private:
	unsigned int vao, vbo, ebo;
public:
	Shader2D s2d;
	std::vector<Sprite> sl; std::vector<Anim> al;
};
