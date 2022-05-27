#pragma once

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

class Buffer
{
public:
	Buffer();
	~Buffer();

	// common
	void add_buffer();	// create element array buffer additionally
    void bind();		// bind the vertex array object

	// upload
	void upload_vertices(float* vertices,size_t vsize);			// upload vertex array
	void upload_vertices(std::vector<float> iv);				// upload vertex list
	void upload_elements(unsigned int* elements,size_t esize);	// upload element array
	void upload_indices(std::vector<float> is);					// upload indices array
	void upload_indices(glm::vec2* is,size_t issize);			// upload vector indices array

	// return
	unsigned int get_indices();
private:
	unsigned int vao,vbo,iebo;
};