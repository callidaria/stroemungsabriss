#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <SOIL/SOIL.h>

class Sprite
{
public:
	Sprite(glm::vec2 p, float w, float h, const char* t)
		: pos(p),sclx(w),scly(h),texpath(t)
	{
		v[0] = p.x; v[1] = p.y+h; v[2] = 0.0f; v[3] = 0.0f;
		v[4] = p.x+w; v[5] = p.y+h; v[6] = 1.0f; v[7] = 0.0f;
		v[8] = p.x+w; v[9] = p.y; v[10] = 1.0f; v[11] = 1.0f;
		v[12] = p.x; v[13] = p.y; v[14] = 0.0; v[15] = 1.0f;
		glGenTextures(1, &tex);
	}
	void texture()
	{
		glBindTexture(GL_TEXTURE_2D, tex);
		int width, height;
		unsigned char* image=SOIL_load_image(texpath,&width,&height,
				0,SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGBA,width,height,0,GL_RGBA,
				GL_UNSIGNED_BYTE,image);
		SOIL_free_image_data(image);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,
				GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,
				GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	void setup() { glBindTexture(GL_TEXTURE_2D, tex); }
	glm::mat4 transform(glm::vec2 tp,glm::vec2 ts,float tr)
	{
		glm::mat4 trans=glm::translate(glm::mat4(1.0f),
				glm::vec3(pos.x+sclx/2+tp.x,pos.y+scly/2+tp.y,0));
		trans=glm::rotate(trans,glm::radians(tr),glm::vec3(0,0,1));
		trans=glm::scale(trans,glm::vec3(ts.x,ts.y,1));
		trans=glm::translate(trans,
				glm::vec3(-(pos.x+sclx/2),-(pos.y+scly/2),0));
		return trans;
	}
	glm::mat4 transform_arbit(glm::vec2 tp,glm::vec2 ts,float tr,glm::vec2 a)
	{
		float ax=a.x-(pos.x+sclx/2);float ay=a.y-(pos.y+scly/2);
		glm::mat4 trans=glm::translate(glm::mat4(1.0f),glm::vec3(
					pos.x+sclx/2+tp.x+ax,
					pos.y+scly/2+tp.y+ay,0));
		trans=glm::rotate(trans,glm::radians(tr),glm::vec3(0,0,1));
		trans=glm::scale(trans,glm::vec3(ts.x,ts.y,1));
		trans=glm::translate(trans,glm::vec3(
					-(pos.x+sclx/2+ax),
					-(pos.y+scly/2+ay),0));
		return trans;
	}
private:
	glm::vec2 pos;float sclx,scly;
	const char* texpath;
public:
	float v[16];
	unsigned int tex;
};
