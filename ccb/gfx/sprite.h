#ifndef CCB_GRAPHICS_SPRITE
#define CCB_GRAPHICS_SPRITE

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "../mat/toolbox.h"

class Sprite
{
public:

	// construction
	Sprite(glm::vec2 p,float w,float h,const char* t);
	~Sprite() {  }

	// setup
	inline void texture() { Toolbox::load_texture(tex,texpath); }
	inline void setup() { glBindTexture(GL_TEXTURE_2D,tex); }

	// transformation
	void transform(glm::vec2 tp,glm::vec2 ts,float tr);
	void transform(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a);
	inline void translate(glm::vec2 tp) { model = glm::translate(model,glm::vec3(tp.x,tp.y,0)); }
	void scale(float wscale,float hscale);
	void scale_arbit(float wscale,float hscale);
	inline void rotate(float rot) { model = glm::rotate(model,glm::radians(rot),glm::vec3(0,0,1)); }

public:

	// system
	std::vector<float> v;
	GLuint tex;

	// position
	glm::mat4 model = glm::mat4(1.0f);
	glm::vec2 pos;
	float sclx,scly;

private:

	const char* texpath;
};

#endif