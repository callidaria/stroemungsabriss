#include "shader.h"

/*
	compile(const char*,const char*) -> void
	!O(1) /load -> (public)
	purpose: compiles the given code files and creates a shader program for later usage
	\param vsp: path to vertex shader code file
	\param fsp: path to fragment shader code file
*/
void Shader::compile(const char* vsp,const char* fsp)
{
	// compiler
	uint32_t vertexShader = compile_shader(vsp,GL_VERTEX_SHADER);  // FIXME: delete after usage
	uint32_t fragmentShader = compile_shader(fsp,GL_FRAGMENT_SHADER);

	// shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram,vertexShader);
	glAttachShader(m_shaderProgram,fragmentShader);
	glBindFragDataLocation(m_shaderProgram,0,"outColour");
	glLinkProgram(m_shaderProgram);
	enable();
}

/*
	!O(1) /load -> (public)
	purpose: standard compile process with common attribute setup
		it is not necessary to compile first, these exist for most common shader structures
	\param vspath: path to vertex shader code file
	\param fspath: path to fragment shader code file
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
	def_attributeF("position",3,0,PATTERN_OBJECT_LOAD_REPEAT);
	def_attributeF("texCoords",2,3,PATTERN_OBJECT_LOAD_REPEAT);
	def_attributeF("normals",3,5,PATTERN_OBJECT_LOAD_REPEAT);
	def_attributeF("tangent",3,8,PATTERN_OBJECT_LOAD_REPEAT);
}

/*
	!O(1) /function -> (public)
	purpose: define input pattern from array for shader integer variables, that are not uniform
	\param vname: target variable name as referred to in shader code file
	\param dim: dimension of variable -> 1 => int, 2 => ivec2, 3 => ivec3, 4 => ivec4
	\param offset: offset in upload array pattern -> (int,int,INT,INT,int,int) => 2
	\param cap: capacity of upload array pattern. how many ints belong to a single vertex
*/
void Shader::def_attributeI(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(int32_t);
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribIPointer(attrib,dim,GL_INT,cap*vsize,(void*)(offset*vsize));
}

/*
	!O(1) /function -> (public)
	purpose: define input pattern from array for shader variables, that are not uniform
	\param vname: target variable name as referred to in shader code file
	\param dim: dimension of variable -> 1 => float, 2 => vec2, 3 => vec3, 4 => vec4
	\param offset: offset in upload array pattern -> (float,float,FLOAT,FLOAT,float,float) => 2
	\param cap: capacity of upload array pattern. how many floats belong to a single vertex
*/
void Shader::def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(float);
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
}
// TODO: identifying attributes like this is broken for anything not 32-bit and also out of fashion
// we shall implement a more dynamic method, to increase supported datatypes

/*
	!O(1) /function -> (public)
	purpose: define an irregular integer attribute pattern for non-uniform shader variables
	\param vsize: attribute size in vertex array pattern
	\param osize: offset in vertex array pattern
	NOTE: see previous attribute definitions for parameter descriptions
*/
void Shader::def_irregular_attributeI(const char* vname,uint8_t dim,size_t vsize,size_t osize)
{
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribIPointer(attrib,dim,GL_INT,vsize,(void*)osize);
}

/*
	!O(1) /function -> (public)
	purpose: define an irregular float attribute pattern for non-uniform shader variables
	NOTE: see previous attribute definitions for parameter descriptions
*/
void Shader::def_irregular_attributeF(const char* vname,uint8_t dim,size_t vsize,size_t osize)
{
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,vsize,(void*)osize);
}

/*
	def_indexF(unsigned int,const char*,uint8_t,uint8_t,uint8_t)
	vname, dim, offset, cap: same function as in def_attributeF
	purpose: define input pattern of index buffer object for shader variables (not uniform)
	NOTE: !!! ibo needs to be bound first !!!
*/
void Shader::def_indexF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(float);
	int32_t index = handle_index_location_by_name(vname);
	glVertexAttribPointer(index,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
	glVertexAttribDivisor(index,1);
}

/*
	!O(1) /+load -> (public)
	purpose: define float index variable
	\param vname: index variable name
	\param dim: upload dimension of variable
	\param isize: index upload pattern range until repeat
	\param offset: variable value offset within upload pattern range
*/
void Shader::def_irregular_indexF(const char* vname,uint8_t dim,size_t isize,size_t offset)
{
	int32_t index = handle_index_location_by_name(vname);
	glVertexAttribPointer(index,dim,GL_FLOAT,GL_FALSE,isize,(void*)(offset));
	glVertexAttribDivisor(index,1);
}

/*
	compile_shader(const char*,GLenum) -> uint32_t
	path: path to shader code file, meant to be compiled
	stype: type of shader to compile the shader code file as:
		vertex shader   -> GL_VERTEX_SHADER
		geometry shader -> GL_GEOMETRY_SHADER
		fragment shader -> GL_FRAGMENT_SHADER
	returns: compiled and debugged shader. if shader could not be compiled successfully
		a debug log will be posted into console at runtime.
*/
uint32_t Shader::compile_shader(const char* path,GLenum stype)
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
	uint32_t shader = glCreateShader(stype);
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

	// return compiled shader
	return shader;
}

/*
	!O(1) /function -> (private)
	purpose: converts uniform name to attribute location in shader
	\param name: variable name of uniform attrib
	\returns numerical representation of the attribute referenced by given variable name
*/
int32_t Shader::handle_attrib_location_by_name(const char* name)
{
	int32_t attrib = glGetAttribLocation(m_shaderProgram,name);
	glEnableVertexAttribArray(attrib);
	return attrib;
}

/*
	!O(1) /function -> (private)
	purpose: get attribute location in shader program by variable name
	\param name: variable name
	\returns shader attribute location
*/
int32_t Shader::handle_index_location_by_name(const char* name)
{
	int32_t index = glGetAttribLocation(m_shaderProgram,name);
	glEnableVertexAttribArray(index);
	return index;
}

/*
	upload_camera(Camera3D) -> void
	cam3d: camera to upload as uniform to shader program
	purpose: upload relevant camera matrices
*/
void Shader::upload_camera(Camera3D cam3d)
{
	upload_matrix("view",cam3d.view3D);
	upload_matrix("proj",cam3d.proj3D);
}
