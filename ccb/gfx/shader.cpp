#include "shader.h"

/*
	Constructor()
	DEPRECATED: make a constructor, maybe even autocompile at creation?
*/
Shader::Shader() {  }

/*
	compile(const char*,const char*) -> void
	vsp: path to vertex shader code file
	fsp: path to fragment shader code file
	purpose: compiles the given code files and creates a shader program for later usage
*/
void Shader::compile(const char* vsp,const char* fsp)
{
	// compiler
	unsigned int vertexShader = compile_shader(vsp,GL_VERTEX_SHADER);  // FIXME: delete after usage
	unsigned int fragmentShader = compile_shader(fsp,GL_FRAGMENT_SHADER);

	// shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram,vertexShader);
	glAttachShader(m_shaderProgram,fragmentShader);
	glBindFragDataLocation(m_shaderProgram,0,"outColour");
	glLinkProgram(m_shaderProgram);
	enable();
}

/*
	compile<dimension>(const char*,const char*) -> void
	vspath: path to vertex shader code file
	fspath: path to fragment shader code file
	purpose: standard compile process with common attribute setup
		it is not necessary to compile first, these exist for most common shader structures
*/
void Shader::compile2d(const char* vspath,const char* fspath)
{
	compile(vspath,fspath);
	def_attributeF("position",2,0,4);
	def_attributeF("texCoords",2,2,4);
}

void Shader::compile3d(const char* vspath,const char* fspath)
{
	compile(vspath,fspath);
	def_attributeF("position",3,0,14);
	def_attributeF("texCoords",2,3,14);
	def_attributeF("normals",3,5,14);
	def_attributeF("tangent",3,8,14);
	def_attributeF("bitangent",3,11,14);
}

/*
	def_attributeF(const char*,uint8_t,uint8_t,uint8_t) -> void
	vname: target variable name as referred to in shader code file
	dim: dimension of variable -> 1 => float, 2 => vec2, 3 => vec3, 4 => vec4
	offset: offset in upload array pattern -> (float,float,FLOAT,FLOAT,float,float) => 2
	cap: capacity of upload array pattern. how many floats belong to a single vertex
	purpose: define input pattern from array for shader variables, that are not uniform
*/
void Shader::def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(float);
	int attrib = glGetAttribLocation(m_shaderProgram,vname);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
}

/*
	def_indexF(unsigned int,const char*,uint8_t,uint8_t,uint8_t)
	ibo: index buffer object to be used by shader and defined by layout pattern
	vname, dim, offset, cap: same function as in def_attributeF
	purpose: define input pattern of index buffer object for shader variables (not uniform)
		!!! ibo needs to be bound first !!!
*/
void Shader::def_indexF(unsigned int ibo,const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(float);
	int attrib = glGetAttribLocation(m_shaderProgram,vname);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
	glVertexAttribDivisor(attrib,1);
}
// FIXME: unsigned int usage

/*
	upload_<X>(const char*,X x) -> void
	loc: name of uniform variable as referred to in the shader code file
	x: variable in desired datatype to upload to the shader as variable defined by location
	purpose: definition of uniform variables in shader by program
*/
void Shader::upload_int(const char* loc,int i) { glUniform1i(glGetUniformLocation(m_shaderProgram,loc),i); }
// FIXME: int usage

void Shader::upload_float(const char* loc,float f) { glUniform1f(glGetUniformLocation(m_shaderProgram,loc),f); }

void Shader::upload_vec2(const char* loc,glm::vec2 v)
{ glUniform2f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y); }

void Shader::upload_vec3(const char* loc,glm::vec3 v)
{ glUniform3f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z); }

void Shader::upload_vec4(const char* loc,glm::vec4 v)
{ glUniform4f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z,v.w); }

void Shader::upload_matrix(const char* loc,glm::mat4 m)
{ glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram,loc),1,GL_FALSE,glm::value_ptr(m)); }

/*
	upload_camera(Camera2D||Camera3D) -> void
	cam2d||cam3d: camera to upload as uniform to shader program
	purpose: upload relevant camera matrices
*/
void Shader::upload_camera(Camera2D cam2d)
{
	upload_matrix("view",cam2d.view2D);
	upload_matrix("proj",cam2d.proj2D);
}

void Shader::upload_camera(Camera3D cam3d)
{
	upload_matrix("view",cam3d.view3D);
	upload_matrix("proj",cam3d.proj3D);
}

/*
	enable() -> void
	purpose: enables the program, so that it can be used. deactivates all others!
*/
void Shader::enable() { glUseProgram(m_shaderProgram); }

/*
	disable() -> void
	purpose: disables any shader program that might still be enabled
*/
void Shader::disable()
{ glUseProgram(0); }

/*
	compile_shader(const char*,GLenum) -> unsigned int
	path: path to shader code file, meant to be compiled
	stype: type of shader to compile the shader code file as:
		vertex shader   -> GL_VERTEX_SHADER
		geometry shader -> GL_GEOMETRY_SHADER
		fragment shader -> GL_FRAGMENT_SHADER
	returns: compiled and debugged shader. if shader could not be compiled successfully
		a debug log will be posted into console at runtime.
*/
unsigned int Shader::compile_shader(const char* path,GLenum stype)
{
	// get source file
	std::string src; // FIXME: delete after usage
	std::ifstream file(path);
	if (!file.is_open()) {
		printf("\033[1;31mno shader found at path: %s\033[0m\n",path);
		return 0;
	}

	// reads from source
	std::string line;
	while (!file.eof()) {
		std::getline(file,line);
		src.append(line+"\n"); // ??standard && addable const char* is less performant bc stack
	} file.close();
	const char* source = src.c_str();

	// shader creation
	unsigned int shader = glCreateShader(stype);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);

	// shader debug
	int stat;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&stat);
	if (!stat) {
		char log[512];
		glGetShaderInfoLog(shader,512,NULL,log);
		printf("\033[1;31mshader error at: %s\nerror:\033[033[36m%s\033[0m\n",path,log);
	}

	return shader;
}

/*
	upload_<X>(const char*,X x) -> void
	loc: name of uniform variable as referred to in the shader code file
	x: variable in desired datatype to upload to the shader as variable defined by location
	purpose: definition of uniform variables in shader by program
*/
void Shader::upload_int(const char* loc,int i)
{ glUniform1i(glGetUniformLocation(m_shaderProgram,loc),i); }
void Shader::upload_float(const char* loc,float f)
{ glUniform1f(glGetUniformLocation(m_shaderProgram,loc),f); }
void Shader::upload_vec2(const char* loc,glm::vec2 v)
{ glUniform2f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y); }
void Shader::upload_vec3(const char* loc,glm::vec3 v)
{ glUniform3f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z); }
void Shader::upload_vec4(const char* loc,glm::vec4 v)
{ glUniform4f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z,v.w); }
void Shader::upload_matrix(const char* loc,glm::mat4 m)
{ glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram,loc),1,GL_FALSE,glm::value_ptr(m)); }

/*
	upload_camera(Camera2D||Camera3D) -> void
	cam2d||cam3d: camera to upload as uniform to shader program
	purpose: upload relevant camera matrices
*/
void Shader::upload_camera(Camera2D cam2d)
{
	upload_matrix("view",cam2d.view2D);
	upload_matrix("proj",cam2d.proj2D);
}
void Shader::upload_camera(Camera3D cam3d)
{
	upload_matrix("view",cam3d.view3D);
	upload_matrix("proj",cam3d.proj3D);
}