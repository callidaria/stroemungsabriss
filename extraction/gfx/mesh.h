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
			glm::vec3 pos,float scale,glm::vec3 rot,std::vector<float> &vl,uint32_t &mofs);
	~Mesh() {  }

	// setup
	void texture();

public:

	// data
	uint32_t tex,specmap,normap,emitmap;
	uint32_t ofs,size;

	// transformation
	glm::mat4 model = glm::mat4(1);

private:

	// information
	const char* texpath,*smpath,*nmpath,*empath;
};

#endif
