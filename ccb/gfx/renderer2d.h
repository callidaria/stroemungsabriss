#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "sprite.h"
#include "anim.h"
#include "../mat/camera2d.h"
#include "../fcn/buffer.h"

class Renderer2D
{
public:
	Renderer2D()
	{
		buffer = Buffer();
		buffer.add_buffer();
	}
	int add(glm::vec2 p,float w,float h,const char* t)
	{
		Sprite proc = Sprite(p,w,h,t);
		sl.push_back(proc);
		return sl.size()-1; // less CPU expensive without sub ??TIME over SPACE
	}
	int add(glm::vec2 p,float w,float h,const char* t,unsigned int r,unsigned int c,unsigned int f,
			unsigned int itn)
	{
		Anim proc = Anim(p,w,h,t,r,c,f,itn);
		al.push_back(proc);
		return al.size()-1;
	}
	void load_vertex()
	{
		int ls = sl.size(); int la = al.size();
		float v[(ls+la)*16]; unsigned int e[(ls+la)*6];
		for (int j = 0; j < ls+la; j++) {
			for (int i = 0; i < 16; i++) {
				if (j<ls) v[j*16+i] = sl.at(j).v[i];
				else v[j*16+i] = al.at(j-ls).v[i];
			}
			e[j*6] = j*4; e[j*6+1] = j*4+1; e[j*6+2] = j*4+2;
			e[j*6+3] = j*4+2; e[j*6+4] = j*4+3; e[j*6+5] = j*4+0;
		}
		buffer.bind();
		buffer.upload_vertices(v,sizeof(v));
		buffer.upload_elements(e,sizeof(e));
	}
	void load_texture()
	{
		for (int i = 0; i < sl.size(); i++) sl.at(i).texture();
		for (int i = 0; i < al.size(); i++) al.at(i).texture();
	}
	void load()
	{
		load_vertex();
		s2d.compile2d("shader/vertex2d.shader","shader/fragment2d.shader");
		load_texture();
	}
	void load_wcam(Camera2D* c)
	{ 
		load_vertex();
		s2d.compile2d("shader/vertex2d.shader","shader/fragment2d.shader");
		load_texture();
		upload_view(c->view2D);
		upload_proj(c->proj2D);
	}
	void prepare()
	{
		s2d.enable();
		reset_shader();
		buffer.bind();
		glDisable(GL_CULL_FACE);
		glDisable(GL_DEPTH_TEST);
		glActiveTexture(GL_TEXTURE0);
	}
	void render_sprite(int b, int e)
	{
		for (int i = b; i < e; i++) {
			upload_model(sl.at(i).model); // ??how expensive exactely
			glBindTexture(GL_TEXTURE_2D, sl.at(i).tex);
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
		}
	}
	void render_state(int s, glm::vec2 i) // !!float is taken but integer is needed. SPACE
	{
		glBindTexture(GL_TEXTURE_2D,al.at(s).tex);
		upload_row(al.at(s).r);upload_col(al.at(s).c);upload_tindex(i);
		upload_model(al.at(s).model);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT,(void*)((s+sl.size())*6*sizeof(int)));
	}
	void render_anim(int i)
	{
		int index=al.at(i).setup();int r=al.at(i).r;int c=al.at(i).c;
		glm::vec2 ind = glm::vec2((int)(index%c),(int)(index/c));
		upload_row(r);
		upload_col(c);
		upload_tindex(ind);
		upload_model(al.at(i).model);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sl.size())*6*sizeof(int)));
	}
	void reset_shader()
	{
		upload_row(1);
		upload_col(1);
		upload_tindex(glm::vec2(0,0));
	}
	int get_max_sprite() { return sl.size(); }
	int get_max_anim() { return al.size(); }
	void upload_model(glm::mat4 m) { s2d.upload_matrix("model",m); }
	void upload_view(glm::mat4 m) { s2d.upload_matrix("view",m); }
	void upload_proj(glm::mat4 m) { s2d.upload_matrix("proj",m); }
	void upload_row(int i) { s2d.upload_int("row",i); }
	void upload_col(int i) { s2d.upload_int("col",i); }
	void upload_tindex(glm::vec2 v) { s2d.upload_vec2("i_tex",v); }
public:
	Buffer buffer;
	Shader s2d;
	std::vector<Sprite> sl;
	std::vector<Anim> al;
};
