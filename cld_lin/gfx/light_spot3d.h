#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "renderer3d.h"

class SpotLight3D
{
public:
	SpotLight3D(Renderer3D* in_r3d,int in_ind,glm::vec3 in_pos,
			glm::vec3 in_col,glm::vec3 in_dir,float in_cutIn,
			float in_cutOut)
		: r3d(in_r3d),ind(in_ind),pos(in_pos),col(in_col),dir(in_dir),
			cutIn(in_cutIn),cutOut(in_cutOut)
	{ }
	void upload()
	{
		std::string base="sl["+std::to_string(ind)+"].";
		r3d->s3d.upload_vec3((base+"pos").c_str(),pos);
		r3d->s3d.upload_vec3((base+"col").c_str(),col);
		r3d->s3d.upload_vec3((base+"dir").c_str(),dir);
		r3d->s3d.upload_float((base+"cut_in").c_str(),
				glm::cos(glm::radians(cutIn)));
		r3d->s3d.upload_float((base+"cut_out").c_str(),
				glm::cos(glm::radians(cutOut)));
	}
	void set_amnt(int n) { r3d->s3d.upload_int("amnt_light_spot",n); }
private:
	Renderer3D* r3d;
	int ind;
	glm::vec3 pos,col,dir;
	float cutIn,cutOut;
};
