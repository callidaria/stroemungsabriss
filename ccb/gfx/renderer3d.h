#pragma once

#include <iostream>
#include "shader.h"
#include "mesh.h"
#include "../mat/camera3d.h"
#include "../fcn/buffer.h"

class Renderer3D
{
public:
	Renderer3D() { Buffer buffer; }
	void add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r)
	{ Mesh proc=Mesh(m,t,sm,nm,em,p,s,r,&mofs);ml.push_back(proc); }
	void load_vertex()
	{
		int lm=ml.size();unsigned int vs=0;int io=0;
		for (int h=0;h<lm;h++) vs+=ml.at(h).v.size();
		/*float v[vs];*/std::vector<float> v;
		for (int j=0;j<lm;j++) {
			for (int i=0;i<ml.at(j).v.size();i++)
				//v[io+i]=ml.at(j).v[i];
				v.push_back(ml.at(j).v[i]);
			io+=ml.at(j).v.size();
		} buffer.bind();
		buffer.upload_vertices(v);
	}
	void load_texture()
	{
		for(int i=0;i<ml.size();i++) ml.at(i).texture();
		s3d.upload_int("tex",0);
		s3d.upload_int("sm",1);
		s3d.upload_int("emit",2);
		s3d.upload_int("shadow_map",3);
		s3d.upload_int("nmap",4);
	}
	void load(Camera3D* c)
	{
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		load_vertex();
		shs.compile3d("shader/fbv_shadow.shader","shader/fbf_shadow.shader");
		s3d.compile3d("shader/vertex3d.shader","shader/fragment3d.shader");
		load_texture();
		upload_view(c->view3D);upload_proj(c->proj3D);
	}
	void prepare()
	{
		s3d.enable();
		buffer.bind();
		glActiveTexture(GL_TEXTURE0);
	}
	void prepare_wcam(Camera3D* c)
	{
		prepare();
		c->update();upload_view(c->view3D);upload_proj(c->proj3D);
		s3d.upload_vec3("view_pos",c->pos);
	}
	void render_mesh(int b,int e)
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
	void upload_model(glm::mat4 m) { s3d.upload_matrix("model",m); }
	void upload_view(glm::mat4 m) { s3d.upload_matrix("view",m); }
	void upload_proj(glm::mat4 m) { s3d.upload_matrix("proj",m); }
	void upload_shadow(glm::mat4 m) { s3d.upload_matrix("light_trans",m); }
private:
	Buffer buffer;
	unsigned int mofs = 0;
public:
	Shader s3d,shs;
	std::vector<Mesh> ml;
};
