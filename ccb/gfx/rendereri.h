#ifndef CCB_GRAPHICS_INSTANCED_RENDERER
#define CCB_GRAPHICS_INSTANCED_RENDERER

#include <iostream>
#include <vector>

#include "shader.h"
#include "instance.h"

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
	~RendererI() {  }

	// adders
	uint16_t add(glm::vec2 p,float w,float h,const char* t);
	uint16_t add(glm::vec2 p,float w,float h,const char* t,uint8_t row,uint8_t col,
			uint8_t itn,uint8_t f);

	// loaders
	void load();

	// preparation
	void prepare(float dtime);
	void reset_anim_tick(uint16_t cluster,uint16_t idx);

	// draw
	void render(uint16_t i,uint16_t amt);
	void render(uint16_t i,uint16_t amt,glm::vec2 i_tex);
	void render_anim(uint16_t i,uint16_t amt);

	// uploads
	glm::vec2 get_offset(uint16_t i,uint16_t j);
	glm::vec2 get_aOffset(uint16_t i,uint16_t j);
	void set_offset(uint16_t i,uint16_t j,glm::vec2 o);
	void set_aOffset(uint16_t i,uint16_t j,glm::vec2 o);
	void set_rotation(uint16_t i,uint16_t j,float r);
	void set_aRotation(uint16_t i,uint16_t j,float r);
	void add_offset(uint16_t i,uint16_t j,glm::vec2 dv);
	void add_aOffset(uint16_t i,uint16_t j,glm::vec2 dv);

	// getters
	uint16_t get_next_instindex();
	uint16_t get_next_animindex();

private:

	// cascabel
	Buffer buffer = Buffer();
	Shader sI = Shader();

	// instance object lists
	std::vector<Instance> il;
	std::vector<InstancedAnim> ial;
};

#endif