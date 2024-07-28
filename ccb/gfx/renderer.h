#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"

#include "shader.h"
#include "texture.h"


// memory
constexpr uint8_t RENDERER_BUFFERS_SPRITE_COUNT = 4;


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

// TODO
struct TextureAtlasTuple
{
	uint8_t rows,columns;
	uint16_t frames;
};

// TODO
struct RTextureTuple
{
	Texture texture;
	TextureAtlasTuple atlas;
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
	Transform2D transform;
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
};

inline Renderer g_Renderer = Renderer();

#endif
