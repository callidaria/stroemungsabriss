#ifndef CCB_MATHEMATICS_TOOLBOX
#define CCB_MATHEMATICS_TOOLBOX

#include <iostream>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifdef __WIN32__
#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#else
#include <SOIL/SOIL.h>
#endif

class Toolbox
{
public:

	// loader
	static void load_object(const char* path,std::vector<float> &ov,glm::vec3 pos,
			float scl,glm::vec3 rot);

	// math helper
	static float calculate_vecangle(glm::vec2 a,glm::vec2 b);
	static void transform_vector(glm::vec3 &ov,glm::vec3 pos,float scl,glm::vec3 rot);
	static void rotate_vector(glm::vec3 &ov,glm::vec3 rot);

	// vertex setup
	static std::vector<float> create_sprite_canvas();
	static std::vector<float> create_sprite_canvas(glm::vec2 pos,float width,float height);
	static std::vector<float> create_sprite_canvas_triangled(glm::vec2 pos,float width,float height);

	// graphical setup
	static void load_texture(uint32_t tex,const char* path,bool corrected=false);
	static void load_texture(uint32_t tex,const char* path,float bias,bool corrected=false);
	static void load_texture_unfiltered(uint32_t tex,const char* path,bool corrected=false);
	static void load_texture_repeat(uint32_t tex,const char* path,bool corrected=false);
	static void generate_elements(uint16_t i,std::vector<unsigned int> &ls);

	// filter settings
	static void set_texture_parameter_linear_mipmap();
	static void set_texture_parameter_nearest_unfiltered();

	// pattern handling
	static void set_texture_parameter_clamp_to_edge();
	static void set_texture_parameter_clamp_to_border();
	static void set_texture_parameter_texture_repeat();

private:

	// helpers
	static void load_texture_function_head(uint32_t tex,const char* path,bool corrected);
};

#endif