#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include <queue>

#include "../core.h"
#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"
#include "shader.h"


// memory
constexpr uint8_t RENDERER_BUFFERS_SPRITE_COUNT = 4;
constexpr uint8_t RENDERER_TEXTURE_BUFFER_COUNT = 4;
constexpr uint32_t RENDERER_TEXTURE_BUFFER_WIDTH = 4096;
constexpr uint32_t RENDERER_TEXTURE_BUFFER_HEIGHT = 4096;
constexpr uint32_t RENDERER_TEXTURE_BUFFER_SIZE = RENDERER_TEXTURE_BUFFER_WIDTH*RENDERER_TEXTURE_BUFFER_HEIGHT*4;


// basic functionality of 2D geometry
struct RTransform2D
{
	// transformation utility
	void transform(glm::vec2 p,float w,float h,float r);
	void transform(glm::vec2 p,float w,float h,float r,glm::vec2 a);
	inline void translate(glm::vec2 p) { model = glm::translate(model,glm::vec3(p.x,p.y,0)); }
	inline void scale(float w,float h) { model[0][0] = w, model[1][1] = h; }
	void scale(float w,float h,glm::vec2 a);
	inline void rotate(float r) { model = glm::rotate(model,glm::radians(r),glm::vec3(0,0,1)); }
	void rotate(float r,glm::vec2 a);
	inline void to_origin() { translate(position), scale(width,height); }

	// attributes
	const glm::vec2 position;
	const float width,height;
	float rot = .0f;

	// transform
	glm::mat4 model = glm::mat4(1.f);
};

// texture duality structure containing source and data
struct RTextureTuple
{
	// utility
	inline void load() { glGenTextures(1,&texture), Toolbox::load_texture(texture,path); }

	// data
	uint32_t texture;
	const char* path;

	// math
	uint8_t rows,columns;
	uint8_t frames;
};

// description of current buffer state, is it rendering, is it loading, should it be ignored?
enum BufferState
{
	RBFR_IDLE,
	RBFR_LOAD,
	RBFR_RENDER,
	RBFR_STATE_COUNT
};

// attribute structure for all object buffers
struct BufferAttribs
{
	BufferState state = RBFR_IDLE;
	bool auto_stateswitch = false;
};

// render entry connected to loaded sprite and individually transformed
struct Sprite
{
	RTransform2D transform;
	uint16_t texture_id;
	glm::vec2 atlas_index = glm::vec2(0);
};

// animation entry connected to render entry, holding state of animation
struct SpriteAnimation
{
	uint16_t id;
	uint8_t cycle_duration;
	float frame_duration,anim_progression = .0f;
};

// buffer data to seperately load and display to other buffers
struct SpriteBuffer
{
	std::vector<RTextureTuple> textures;
	std::vector<Sprite> sprites;
	std::vector<SpriteAnimation> animations;
	BufferAttribs attribs;
};

// load thread data
struct SpriteLoadInstrData
{
	SDL_GLContext* context;
	std::queue<SpriteBuffer*> ldbfr;
	volatile bool snap_load = false;
};

// sdl thread testing data structure
/*
struct CombinedLoadData
{
	SpriteLoadInstrData* data;
	ThreadState* state;
};
*/


class Renderer
{
public:

	// construction
	Renderer();
	~Renderer() {  }

	// object adder
	uint16_t add_sprite(uint8_t bfr_id,const char* texpath,uint8_t r=1,uint8_t c=1,uint8_t f=1);

	// registration
	void register_sprite(uint8_t bfr_id,uint16_t tex_id,glm::vec2 p,float w,float h,
			bool animate=false,uint8_t s=0);

	// stages
	void update();
	void close();

public:

	// shaders
	Buffer spr_buffer;
	Shader spr_shader;

	// buffers
	SpriteBuffer bfr_sprite[RENDERER_BUFFERS_SPRITE_COUNT];
	//uint32_t texture_buffers[RENDERER_TEXTURE_BUFFER_COUNT];

private:

	// threading
	//std::thread th_sprite_loader;
	SDL_Thread* ld_thread;
};

static inline ThreadState th_ldsprite_data;
static inline SpriteLoadInstrData th_inst_sprite_data;
//static inline CombinedLoadData th_data;

#endif
