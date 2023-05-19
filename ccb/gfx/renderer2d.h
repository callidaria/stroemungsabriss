#ifndef CCB_GRAPHICS_RENDERER2D
#define CCB_GRAPHICS_RENDERER2D

#include <iostream>
#include <vector>

#include "shader.h"
#include "sprite.h"
#include "anim.h"

#include "../mat/camera2d.h"
#include "../mat/toolbox.h"

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
	void load();
	void load(Camera2D* cam2d);

	// preparation
	void prepare();

	// renderers
	void render_sprite(uint16_t b, uint16_t e);
	void render_sprite(uint16_t b,uint16_t e,GLuint tex);
	void render_state(uint16_t s,glm::vec2 i);
	void render_anim(uint16_t i);

	// settings
	void reset_shader();

	// getters
	uint16_t get_max_sprite();
	uint16_t get_max_anim();

private:

	// loaders
	void load_vertex();
	void load_texture();

public:

	Buffer buffer = Buffer();
	Shader s2d;
	std::vector<Sprite> sl;
	std::vector<Anim> al;
};

#endif