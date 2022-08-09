#pragma once

#include <iostream>
#include <vector>
#include "shader.h"
#include "sprite.h"
#include "anim.h"
#include "../mat/camera2d.h"
#include "../fcn/buffer.h"

class Renderer2D
{
public:

	// construction
	Renderer2D();
	~Renderer2D();

	// overloaded adding functions
	uint16_t add(glm::vec2 p,float w,float h,const char* t);
	uint16_t add(glm::vec2 p,float w,float h,const char* t,uint8_t r,uint8_t c,
			uint8_t f,uint8_t itn);

	// loaders
	void load_vertex();  // TODO: make private
	void load_texture();  // TODO: make private
	void load();
	void load_wcam(Camera2D* c);  // TODO: overload with load()

	// preparation
	void prepare();

	// renderers
	void render_sprite(uint16_t b, uint16_t e);
	void render_sprite(uint16_t b,uint16_t e,unsigned int tex);
	void render_state(uint16_t s,glm::vec2 i);  // !!float is taken but integer is needed. SPACE
	void render_anim(uint16_t i);

	// helpers
	void reset_shader();

	// uploads
	void upload_model(glm::mat4 m);
	void upload_view(glm::mat4 m);
	void upload_proj(glm::mat4 m);
	void upload_row(uint8_t i);
	void upload_col(uint8_t i);
	void upload_tindex(glm::vec2 v);
	// FIXME: check if some are replacable or obsolete

	// getters
	uint16_t get_max_sprite();
	uint16_t get_max_anim();

public:
	Buffer buffer = Buffer();
	Shader s2d;
	std::vector<Sprite> sl;
	std::vector<Anim> al;
};
