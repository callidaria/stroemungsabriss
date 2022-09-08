#pragma once

#include <iostream>
#include "shader.h"
#include "mesh.h"
#include "../mat/camera3d.h"
#include "../fcn/buffer.h"

class Renderer3D
{
public:

	// construction
	Renderer3D();
	~Renderer3D();

	void add(const char* m,const char* t,const char* sm,const char* nm,const char* em,
			glm::vec3 p,float s,glm::vec3 r);

	void load_vertex();
	void load_texture();
	void load(Camera3D* cam3d);

	// preparations
	void prepare();
	void prepare_wcam(Camera3D* cam3d);

	// draw
	void render_mesh(uint16_t b,uint16_t e);

	// uploads
	void upload_model(glm::mat4 m);
	void upload_view(glm::mat4 m);
	void upload_proj(glm::mat4 m);
	void upload_shadow(glm::mat4 m);
private:
	Buffer buffer = Buffer();
	unsigned int mofs = 0;
public:
	Shader s3d,shs;
	std::vector<Mesh> ml;
};