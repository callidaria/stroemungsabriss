#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "renderer3d.h"

class PointLight3D
{
public:
	PointLight3D(Renderer3D* in_r3d,int in_ind,glm::vec3 in_pos,
			glm::vec3 in_col,float in_const,float in_lin,
			float in_quad,float in_ins)
		: r3d(in_r3d),ind(in_ind),pos(in_pos),col(in_col),
			lconst(in_const),lin(in_lin),quad(in_quad),ins(in_ins)
	{ }
	void upload()
	{
		std::string base="pl["+std::to_string(ind)+"].";
		r3d->s3d.upload_vec3((base+"pos").c_str(),pos);
		r3d->s3d.upload_vec3((base+"col").c_str(),col);
		r3d->s3d.upload_float((base+"lconst").c_str(),lconst);
		r3d->s3d.upload_float((base+"lin").c_str(),lin);
		r3d->s3d.upload_float((base+"quad").c_str(),quad);
		r3d->s3d.upload_float((base+"ins").c_str(),ins);
	}
	void set_amnt(int n) { r3d->s3d.upload_int("amnt_light_point",n); }
private:
	Renderer3D* r3d;
	int ind;
	glm::vec3 pos,col;
	float lconst,lin,quad,ins;
};
