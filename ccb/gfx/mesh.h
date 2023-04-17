#pragma once

#include <iostream>
#include <cstring>
#include <vector>

#include <GL/glew.h>
#include <glm/gtx/rotate_vector.hpp>

#include "../mat/toolbox.h"

// viruals for a geometry class, capable of casting shadows
class ShadowGeometry
{
public:
	virtual ~ShadowGeometry() {  }
	virtual void render_shadow() {  }
};

class Mesh
{
public:

	// construction
	Mesh(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 ip,float is,glm::vec3 ir,unsigned int* mofs);
	~Mesh();

	// setup
	void texture();

	// TODO: object transformation features & store model matrix

private:

	// helpers
	void transform(glm::vec3 &o,glm::vec3 p,float s,glm::vec3 r);
	void rotate(glm::vec3 &o,glm::vec3 r);

public:

	// upload data
	std::vector<float> v;
	GLuint tex,specmap,normap,emitmap;

	// transformation
	glm::mat4 model = glm::mat4(1);
	glm::vec3 pos,rot;
	float scl;

	// information
	unsigned int ofs,size;

private:

	// information
	const char* texpath,*smpath,*nmpath,*empath;
};