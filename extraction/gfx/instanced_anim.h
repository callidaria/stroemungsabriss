#ifndef CCB_GRAPHICS_ANIMATED_INSTANCE
#define CCB_GRAPHICS_ANIMATED_INSTANCE

#include <iostream>
#include <vector>

#include "../mat/toolbox.h"

#include "shader.h"

constexpr uint32_t IANIMATION_MCAP = 4096;
constexpr uint32_t IANIMATION_REPEAT = 6;
constexpr uint32_t IANIMATION_VALUE_RANGE = IANIMATION_MCAP*IANIMATION_REPEAT;
// FIXME: all of this always has to be equal to their counterparts in instance.h! remove & replace

class InstancedAnim
{
public:

	// construction
	InstancedAnim(glm::vec2 p,float w,float h,const char* tex_paths,uint8_t irow,uint8_t icol,
			uint8_t itn,uint8_t f);
	~InstancedAnim() {  }

	// setup
	inline void texture() { Toolbox::load_texture(tex,tpath); }
	void setup(Shader* shader);
	void update(float dtime);
	inline void reset_tick(uint16_t idx) { ticks[idx] = 0; }

public:

	// data
	uint32_t tex;
	float i[IANIMATION_VALUE_RANGE];

	// transform
	glm::mat4 model = glm::mat4(1.f);
	glm::vec2 position;
	float width,height;

private:

	// texture
	const char* tpath;

	// animation
	std::vector<float> ticks = std::vector<float>(IANIMATION_MCAP,0);
	uint8_t row,col,st_count,tick_cap,tps;
};

#endif
