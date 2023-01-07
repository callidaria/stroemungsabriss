#pragma once

#include <iostream>

#include <glm/glm.hpp>

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/gfx/shader.h"

#include "../../ccb/mat/camera3d.h"
#include "../../ccb/mat/toolbox.h"

class CardSystem
{
public:

	// construction
	CardSystem();
	~CardSystem() {  };

	// interaction
	void deal_card();

	// draw
	void render();

private:

	// cascabel
	Buffer bfr = Buffer();
	Shader sdr = Shader();
	Camera3D cam3D = Camera3D(glm::vec3(0,4,10),1280.0f,720.0f,90.0f);

	// texture
	GLuint tex;
};