#ifndef CCB_MATHEMATICS_TOOLBOX
#define CCB_MATHEMATICS_TOOLBOX

#include <iostream>
#include <string>
#include <vector>
#include <queue>
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


// logger macro definition to include component in debug build
#ifdef DEBUG

// text colour definitions
constexpr const char* LOG_WHITE = "\e[0;30m";
constexpr const char* LOG_RED = "\e[1;31m";
constexpr const char* LOG_GREEN = "\e[1;32m";
constexpr const char* LOG_YELLOW = "\e[1;33m";
constexpr const char* LOG_BLUE = "\e[1;34m";
constexpr const char* LOG_PURPLE = "\e[1;35m";
constexpr const char* LOG_CYAN = "\e[1;36m";
constexpr const char* LOG_GREY = "\e[1;90m";
constexpr const char* LOG_CLEAR = "\e[0;39m";

// functionality shortcuts
constexpr const char* LOG_HEADINGS = LOG_PURPLE;
constexpr const char* LOG_DESTRUCTION = LOG_CYAN;
constexpr const char* LOG_SETTINGS = LOG_YELLOW;
constexpr const char* LOG_TIMING = LOG_GREY;
constexpr const char* LOG_SNITCH = LOG_BLUE;

// time performance records
inline std::chrono::steady_clock::time_point log_tdelta = std::chrono::steady_clock::now();
static inline void reset_timestamp() { log_tdelta = std::chrono::steady_clock::now(); }
static inline void produce_timestamp(bool padding=true)
{
	double ll_delta = (std::chrono::steady_clock::now()-log_tdelta).count()*CONVERSION_MULT_MILLISECONDS;
	printf((padding) ? "%s%12fms" : "%s%fms",LOG_TIMING,ll_delta);
	reset_timestamp();
}

// basic console communication
#define COMM_RST() reset_timestamp()
#define COMM_AWT(...) COMM_RST(),printf(__VA_ARGS__),printf("... ");
#define COMM_CNF() printf("%sdone%s in ",LOG_GREEN,LOG_TIMING),produce_timestamp(false),printf("%s\n",LOG_CLEAR);
#define COMM_MSG(col,...) produce_timestamp(),printf(" | %s",col),printf(__VA_ARGS__),printf("%s\n",LOG_CLEAR);
#define COMM_ERR(...) printf("%serror: ",LOG_RED),printf(__VA_ARGS__),printf("%s\n",LOG_CLEAR);
#define COMM_LOG(...) COMM_MSG(LOG_CLEAR,__VA_ARGS__);
#define COMM_SCC(...) COMM_MSG(LOG_GREEN,__VA_ARGS__);

// communicate by condition
#define COMM_MSG_COND(cnd,col,...) if (cnd) { COMM_MSG(col,__VA_ARGS__); }
#define COMM_ERR_COND(cnd,...) if (cnd) { COMM_ERR(__VA_ARGS__); }
#define COMM_MSG_FALLBACK(col,...) else { COMM_MSG(col,__VA_ARGS__); }
#define COMM_ERR_FALLBACK(...) else { COMM_ERR(__VA_ARGS__); }

// logger macro definition to exclude component in release build
#else

#define COMM_RST()
#define COMM_AWT(...)
#define COMM_CNF()
#define COMM_MSG(col,...)
#define COMM_ERR(...)
#define COMM_LOG(...)
#define COMM_SCC(...)
#define COMM_MSG_COND(cnd,col,...)
#define COMM_ERR_COND(cnd,...)
#define COMM_MSG_FALLBACK(col,...)
#define COMM_ERR_FALLBACK(...)

#endif


// data to represent detatched and waiting thread state
/*
struct ThreadState
{
	std::mutex mux;
	std::condition_variable cond;
	volatile bool active = true;
};
*/

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

	// threading
	/*
	static void thread_detached_continue(ThreadState& state);
	static void thread_detached_stop(ThreadState& state);
	*/

	// vertex setup
	/*
	static void create_sprite_canvas(std::vector<float>& vs,size_t& ofs,glm::vec2 pos,float width,float height);
	static void create_sprite_canvas_triangled(std::vector<float>& vs,size_t& ofs,
			glm::vec2 pos,float width,float height);
	static void generate_elements(size_t& k0,size_t& k1,std::vector<uint32_t>& e);
	*/

	// inline definition
	/*
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
	*/
	// TODO: find out if vectorcopy is optimized away when inline
};

#endif
