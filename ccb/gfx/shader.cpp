#include "shader.h"

/*
	compile(const char*,const char*) -> void
	vsp: path to vertex shader code file
	fsp: path to fragment shader code file
	purpose: compiles the given code files and creates a shader program for later usage
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
	def_attributeF("position",3,0,TOOLBOX_OBJECT_LOAD_REPEAT);
	def_attributeF("texCoords",2,3,TOOLBOX_OBJECT_LOAD_REPEAT);
	def_attributeF("normals",3,5,TOOLBOX_OBJECT_LOAD_REPEAT);
	def_attributeF("tangent",3,8,TOOLBOX_OBJECT_LOAD_REPEAT);
}

/*
	TODO
*/
void Shader::def_attributeI(const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(int32_t);
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribIPointer(attrib,dim,GL_INT,cap*vsize,(void*)(offset*vsize));
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
	int32_t attrib = handle_attrib_location_by_name(vname);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
}

/*
	def_indexF(unsigned int,const char*,uint8_t,uint8_t,uint8_t)
	ibo: index buffer object to be used by shader and defined by layout pattern
	vname, dim, offset, cap: same function as in def_attributeF
	purpose: define input pattern of index buffer object for shader variables (not uniform)
		!!! ibo needs to be bound first !!!
*/
void Shader::def_indexF(uint32_t ibo,const char* vname,uint8_t dim,uint8_t offset,uint8_t cap)
{
	size_t vsize = sizeof(float);
	int attrib = glGetAttribLocation(m_shaderProgram,vname);
	glEnableVertexAttribArray(attrib);
	glVertexAttribPointer(attrib,dim,GL_FLOAT,GL_FALSE,cap*vsize,(void*)(offset*vsize));
	glVertexAttribDivisor(attrib,1);
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
	TODO
*/
int32_t Shader::handle_attrib_location_by_name(const char* name)
{
	int attrib = glGetAttribLocation(m_shaderProgram,name);
	glEnableVertexAttribArray(attrib);
	return attrib;
}
// FIXME: find out if the return variable is indeed signed

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
