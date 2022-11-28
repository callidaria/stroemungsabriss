#pragma once

#include <iostream>
#include <vector>

#include "../mat/camera2d.h"

#include "../fcn/buffer.h"

#include "shader.h"
#include "instance.h"
#include "instanced_anim.h"

class RendererI
{
public:

	// construction
	RendererI();
	~RendererI();

	// adders
	uint16_t add(glm::vec2 p,float w,float h,const char* t);
	uint16_t add(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,
			uint8_t itn,uint8_t f);

	// loaders
	void load();
	void load(Camera2D* cam2d);

	// preparation
	void prepare();

	// draw
	void render(uint16_t i,uint16_t amt);
	void render(uint16_t i,uint16_t amt,glm::vec2 i_tex);
	void render_anim(uint16_t i,uint16_t amt);

	// uploads
	glm::vec2 get_offset(uint16_t i,uint16_t j);
	void set_offset(uint16_t i,uint16_t j,glm::vec2 o);
	void add_offset(uint16_t i,uint16_t j,glm::vec2 dv);

private:

	// loaders
	void load_vertex();
	void load_texture();

private:

	Buffer buffer = Buffer();
	Buffer aBuffer = Buffer();

public:

	// classic instances
	Shader sI;
	std::vector<Instance> il;

	// animated instances
	Shader sAI;
	std::vector<InstancedAnim> ial;
};