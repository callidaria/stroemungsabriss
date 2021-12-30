#include "../gfx/shader.h"

Shader::Shader() {  } // ??why this again seriously i was this stupid 3 years ago
void Shader::compile(const char* vsp,const char* fsp)
{
	// compiler
	unsigned int vertexShader = compile_shader(vsp,GL_VERTEX_SHADER); // !!delete after usage
	unsigned int fragmentShader = compile_shader(fsp,GL_FRAGMENT_SHADER);

	// shader program
	m_shaderProgram = glCreateProgram();
	glAttachShader(m_shaderProgram,vertexShader);
	glAttachShader(m_shaderProgram,fragmentShader);
	glBindFragDataLocation(m_shaderProgram,0,"outColour"); // !!make char pointer parameter for outColour
	glLinkProgram(m_shaderProgram);
	enable();
}
void Shader::compile2d(const char* vspath,const char* fspath) // !!cleanup and launch with enumerator && casediff
{
	compile(vspath,fspath);

	// enabling maybe obsolete cause of limited upload requests
	int posAttrib = glGetAttribLocation(m_shaderProgram,"position"); // ??outsource to usage class
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,4*sizeof(float),0); // ??reduction to one upload bitshift
	int texAttrib = glGetAttribLocation(m_shaderProgram,"texCoords");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
}
void Shader::compile3d(const char* vspath,const char* fspath) // !!another cleanup
{
	compile(vspath,fspath);

	// ??obsolete
	int posAttrib = glGetAttribLocation(m_shaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib,3,GL_FLOAT,GL_FALSE,14*sizeof(float),0);
	int texAttrib = glGetAttribLocation(m_shaderProgram,"texCoords");
	glEnableVertexAttribArray(texAttrib);
	glVertexAttribPointer(texAttrib,2,GL_FLOAT,GL_FALSE,14*sizeof(float),(void*)(3*sizeof(float)));
	int nmlAttrib = glGetAttribLocation(m_shaderProgram,"normals");
	glEnableVertexAttribArray(nmlAttrib);
	glVertexAttribPointer(nmlAttrib,3,GL_FLOAT,GL_FALSE,14*sizeof(float),(void*)(5*sizeof(float)));
	int tgAttrib = glGetAttribLocation(m_shaderProgram,"tangent");
	glEnableVertexAttribArray(tgAttrib);
	glVertexAttribPointer(tgAttrib,3,GL_FLOAT,GL_FALSE,14*sizeof(float),(void*)(8*sizeof(float)));
	int btgAttrib = glGetAttribLocation(m_shaderProgram,"bitangent");
	glEnableVertexAttribArray(btgAttrib);
	glVertexAttribPointer(btgAttrib,3,GL_FLOAT,GL_FALSE,14*sizeof(float),(void*)(11*sizeof(float)));
}
void Shader::compile_vCols(const char* vspath,const char* fspath)
{
	compile(vspath,fspath);

	// !!reduce recurring code chunks
	int posAttrib = glGetAttribLocation(m_shaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
	int dposAttrib = glGetAttribLocation(m_shaderProgram,"dposition");
	glEnableVertexAttribArray(dposAttrib);
	glVertexAttribPointer(dposAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(2*sizeof(float)));
	int colAttrib = glGetAttribLocation(m_shaderProgram,"colour");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(4*sizeof(float)));
	int idxAttrib = glGetAttribLocation(m_shaderProgram,"idx");
	glEnableVertexAttribArray(idxAttrib);
	glVertexAttribPointer(idxAttrib,1,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(7*sizeof(float)));
	// !!reduce to int
}
void Shader::compile_hp(const char* vspath,const char* fspath)
{
	compile(vspath,fspath);
	int posAttrib = glGetAttribLocation(m_shaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,4*sizeof(float),0);
	int colAttrib = glGetAttribLocation(m_shaderProgram,"bar_id");
	glEnableVertexAttribArray(colAttrib);
	glVertexAttribPointer(colAttrib,1,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(2*sizeof(float)));
	int edgeAttrib = glGetAttribLocation(m_shaderProgram,"edge_id");
	glEnableVertexAttribArray(edgeAttrib);
	glVertexAttribPointer(edgeAttrib,1,GL_FLOAT,GL_FALSE,4*sizeof(float),(void*)(3*sizeof(float)));
}
void Shader::compile_mDlg(const char* vspath,const char* fspath)
{
	compile(vspath,fspath);
	int posAttrib = glGetAttribLocation(m_shaderProgram,"position");
	glEnableVertexAttribArray(posAttrib);
	glVertexAttribPointer(posAttrib,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);
}
void Shader::load_index(unsigned int ibo) // !!index upload checking && double upload nessessary
{
	int offsetAttrib = glGetAttribLocation(m_shaderProgram,"offset");
	glEnableVertexAttribArray(offsetAttrib);
	glBindBuffer(GL_ARRAY_BUFFER,ibo);
	glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);
	glVertexAttribDivisor(offsetAttrib,1);
}
void Shader::load_text(unsigned int ibo) // !!check for stupid shit
{
	int offsetAttrib=glGetAttribLocation(m_shaderProgram,"offset");
	glEnableVertexAttribArray(offsetAttrib);
	glBindBuffer(GL_ARRAY_BUFFER,ibo);
	glVertexAttribPointer(offsetAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
	glVertexAttribDivisor(offsetAttrib,1);
	int texposAttrib=glGetAttribLocation(m_shaderProgram,"texpos");
	glEnableVertexAttribArray(texposAttrib);
	glVertexAttribPointer(texposAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(2*sizeof(float)));
	glVertexAttribDivisor(texposAttrib,1);
	int boundsAttrib=glGetAttribLocation(m_shaderProgram,"bounds");
	glEnableVertexAttribArray(boundsAttrib);
	glVertexAttribPointer(boundsAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(4*sizeof(float)));
	glVertexAttribDivisor(boundsAttrib,1);
	int cursorAttrib=glGetAttribLocation(m_shaderProgram,"cursor");
	glEnableVertexAttribArray(cursorAttrib);
	glVertexAttribPointer(cursorAttrib,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(6*sizeof(float)));
	glVertexAttribDivisor(cursorAttrib,1);
}
void Shader::enable() { glUseProgram(m_shaderProgram); }
void Shader::enable_location(const char* loc)
{
	int attrib = glGetAttribLocation(m_shaderProgram,loc);
	glEnableVertexAttribArray(attrib);
}
unsigned int Shader::compile_shader(const char* path,GLenum stype) // ??int or enum !!unify with frame class
{
	// reads from source
	std::string src; // !!delete after usage
	std::ifstream file(path);
	if (!file.is_open()) {
		printf("\033[1;31mno shader found at path: %s\033[0m\n",path);
		return 0;
	} std::string line = ""; // ??initialization madness
	while (!file.eof()) {
		std::getline(file,line); // !!unify string/char reading c/c++
		src.append(line+"\n"); // ??standard && addable const char* is less performant bc stack
	} file.close();
	const char* source = src.c_str(); // ??conversion nessessary after read revolution or .usagerev

	// shader creation
	unsigned int shader = glCreateShader(stype);
	glShaderSource(shader,1,&source,NULL); // !!change source usage
	glCompileShader(shader);

	// shader debug
	int stat;
	glGetShaderiv(shader,GL_COMPILE_STATUS,&stat);
	if (stat!=GL_TRUE) { // ??GL_TRUE macro synonymous with boolean value expression
		char log[512];
		glGetShaderInfoLog(shader,512,NULL,log);
		printf("\033[1;31mshader error at: %s\nerror:\033[033[36m%s\033[0m\n",path,log);
	} return shader; // ??add comfirmation output
}
// !!not to be a hater but why not outsource and make shader program gettable from outside
void Shader::upload_int(const char* loc,int i) { glUniform1i(glGetUniformLocation(m_shaderProgram,loc),i); }
void Shader::upload_float(const char* loc,float f) { glUniform1f(glGetUniformLocation(m_shaderProgram,loc),f); }
void Shader::upload_vec2(const char* loc,glm::vec2 v)
{
	glUniform2f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y);
}
void Shader::upload_vec3(const char* loc,glm::vec3 v)
{
	glUniform3f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z);
}
void Shader::upload_vec4(const char* loc,glm::vec4 v)
{
	glUniform4f(glGetUniformLocation(m_shaderProgram,loc),v.x,v.y,v.z,v.w);
}
void Shader::upload_matrix(const char* loc,glm::mat4 m)
{
	glUniformMatrix4fv(glGetUniformLocation(m_shaderProgram,loc),1,GL_FALSE,glm::value_ptr(m));
}
