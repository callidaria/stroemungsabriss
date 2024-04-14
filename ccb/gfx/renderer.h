#ifndef CCB_GRAPHICS_RENDERER
#define CCB_GRAPHICS_RENDERER

#include "../mat/toolbox.h"
#include "../mat/camera2d.h"  // TODO: join camera definitions into one file
#include "../fcn/buffer.h"
#include "shader.h"


// data structure for sprite information
struct Sprite
{
	// attributes
	glm::vec2 position;
	float width,height;
	const char* texpath;

	// data
	glm::mat4 model = glm::mat4(1.f);
	uint32_t texture;
};

// data structure for spritesheet information
struct Atlas
{
	// attributes
	glm::vec2 position;
	float width,height;
	const char* texpath;
	uint8_t rows,columns;
	uint8_t frames,span;

	// data
	glm::mat4 model = glm::mat4(1.f);
	uint32_t texture;
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

	// setup
	void load();
	void prepare_sprites();
	// FIXME: restructure loading concept

	// draw
	void render_sprite(uint16_t i);
	void render_sprite_overwritten(uint16_t i,uint32_t tex);
	void render_sprite_frame(uint16_t i,glm::vec2 pos);
	void render_sprite_animated(uint16_t i);

private:

	// engine
	Buffer sprite_buffer;
	Shader sprite_shader;

	// data
	std::vector<Sprite> sprites;
	std::vector<Atlas> atlas;
	std::vector<float> sprite_vertices;
	std::vector<uint32_t> sprite_elements;
};

#endif
