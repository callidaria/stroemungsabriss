#ifndef CCB_MATHEMATICS_TOOLBOX
#define CCB_MATHEMATICS_TOOLBOX

#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <chrono>

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


// define build state
#define DEBUG
// TODO: move debug flag definition


// math constants
constexpr double MATH_CARTESIAN_XRANGE = 1280.f;
constexpr double MATH_CARTESIAN_YRANGE = 720.f;
constexpr double MATH_PI = 3.141592653;
constexpr double MATH_E = 2.7182818284;
constexpr double MATH_CENTER_GOLDEN = MATH_CARTESIAN_XRANGE*.618;

// conversion
constexpr uint32_t CONVERSION_THRES_MILLISECONDS = 1000;
constexpr uint32_t CONVERSION_THRES_MICROSECONDS = 1000000;
constexpr uint32_t CONVERSION_THRES_NANOSECONDS = 1000000000;
constexpr double CONVERSION_MULT_MICROSECONDS = .001;
constexpr double CONVERSION_MULT_MILLISECONDS = .000001;
constexpr double CONVERSION_MULT_SECONDS = .000000001;

// pattern
constexpr uint8_t PATTERN_SPRITE_LOAD_REPEAT = 4;
constexpr uint8_t PATTERN_SPRITE_ELEMENT_REPEAT = 6;
constexpr uint8_t PATTERN_SPRITE_VERTEX_REPEAT = 4*PATTERN_SPRITE_LOAD_REPEAT;
constexpr uint8_t PATTERN_SPRITE_TRIANGLE_REPEAT = 6*PATTERN_SPRITE_LOAD_REPEAT;
constexpr uint8_t PATTERN_OBJECT_LOAD_REPEAT = 11;


// logger macro definitions
// text colour definitions
constexpr const char* LOG_RED = "\033[1;31m";
constexpr const char* LOG_BLUE = "\033[1;36m";
constexpr const char* LOG_CLEAR = "\033[0m";

// logger macro definition to include component in debug build
#ifdef DEBUG

// basic console communication
#define COMM_MSG(col,...) printf("%s",col),printf(__VA_ARGS__),printf("%s\n",LOG_CLEAR);
#define COMM_ERR(msg) printf("%s%s%s\n",LOG_RED,msg,LOG_CLEAR);

// communicate by condition
#define COMM_MSG_COND(msg,col,cnd) if (cnd) COMM_MSG(msg,col);
#define COMM_ERR_COND(msg,cnd) if (cnd) COMM_ERR(msg);

// switch errors
#define COMM_SWITCH(msg,err,col,cnd) if (cnd) { COMM_MSG(msg,col); } else { COMM_ERR(err); }

// logger macro definition to exclude component in release build
#else

#define COMM_MSG(msg,col)
#define COMM_ERR(msg)
#define COMM_MSG_COND(msg,col,cnd)
#define COMM_ERR_COND(msg,cnd)
#define COMM_SWITCH(msg,err,col,cnd)

#endif


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
	std::vector<DebugLogKey> key_list = {  };
	std::chrono::steady_clock::time_point last_ticks;
	uint8_t max_name_width = 0;
};

// data to represent detatched and waiting thread state
struct ThreadState
{
	std::mutex mux;
	std::condition_variable cond;
	volatile bool active = true;
};

class Toolbox
{
public:

	// system
	static void copy_file(const char* origin,const char* dest);

	// loader
	//static uint32_t load_object(const char* path,std::vector<float>& ov,glm::vec3 pos,
	//		float scl,glm::vec3 rot);

	// math helper
	/*
	static float calculate_vecangle(glm::vec2 a,glm::vec2 b);
	static void transform_vector(glm::vec3 &ov,glm::vec3 pos,float scl,glm::vec3 rot);
	static void rotate_vector(glm::vec3 &ov,glm::vec3 rot);
	*/
	static void transition_float_on_condition(float &tval,float tspeed,bool cnd);

	// timing debug
	/*
	static void start_debug_logging(DebugLogData &dld,const char* tname);
	static void add_timekey(DebugLogData &dld,const char* kname);
	static void flush_debug_logging(DebugLogData dld);
	*/

	// threading
	static void thread_detached_continue(ThreadState& state);
	static void thread_detached_stop(ThreadState& state);

	// vertex setup
	static void create_sprite_canvas(std::vector<float>& vs,size_t& ofs,glm::vec2 pos,float width,float height);
	static void create_sprite_canvas_triangled(std::vector<float>& vs,size_t& ofs,
			glm::vec2 pos,float width,float height);
	static void generate_elements(size_t& k0,size_t& k1,std::vector<uint32_t>& e);

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
	/*
	static inline std::string produce_logging_cell(std::string content,std::string col,uint8_t len)
		{ return "| "+col+content+std::string(len-content.length(),' ')+"\033[0m |"; }
	*/
};

#endif
