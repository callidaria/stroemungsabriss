#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/math.h"
#include "../fcn/buffer.h"

#include "shader.h"
#include "texture.h"


// upload structures
// per instance upload datastructure for duplicate shader
struct SpriteInstanceUpload
{
	// utility
	void set_rotation(float dg_rot);

	// data
	glm::vec2 offset = glm::vec2(0);
	float rotation_sin = .0f, rotation_cos = 1.f;
	glm::vec2 atlas_index = glm::vec2(0);
};
constexpr uint8_t INSTANCE_SHADER_UPLOAD_REPEAT = sizeof(SpriteInstanceUpload)/sizeof(float);


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

// animation entry connected to sprite render entry, holding state of animation
struct SpriteAnimation
{
	uint16_t id;
	uint8_t cycle_duration;
	float frame_duration, anim_progression = .0f;
};

// data structure to represent a full set of duplicates
constexpr uint16_t INSTANCE_CAPACITY = 4096;
struct SpriteInstance
{
	uint16_t texture_id;
	Transform2D transform;
	SpriteInstanceUpload upload[INSTANCE_CAPACITY];
	uint16_t active_range = 0;
};

// animation entry connected to duplicate render entry, holding state of animation
struct SpriteAnimationInstance
{
	uint16_t id;
	uint8_t cycle_duration;
	float anim_progressions[INSTANCE_CAPACITY] = { .0f };
	float frame_duration;
};

// batch data to seperately load and display to other buffers
struct RenderBatch
{
	// sprites
	std::vector<SpriteTextureTuple> textures;
	std::vector<Sprite> sprites;
	std::vector<SpriteAnimation> anim_sprites;

	// instanced sprites
	std::vector<SpriteInstance> duplicates;
	std::vector<SpriteAnimationInstance> anim_duplicates;

	// attributes
	BatchState state = RBFR_IDLE;
	volatile bool load_semaphore = false;
};
constexpr uint8_t RENDERER_BATCHES_COUNT = 4;


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
	void register_sprite(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur);
	void register_duplicate(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h);
	void register_duplicate(uint8_t batch_id,uint16_t tex_id,glm::vec2 p,float w,float h,uint8_t dur);

	// spawn
	void spawn_sprite_instance(uint8_t batch_id,uint16_t inst_id,glm::vec2 ofs,
			float rot=.0f,glm::vec2 subtex=glm::vec2(0));

	// stages
	void update();

public:

	// shaders
	Buffer spr_buffer;
	Shader spr_shader,dpl_shader;

	// buffers
	RenderBatch batches[RENDERER_BATCHES_COUNT];
};

inline Renderer g_Renderer = Renderer();

#endif
