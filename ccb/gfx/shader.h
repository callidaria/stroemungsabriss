#ifndef CCB_GRAPHICS_SHADER
#define CCB_GRAPHICS_SHADER

#include "../mat/math.h"

class Shader
{
public:

	// construction
	Shader(const char* path,GLenum type);
	~Shader() {  }

public:

	// data
	uint32_t shader;
};


class ShaderPipeline
{
public:

	// construction
	ShaderPipeline() {  }
	~ShaderPipeline() {  }

	// setup
	void assemble(const Shader& vs,const Shader& fs);

	// environments
	void point_buffer2D();
	void point_buffer3D();

	// definition
	void def_attributeI(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_irregular_attributeI(const char* vname,uint8_t dim,size_t vsize,size_t osize);
	void def_irregular_attributeF(const char* vname,uint8_t dim,size_t vsize,size_t osize);
	void def_indexF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap);
	void def_irregular_indexF(const char* vname,uint8_t dim,size_t isize,size_t offset);

	// setup
	inline void enable() { glUseProgram(m_shaderProgram); }
	static inline void disable() { glUseProgram(0); }

	// upload
	inline void upload_int(const char* loc,uint32_t i)
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
	inline void upload_camera()
		{ upload_matrix("view",g_Camera2D.view2D), upload_matrix("proj",g_Camera2D.proj2D); }
	inline void upload_camera(Camera3D& cam3d)
		{ upload_matrix("view",cam3d.view3D), upload_matrix("proj",cam3d.proj3D); }

private:

	// helpers
	int32_t handle_attrib_location_by_name(const char* name);
	int32_t handle_index_location_by_name(const char* name);

private:

	uint32_t m_shaderProgram;
};

#endif
