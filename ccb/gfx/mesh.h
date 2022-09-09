#pragma once

#include <iostream>
#include <cstring>
#include <vector>

#include <GL/glew.h>

#include <glm/gtx/rotate_vector.hpp>

#ifdef __WIN32__
#define STB_IMAGE_IMPLEMENTATION
#ifndef STBI_INCLUDE_STB_IMAGE_H
#include "../../include/stb_image.h"
#endif

#else
#include <SOIL/SOIL.h>
#endif

class Mesh
{
public:

	// construction
	Mesh(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 ip,float is,glm::vec3 ir,unsigned int* mofs);
	~Mesh();

	// setup
	void texture();

private:

	// helpers
	//void load_texture(unsigned int &tex,);
	glm::vec3 transform(glm::vec3 o,glm::vec3 p,float s,glm::vec3 r);
	glm::vec3 rotate(glm::vec3 o,glm::vec3 r);

public:

	// upload data
	std::vector<float> v;
	unsigned int tex,specmap,normap,emitmap;  // FIXME: unsigned to uint

	// transformation
	glm::vec3 pos,rot;
	float scl;

	// information
	unsigned int ofs,size;

private:

	// information
	const char* texpath,*smpath,*nmpath,*empath;
};