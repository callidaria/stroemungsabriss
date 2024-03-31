#ifndef CCB_MATHEMATICS_TOOLBOX
#define CCB_MATHEMATICS_TOOLBOX

#include <iostream>
#include <chrono>
#include <vector>

#include <GL/glew.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include <glm/gtx/rotate_vector.hpp>

#ifndef STBI_INCLUDE_STB_IMAGE_H
#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_STATIC
#include "../../include/stb_image.h"
#endif

#ifndef INCLUDE_STB_IMAGE_WRITE_H
#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#include "../../include/stb_image_write.h"
#endif

// math constants
constexpr double MATH_CARTESIAN_XRANGE = 1280.f;
constexpr double MATH_CARTESIAN_YRANGE = 720.f;
constexpr double MATH_PI = 3.141592653;
constexpr double MATH_CENTER_GOLDEN = MATH_CARTESIAN_XRANGE*.618;

// conversion
constexpr uint32_t CONVERSION_THRES_MILLISECONDS = 1000;
constexpr uint32_t CONVERSION_THRES_MICROSECONDS = 1000000;
constexpr uint32_t CONVERSION_THRES_NANOSECONDS = 1000000000;
constexpr double CONVERSION_MULT_MICROSECONDS = .001;
constexpr double CONVERSION_MULT_MILLISECONDS = .000001;
constexpr double CONVERSION_MULT_SECONDS = .000000001;

// pattern
constexpr uint8_t TOOLBOX_SPRITE_LOAD_REPEAT = 4;
constexpr uint8_t TOOLBOX_SPRITE_ELEMENT_REPEAT = 6;
constexpr uint8_t TOOLBOX_SPRITE_VERTEX_REPEAT = 4*TOOLBOX_SPRITE_LOAD_REPEAT;
constexpr uint8_t TOOLBOX_SPRITE_TRIANGLE_REPEAT = 6*TOOLBOX_SPRITE_LOAD_REPEAT;
constexpr uint8_t TOOLBOX_OBJECT_LOAD_REPEAT = 11;


// debug timing keys to record individual loadtimes for task sequences
struct DebugLogKey
{
	const char* key_name;
	double delta_ticks;
};

// structure to hold all timing debug data for a single method
struct DebugLogData
{
	const char* task_name;
	std::vector<DebugLogKey> key_list;
	std::chrono::steady_clock::time_point last_ticks;
};

class Toolbox
{
public:

	// loader
	static uint32_t load_object(const char* path,std::vector<float>& ov,glm::vec3 pos,
			float scl,glm::vec3 rot);

	// math helper
	static float calculate_vecangle(glm::vec2 a,glm::vec2 b);
	static void transform_vector(glm::vec3 &ov,glm::vec3 pos,float scl,glm::vec3 rot);
	static void rotate_vector(glm::vec3 &ov,glm::vec3 rot);
	static void transition_float_on_condition(float &tval,float tspeed,bool cnd);

	// timing debug
	static void start_debug_logging(DebugLogData &dld,const char* tname);
	static void add_timekey(DebugLogData &dld,const char* kname);
	static void flush_debug_logging(DebugLogData dld);

	// vertex setup
	static void create_sprite_canvas(std::vector<float>& vs,uint16_t ofs,glm::vec2 pos,float width,float height);
	static void create_sprite_canvas_triangled(std::vector<float>& vs,uint16_t ofs,glm::vec2 pos,float width,float height);
	// TODO: find a way to create function overloads, that directly add to a vertex array!

	// graphical setup
	static void load_texture(uint32_t tex,const char* path,bool corrected=false);
	static void load_texture(uint32_t tex,const char* path,float bias,bool corrected=false);
	static void load_texture_unfiltered(uint32_t tex,const char* path,bool corrected=false);
	static void load_texture_repeat(uint32_t tex,const char* path,bool corrected=false);
	static void generate_elements(uint16_t i,std::vector<uint32_t>& ls);

	// filter settings
	static void set_texture_parameter_linear_mipmap();
	static void set_texture_parameter_linear_unfiltered();
	static void set_texture_parameter_nearest_unfiltered();
	static void set_cubemap_texture_parameters();
	static void set_cubemap_texture_parameters_mipmap();

	// pattern handling
	static void set_texture_parameter_clamp_to_edge();
	static void set_texture_parameter_clamp_to_border();
	static void set_texture_parameter_texture_repeat();

	// inline definition
	static inline std::vector<float> create_sprite_canvas() {
		return {
			-1.0f,	1.0f,	0.0f,	1.0f,
			-1.0f,	-1.0f,	0.0f,	0.0f,
			1.0f,	-1.0f,	1.0f,	0.0f,
			-1.0f,	1.0f,	0.0f,	1.0f,
			1.0f,	-1.0f,	1.0f,	0.0f,
			1.0f,	1.0f,	1.0f,	1.0f
		};
	}
	// TODO: find out if vectorcopy is optimized away when inline

private:

	// helpers
	static void load_texture_function_head(uint32_t tex,const char* path,bool corrected);
};

#endif
