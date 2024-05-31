#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"
#include "shader.h"


constexpr uint8_t RENDERER_BUFFERS_SPRITE_COUNT = 4;


// description of current buffer state, is it rendering, is it loading, should it be ignored?
enum BufferState
{
	RBFR_IDLE,
	RBFR_LOAD,
	RBFR_RENDER,
	RBFR_STATE_COUNT
};

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
};

struct Sprite
{
	RTransform2D transform;
	uint16_t texture_id;
	uint8_t rows,columns;
	uint8_t frames,span;
};

// buffer data to seperately load and display to other buffers
struct SpriteBuffer
{
	// data
	std::vector<RTextureTuple> textures;
	std::vector<Sprite> sprites;

	// state
	BufferState state = RBFR_IDLE;
	bool auto_stateswitch = true;
};


class Renderer
{
public:

	// construction
	Renderer();
	~Renderer() {  }

	// object adder
	uint16_t add_sprite(uint8_t bfr_id,const char* texpath);

	// registration
	void register_sprite(uint8_t bfr_id,uint16_t tex_id,glm::vec2 p,float w,float h,
			uint8_t r=1,uint8_t c=1,uint8_t f=0,uint8_t s=0);

	// interaction
	inline void load_buffer_sprites(uint8_t id) { bfr_sprite[id].state = RBFR_LOAD; }

	// stages
	void load();
	void update();

public:

	// shaders
	Buffer spr_buffer;
	Shader spr_shader;

	// buffers
	SpriteBuffer bfr_sprite[RENDERER_BUFFERS_SPRITE_COUNT];
};

#endif
