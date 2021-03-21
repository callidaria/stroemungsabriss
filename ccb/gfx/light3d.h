#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "renderer3d.h"

class Light3D
{
public:
	Light3D() {  }
	Light3D(Renderer3D* in_r3d,int in_ind,glm::vec3 in_pos,glm::vec3 in_col,float in_ins)
		:r3d(in_r3d),ind(in_ind),pos(in_pos),col(in_col),ins(in_ins) { }
	void upload()
	{
		std::string base="al["+std::to_string(ind)+"].";
		r3d->s3d.enable();
		r3d->s3d.upload_vec3((base+"pos").c_str(),pos);
		r3d->s3d.upload_vec3((base+"col").c_str(),col);
		r3d->s3d.upload_float((base+"ins").c_str(),ins);
		r3d->s3d.upload_vec3("light_pos",pos);
		r3d->s3d.enable();
	}
	void set_amnt(int n)
	{
		r3d->s3d.enable();
		r3d->s3d.upload_int("amnt_light_sun",n);
		r3d->s3d.enable();
	}
	void create_shadow(glm::vec3 to,float width,float height,float f,int res)
	{
		sh_res=res;
		glGenFramebuffers(1,&depth_fbo);glGenTextures(1,&dtex);
		glBindTexture(GL_TEXTURE_2D,dtex);
		glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,res,res,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_BORDER);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_BORDER);
		float bc[] = { 1.0f,1.0f,1.0f,1.0f };
		glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,bc);
		glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,dtex,0);
		glDrawBuffer(GL_NONE);glReadBuffer(GL_NONE);
		glBindFramebuffer(GL_FRAMEBUFFER,0);

		proj=glm::ortho(-width/2,width/2,-height/2,height/2,0.1f,100.0f);
		view=glm::lookAt(pos/f+to,to,glm::vec3(0,1,0));
		shadow_mat=proj*view;
	}
	void prepare_shadow()
	{
		glViewport(0,0,sh_res,sh_res);
		glBindFramebuffer(GL_FRAMEBUFFER,depth_fbo);
		glEnable(GL_CULL_FACE);glEnable(GL_DEPTH_TEST);glCullFace(GL_FRONT);
		glClearColor(0,0,0,1);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
		r3d->prepare();
		r3d->upload_view(view);
		r3d->upload_proj(proj);

	}
	void close_shadow(int w_res, int h_res)
	{
		glBindFramebuffer(GL_FRAMEBUFFER,0);
		glViewport(0,0,w_res,h_res);
		glCullFace(GL_BACK);
	}
	void upload_shadow()
	{
		r3d->upload_shadow(shadow_mat);
		glActiveTexture(GL_TEXTURE3);
		glBindTexture(GL_TEXTURE_2D,dtex);
	}
	void upload_shadow_terra()
	{
		glActiveTexture(GL_TEXTURE1);glBindTexture(GL_TEXTURE_2D,dtex);
	}
public:
	Renderer3D* r3d;
	int ind;
	glm::vec3 pos,col;
	float ins;
	int sh_res;
	unsigned int depth_fbo,dtex;
	glm::mat4 proj,view,shadow_mat;
};
