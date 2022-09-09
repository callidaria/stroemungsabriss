#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "instance.h"
#include "../mat/camera2d.h"
#include "../fcn/buffer.h"

class RendererI
{
public:

	// construction
	RendererI();
	~RendererI();

	// adders
	uint16_t add(glm::vec2 p,float w,float h,const char* t);

	// loaders
	void load();
	void load(Camera2D* cam2d);

	// preparation
	void prepare();

	// draw
	void render(uint16_t i,uint16_t amt);

	// uploads
	void set_offset(uint16_t i,uint16_t j,glm::vec2 o);
	void upload_model(glm::mat4 m);
	void upload_view(glm::mat4 m);
	void upload_proj(glm::mat4 m);

private:

	// loaders
	void load_vertex();
	void load_texture();

private:

	Buffer buffer = Buffer();

public:

	Shader sI;
	std::vector<Instance> il;
};