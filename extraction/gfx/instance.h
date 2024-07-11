#ifndef CCB_GRAPHICS_INSTANCE
#define CCB_GRAPHICS_INSTANCE

#include <iostream>
#include <glm/glm.hpp>

#include "../mat/toolbox.h"
#include "texture.h"

constexpr uint32_t INSTANCE_MCAP = 4096;
constexpr uint32_t INSTANCE_REPEAT = 6;
constexpr uint32_t INSTANCE_VALUE_RANGE = INSTANCE_MCAP*INSTANCE_REPEAT;

class Instance
{
public:

	// construction
	Instance(glm::vec2 p,float w,float h,const char* t);
	~Instance() {  }

	// setup
	inline void setup() { tex.bind(); }

public:

	// data
	//uint32_t tex;
	Texture tex;
	float o[INSTANCE_VALUE_RANGE];

	// transform
	glm::mat4 model = glm::mat4(1.f);
	glm::vec2 position;
	float width,height;
};

#endif
