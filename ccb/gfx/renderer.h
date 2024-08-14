#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"

#include "shader.h"
#include "texture.h"


// maxima
constexpr uint8_t RENDERER_INTERPRETER_COMMAND_COUNT = 3;
constexpr uint8_t RENDERER_BATCHES_COUNT = 4;
constexpr uint16_t INSTANCE_CAPACITY = 4096;


// transformation math
// basic functionality of 2D geometry
struct Transform2D
{
	// transformation combined
	void transform(glm::vec2 p,float w,float h,float r);
	void transform(glm::vec2 p,float w,float h,float r,glm::vec2 a);
	void to_origin();

	// transformation utility
	inline void translate(glm::vec2 p) { model = glm::translate(model,glm::vec3(p.x,p.y,0)); }
	inline void scale(float w,float h) { model[0][0] = w, model[1][1] = h; }
	inline void rotate(float r) { model = glm::rotate(model,glm::radians(r),glm::vec3(0,0,1)); }

	// arbitrary transformation
	void scale(float w,float h,glm::vec2 a);
	void rotate(float r,glm::vec2 a);

	// attributes
	const glm::vec2 position;
	const float width,height;
	float rot = .0f;

	// transform
	glm::mat4 model = glm::mat4(1.f);
};


// upload structures
// TODO
struct SpriteInstanceUpload
{
	// utility
	void set_rotation(float dg_rot);

	// data
	glm::vec2 offset = glm::vec2(0);
	float rotation_sin = .0f, rotation_cos = 1.f;
	glm::vec2 atlas_index = glm::vec2(0);
};


// batch datastructure definitions
// description of current buffer state, is it rendering, is it loading, should it be ignored?
enum BatchState
{
	RBFR_IDLE,
	RBFR_LOAD,
	RBFR_UPLOAD,
	RBFR_RENDER,
	RBFR_STATE_COUNT
};


// sprite data
// texture information for sprites
struct SpriteTextureTuple
{
	Texture texture;
	uint8_t rows,columns;
	uint16_t frames;
};

// render entry connected to loaded sprite and individually transformed
struct Sprite
{
	uint16_t texture_id;
	Transform2D transform;
	glm::vec2 atlas_index = glm::vec2(0);
};

// animation entry connected to render entry, holding state of animation
struct SpriteAnimation
{
	uint16_t id;
	uint8_t cycle_duration;
	float frame_duration, anim_progression = .0f;
};

// TODO
struct SpriteInstance
{
	uint16_t texture_id;
	SpriteInstanceUpload upload[INSTANCE_CAPACITY];
};

// TODO
struct SpriteAnimationInstance
{
	uint16_t id;
	uint8_t cycle_durations[INSTANCE_CAPACITY];
	float anim_progression[INSTANCE_CAPACITY] = { .0f };
	float frame_duration;
};

// batch data to seperately load and display to other buffers
struct RenderBatch
{
	// sprites
	std::vector<SpriteTextureTuple> textures;
	std::vector<Sprite> sprites;
	std::vector<SpriteAnimation> animations;

	// instanced sprites
	std::vector<SpriteInstance> inst_sprites;
	std::vector<SpriteAnimationInstance> inst_animations;

	// attributes
	BatchState state = RBFR_IDLE;
	volatile bool load_semaphore = false;
};


class Renderer
{
public:

	// construction
	Renderer();
	~Renderer() {  }

	// interpretation
	void compile(const char* path);

	// object adder
	uint16_t add_sprite(uint8_t batch_id,const char* texpath,uint8_t r=1,uint8_t c=1,uint8_t f=1);

	// registration
	void register_sprite(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h);
	void register_animation(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur);

	// stages
	void update();

public:

	// shaders
	Buffer spr_buffer;
	Shader spr_shader;

	// buffers
	RenderBatch batches[RENDERER_BATCHES_COUNT];
};

inline Renderer g_Renderer = Renderer();

#endif
