#ifndef CCB_GRAPHICS_SHADER
#define CCB_GRAPHICS_SHADER

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/gtc/type_ptr.hpp>

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"
#include "../mat/camera3d.h"

class Shader
{
public:

	// construction
	Shader() {  }
	~Shader() {  }

	// compilation
	void compile(const char* vspath, const char* fspath);
	void compile2d(const char* vspath, const char* fspath);
	void compile3d(const char* vspath,const char* fspath);

	// definition
	void def_attributeI(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_indexF(uint32_t ibo,const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);

	// setup
	inline void enable() { glUseProgram(m_shaderProgram); }
	static inline void disable() { glUseProgram(0); }

	// upload
	inline void upload_int(const char* loc,int i)
		{ glUniform1i(glGetUniformLocation(m_shaderProgram,loc),i); }
	inline void upload_float(const char* loc,float f)
		{ glUniform1f(glGetUniformLocation(m_shaderProgram,loc),f); }
	inline void upload_vec2(const char* loc,glm::vec2 v)
		{ glUniform2f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y); }
	inline void upload_vec3(const char* loc,glm::vec3 v)
		{ glUniform3f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z); }
	inline void upload_vec4(const char* loc,glm::vec4 v)
		{ glUniform4f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z,v.w); }
	inline void upload_matrix(const char* loc,glm::mat4 m)
	{ glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram,loc),1,GL_FALSE,glm::value_ptr(m)); }
	void upload_camera(Camera2D cam2d);
	void upload_camera(Camera3D cam3d);

private:

	// helpers
	uint32_t compile_shader(const char* path,GLenum stype);
	int32_t handle_attrib_location_by_name(const char* name);

private:

	uint32_t m_shaderProgram;
};

#endif