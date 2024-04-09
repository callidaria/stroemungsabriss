#include "buffer.h"

/*
	Constructor()
	purpose: creates a regular buffer complex
*/
Buffer::Buffer()
{
	glGenVertexArrays(1,&vao);
	glGenBuffers(1,&vbo);
}

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
