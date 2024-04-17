#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"
#include "shader.h"


// basic functionality of 2D geometry
struct Geometry2D
{
	// streamline utility
	// TODO

	// attributes
	glm::vec2 position;
	float width,height;

	// transform
	glm::mat4 model = glm::mat4(1.f);
};

// data structure for sprite information
struct Sprite : Geometry2D
{
	// texture
	const char* texpath;
	uint32_t texture;
};

// data structure for spritesheet information
struct Atlas : Geometry2D
{
	// attributes
	uint8_t rows,columns;
	uint8_t frames,span;

	// texture
	const char* texpath;
	uint32_t texture;
};
// TODO: possibly choose containment over inheritance


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
