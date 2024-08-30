#include "shader.h"


/**
 *	TODO
*/

/*
	constructor(const char*,GLenum)
	path: path to shader code file, meant to be compiled
	type: type of shader to compile the shader code file as:
		vertex shader   -> GL_VERTEX_SHADER
		geometry shader -> GL_GEOMETRY_SHADER
		fragment shader -> GL_FRAGMENT_SHADER
	TODO: purpose listing
*/
Shader::Shader(const char* path,GLenum type)
{
	COMM_AWT("compiling shader: %s",path);

	// get source file
	std::string src;
	std::ifstream file(path);
	if (!file.is_open())
	{
		COMM_ERR("no shader found at path: %s",path);
		return;
	}
	// FIXME: !file.is_open() can be replaced by !file. test this assumption

	// reads from source
	std::string line;
	while (!file.eof())
	{
		std::getline(file,line);
		src.append(line+"\n"); // ??standard && addable const char* is less performant bc stack
	}

	// close source file
	file.close();
	const char* source = src.c_str();

	// shader creation
	shader = glCreateShader(type);
	glShaderSource(shader,1,&source,NULL);
	glCompileShader(shader);

	// shader debug
	int stat;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&stat);
	if (!stat) {
		char log[512];
		glGetShaderInfoLog(shader,512,NULL,log);
		COMM_ERR("shader error -> %s: %s",path,log);
	}

	COMM_CNF();
}


/**
 *	TODO
*/

/*
	TODO
*/
void ShaderPipeline::assemble(const Shader& vs,const Shader& fs)
{
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram,vs.shader);
	glAttachShader(m_shaderProgram,fs.shader);
	glBindFragDataLocation(m_shaderProgram,0,"outColour");
	glLinkProgram(m_shaderProgram);
}

/*
	TODO
*/
void ShaderPipeline::point_buffer2D()
{
	def_attributeF("position",2,0,4);
	def_attributeF("texCoords",2,2,4);
}

/*
	TODO
*/
void ShaderPipeline::point_buffer3D()
{
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
void ShaderPipeline::def_attributeI(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
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
void ShaderPipeline::def_attributeF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
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
void ShaderPipeline::def_irregular_attributeI(const char* vname,uint8_t dim,size_t vsize,size_t osize)
{
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribIPointer(attrib,dim,GL_INT,vsize,(void*)osize);
}

/*
	!O(1) /function -> (public)
	purpose: define an irregular float attribute pattern for non-uniform shader variables
	NOTE: see previous attribute definitions for parameter descriptions
*/
void ShaderPipeline::def_irregular_attributeF(const char* vname,uint8_t dim,size_t vsize,size_t osize)
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
void ShaderPipeline::def_indexF(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
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
void ShaderPipeline::def_irregular_indexF(const char* vname,uint8_t dim,size_t isize,size_t offset)
{
	int32_t index = handle_index_location_by_name(vname);
	glVertexAttribPointer(index,dim,GL_FLOAT,GL_FALSE,isize,(void*)(offset));
	glVertexAttribDivisor(index,1);
}

/*
	!O(1) /function -> (private)
	purpose: converts uniform name to attribute location in shader
	\param name: variable name of uniform attrib
	\returns numerical representation of the attribute referenced by given variable name
*/
int32_t ShaderPipeline::handle_attrib_location_by_name(const char* name)
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
int32_t ShaderPipeline::handle_index_location_by_name(const char* name)
{
	int32_t index = glGetAttribLocation(m_shaderProgram,name);
	glEnableVertexAttribArray(index);
	return index;
}
