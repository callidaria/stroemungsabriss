#include "buffer.h"

/*
	Constructor()
	purpose: creates a regular buffer complex
*/
Buffer::Buffer()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
} Buffer::~Buffer() {  }

/*
	add_buffer() -> void
	purpose: creating optional buffer if used by any feature
*/
void Buffer::add_buffer()
{ glGenBuffers(1,&iebo); }

/*
	bind() -> void
	purpose: binds the vertex array before uploading and rendering
*/
void Buffer::bind()
{ glBindVertexArray(vao); }

/*
	bind_index() -> void
	purpose: binds the index or the element buffer before usage
*/
void Buffer::bind_index()
{ glBindBuffer(GL_ARRAY_BUFFER,iebo); }

/*
	unbind() -> void (static)
	purpose: unbind any bound array buffer
*/
void Buffer::unbind()
{
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
}

/*
	upload_vertices(float*,size_t) -> void
	vertices: array of vertices to upload
	vsize: size of array to upload (by sizeof return)
	purpose: upload vertices to array buffer for later usage in graphical processes
*/
void Buffer::upload_vertices(float* vertices,size_t vsize)
{
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,vsize,vertices,GL_STATIC_DRAW);
}

/*
	upload_vertices(std::vector<float>) -> void
	iv: list of vertices to upload
	purpose: overload previous to support std::vector format
*/
void Buffer::upload_vertices(std::vector<float> iv)
{
	glBindBuffer(GL_ARRAY_BUFFER,vbo);
	glBufferData(GL_ARRAY_BUFFER,iv.size()*sizeof(float),&iv[0],GL_STATIC_DRAW);
}

/*
	upload_elements(unsigned int*,size_t) -> void
	elements: array of optional element indices, mapping the vertex order
	esize: size of array, holding the element indices (by sizeof return)
	purpose: upload vertex order element indices
*/
void Buffer::upload_elements(unsigned int* elements,size_t esize)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,esize,elements,GL_STATIC_DRAW);
}

/*
	upload_elements(std::vector<uint16_t>)
	ie: list of elements to upload
	purpose: overload previous to support std::vector format
*/
void Buffer::upload_elements(std::vector<unsigned int> ie)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,iebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,ie.size()*sizeof(unsigned int),&ie[0],GL_STATIC_DRAW);
}

/*
	upload_indices(std::vector<float>) -> void
	is: list of dynamic indices to upload additionally to static array buffer
	purpose: upload index array for later usage in graphical processes
*/
void Buffer::upload_indices(std::vector<float> is)
{
	glBindBuffer(GL_ARRAY_BUFFER,iebo);
	glBufferData(GL_ARRAY_BUFFER,is.size()*sizeof(float),&is[0],GL_DYNAMIC_DRAW);
}

/*
	upload_indices(std::vector<glm::vec2> is) -> void
	is: array of dynamic indices, to upload additionally to static array buffer
	issize: size of array to upload
	purpose: overload previous to work with glm::vec2 arrays
*/
void Buffer::upload_indices(float* is,size_t issize)
{
	glBindBuffer(GL_ARRAY_BUFFER,iebo);
	glBufferData(GL_ARRAY_BUFFER,issize,is,GL_DYNAMIC_DRAW);
}

/*
	get_indices() -> GLuint
	returns: index or element array buffer
*/
GLuint Buffer::get_indices()
{
	return iebo;
}