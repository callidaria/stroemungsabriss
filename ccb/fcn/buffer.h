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
	~Buffer();

	// usage
	void add_buffer();	// create element array buffer additionally
    void bind();		// bind the vertex array object
	void bind_index();	// bind index or element buffer
	static void unbind();

	// upload
	void upload_vertices(float* vertices,size_t vsize);			// upload vertex array
	void upload_vertices(std::vector<float> iv);				// upload vertex list
	void upload_elements(unsigned int* elements,size_t esize);	// upload element array
	void upload_elements(std::vector<unsigned int> ie);			// upload element list
	void upload_indices(std::vector<float> is);					// upload indices array
	void upload_indices(float* is,size_t issize);			// upload vector indices array

	// getter
	GLuint get_indices();

private:

	GLuint vao,vbo,iebo;
};

#endif