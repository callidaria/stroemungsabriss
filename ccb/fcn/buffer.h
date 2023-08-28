#ifndef CCB_FEATURE_BUFFER
#define CCB_FEATURE_BUFFER

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>

class Buffer
{

public:

	// construction
	Buffer();
	~Buffer() {  }

	// usage
	void add_buffer() { glGenBuffers(1,&iebo); }
    void bind() { glBindVertexArray(vao); }
	void bind_index() { glBindBuffer(GL_ARRAY_BUFFER,iebo); }
	static void unbind();

	// upload
	void upload_vertices(float* vertices,size_t vsize);
	void upload_vertices(std::vector<float> iv);
	void upload_elements(unsigned int* elements,size_t esize);
	void upload_elements(std::vector<unsigned int> ie);
	void upload_indices(std::vector<float> is);
	void upload_indices(float* is,size_t issize);

public:

	uint32_t vao,vbo,iebo;
};

#endif