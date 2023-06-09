#ifndef CCB_GRAPHICS_MESH
#define CCB_GRAPHICS_MESH

#include <iostream>
#include <cstring>
#include <vector>

#include <GL/glew.h>

#include "../mat/toolbox.h"

// virtuals for a geometry class, capable of casting shadows
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
			glm::vec3 ip,float is,glm::vec3 ir,uint32_t &mofs);
	~Mesh() {  }

	// setup
	void texture();

public:

	// upload data
	std::vector<float> v;
	GLuint tex,specmap,normap,emitmap;

	// transformation
	glm::mat4 model = glm::mat4(1);
	glm::vec3 pos,rot;
	float scl;

	// information
	uint16_t ofs,size,inst_count = 0;

private:

	// information
	const char* texpath,*smpath,*nmpath,*empath;
};

#endif

// TODO: ??maybe separate single draw call mesh from instanced mesh