#pragma once

#include <iostream>
#include <vector>

#include "../mat/toolbox.h"

#include "shader.h"

constexpr uint32_t IANIMATION_MCAP = 4096;
constexpr uint32_t IANIMATION_REPEAT = 4;
constexpr uint32_t IANIMATION_VALUES = IANIMATION_MCAP*IANIMATION_REPEAT;
// FIXME: all of this always has to be equal to their counterparts in instance.h! remove & replace

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
	float i[IANIMATION_VALUES] = { 0,0,0,1 };

	// texture
	GLuint tex;

private:

	// texture
	const char* tpath;

	// uniform variables
	uint8_t row,col;
};