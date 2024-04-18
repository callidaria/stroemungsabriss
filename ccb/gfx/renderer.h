#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"
#include "shader.h"


// basic functionality of 2D geometry
struct RTransform2D
{
	// streamline utility
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
	inline void load() { glGenTextures(1,&texture),Toolbox::load_texture(texture,path); }

	// data
	uint32_t texture;
	const char* path;
};

// data structure for sprite information
struct Sprite
{
	RTransform2D transform;
	RTextureTuple texture;
};

// data structure for spritesheet information
struct Atlas
{
	// fundamentals
	RTransform2D transform;
	RTextureTuple texture;

	// attributes
	uint8_t rows,columns;
	uint8_t frames,span;
};


class Renderer
{
public:

	// construction
	Renderer();
	~Renderer() {  }

	// object adder
	uint16_t add_sprite(glm::vec2 p,float w,float h,const char* t);
	uint16_t add_sprite(glm::vec2 p,float w,float h,const char* t,uint8_t r,uint8_t c,uint8_t f,uint8_t s);

	// transformation
	Sprite transform(Sprite spr,glm::vec2 dir,float wdt,float hgt,glm::vec3 rot);
	Sprite translate(Sprite spr,glm::vec2 dir);
	Sprite scale(Sprite spr,float wdt,float height);
	Sprite rotate(Sprite spr,glm::vec3 rot);
	// TODO: make this part of sprite :(

	// setup
	void load();
	void prepare_sprites();
	// FIXME: restructure loading concept

	// draw
	void render_sprite(uint16_t i);
	void render_sprite_overwritten(uint16_t i,uint32_t tex);
	void render_sprite_frame(uint16_t i,glm::vec2 pos);
	void render_sprite_animated(uint16_t i);

public:

	// objects
	std::vector<Sprite> sprites;
	std::vector<Atlas> atlas;

private:

	// engine
	Buffer sprite_buffer;
	Shader sprite_shader;

	// data
	std::vector<float> sprite_vertices;
	std::vector<uint32_t> sprite_elements;
};

#endif
