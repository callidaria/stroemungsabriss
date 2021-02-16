#pragma once

#include <iostream>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <SOIL/SOIL.h>

class Instance
{
public:
	Instance(glm::vec2 p, float w, float h, const char* t)
		: tp(t)
	{
		v[0] = p.x+w; v[1] = p.y+h; v[2] = 1.0f; v[3] = 0.0f;
		v[4] = p.x; v[5] = p.y; v[6] = 0.0f; v[7] = 1.0f;
		v[8] = p.x+w; v[9] = p.y; v[10] = 1.0f; v[11] = 1.0f;

		v[12] = p.x+w; v[13] = p.y+h; v[14] = 1.0f; v[15] = 0.0f;
		v[16] = p.x; v[17] = p.y; v[18] = 0.0; v[19] = 1.0f;
		v[20] = p.x; v[21] = p.y+h; v[22] = 0.0f; v[23] = 0.0f;

		glGenTextures(1, &tex);
	}
	void texture()
	{
		glBindTexture(GL_TEXTURE_2D, tex);
		int width, height;
		unsigned char* image =
			SOIL_load_image(tp, &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0,
			GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER,
				GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER,
				GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void setup() { glBindTexture(GL_TEXTURE_2D, tex); }
public:
	const char* tp;
	unsigned int tex;
	float v[24];
	glm::vec2 o[100];
};
