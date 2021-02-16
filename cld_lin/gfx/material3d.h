#pragma once

#include <iostream>
#include "renderer3d.h"

class Material3D
{
public:
	Material3D(Renderer3D* in_r3d,int in_rep,int in_spec,float in_intense)
		: r3d(in_r3d),rep(in_rep),spec(in_spec),intense(in_intense)
	{  }
	void upload()
	{
		r3d->s3d.upload_int("tex_repeat",rep);
		r3d->s3d.upload_int("specular",spec);
		r3d->s3d.upload_float("spec_int",intense);
	}
public:
	Renderer3D* r3d;
	int rep,spec;
	float intense;
};
