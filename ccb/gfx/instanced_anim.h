#pragma once

#include <iostream>
#include <vector>

#include "../mat/toolbox.h"

#include "shader.h"

constexpr uint32_t IANIMATION_MCAP = 8192;

class InstancedAnim
{
public:

	// construction
	InstancedAnim(glm::vec2 p,float w,float h,const char* tex_paths,uint8_t irow,uint8_t icol,
			uint8_t itn,uint8_t f);
	~InstancedAnim() {  }

	// setup
	void texture();
	void setup(Shader* shader);

public:

	// vertex and index upload
	std::vector<float> v;
	float i[IANIMATION_MCAP] = { 0 };

	// texture
	GLuint tex;

private:

	// texture
	const char* tpath;

	// uniform variables
	uint8_t row,col;
};