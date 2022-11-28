#pragma once

#include <iostream>
#include <vector>

#include "../mat/toolbox.h"

constexpr uint32_t IANIMATION_MCAP = 4096;

class InstancedAnim
{
public:

	// construction
	InstancedAnim(glm::vec2 p,float w,float h,const char* tex_paths,uint8_t row,uint8_t col,
			uint8_t itn,uint8_t f);
	~InstancedAnim() {  }

	// setup
	void texture();
	void setup();

public:

	// vertex and index upload
	std::vector<float> v;
	std::vector<float> i;

	// texture
	GLuint tex;

private:

	// texture
	const char* tpath;
};