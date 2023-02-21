#pragma once

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

#include "../mat/camera2d.h"
#include "../mat/camera3d.h"

class Shader
{
public:

	// construction
	Shader();  // DEPRECATED

	// setup
	void compile(const char* vspath, const char* fspath);
	void def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_indexF(unsigned int ibo,const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);

	// standard compilations
	void compile2d(const char* vspath, const char* fspath);
	void compile3d(const char* vspath,const char* fspath);

	// settings
	void enable();
	static void disable();

private:

	// helpers
	unsigned int compile_shader(const char* path,GLenum stype);

public:

	// upload
	void upload_int(const char* loc,int i);
	void upload_float(const char* loc,float f);
	void upload_vec2(const char* loc,glm::vec2 v);
	void upload_vec3(const char* loc,glm::vec3 v);
	void upload_vec4(const char* loc,glm::vec4 v);
	void upload_matrix(const char* loc,glm::mat4 m);
	void upload_camera(Camera2D cam2d);
	void upload_camera(Camera3D cam3d);

private:

	unsigned int m_shaderProgram;
};