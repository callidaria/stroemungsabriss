#include "renderer.h"


/**
 * TODO: expand
*/

/*
	TODO
*/
void RTransform2D::transform(glm::vec2 p,float w,float h,float r)
{
	rotate(r);
	scale(w,h);
	translate(p);
}

/*
	TODO
*/
void RTransform2D::transform(glm::vec2 p,float w,float h,float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// execute transformation
	rotate(r);
	scale(w,h);
	translate(position+p-a);

	// save current rotation
	rot = r;
}

/*
	TODO
*/
void RTransform2D::scale(float w,float h,glm::vec2 a)
{
	// reset towards arbitrary origin
	model = glm::mat4(1.f);
	translate(a-position);

	// scale from arbitrary position & reconstruct
	rotate(rot);
	scale(w,h);
	translate(position-a);
}

/*
	TODO
*/
void RTransform2D::rotate(float r,glm::vec2 a)
{
	// reset towards arbitrary origin
	float t_wfac = model[0][0], t_hfac = model[1][1];
	model = glm::mat4(1.f);
	translate(a-position);

	// change rotation and reset scaling and previous position
	rotate(r);
	scale(t_wfac,t_hfac);

	// reconstruct valid transform state
	translate(position-a);
	rot = r;
}


/**
 * TODO: expand
*/

/*
	TODO
*/
Renderer::Renderer()
{
	// setup element array buffers for sprites
	for (uint8_t i=0;i<RENDERER_BUFFERS_SPRITE_COUNT;i++) bfr_sprite[i].buffer.add_buffer();
}

/*
	!O(1)m /+load -> (public)
	purpose: add new sprite to renderer
	\param bfr_id: id of sprite buffer to write sprite to
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	\param t: path to file containing sprite texture
	\returns: memory index the sprite can be referenced by later
*/
uint16_t Renderer::add_sprite(uint8_t bfr_id,glm::vec2 p,float w,float h,const char* t)
{
	// information setup
	Sprite s = {
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},
		.texture = { .path = t },
	};

	// data setup
	bfr_sprite[bfr_id].sprites.push_back(s);
	return bfr_sprite[bfr_id].sprites.size()-1;
}

/*
	!O(1)m /+load -> (public)
	purpose: add new spritesheet to renderer
	\param bfr_id: id of sprite buffer to write animation to
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	\param t: path to file containing spritesheet
	\param r: rows on spritesheet
	\param c: columns on spritesheet
	\param f: number of frames held by spritesheet
	\param s: frames the animation takes to fully iterate through all textures
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t Renderer::add_sprite(uint8_t bfr_id,glm::vec2 p,float w,float h,const char* t,
		uint8_t r,uint8_t c,uint8_t f,uint8_t s)
{
	// information setup
	Atlas a = {
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},
		.texture = { .path = t },
		.rows = r,
		.columns = c,
		.frames = f,
		.span = s
	};

	// data setup
	bfr_sprite[bfr_id].atlas.push_back(a);
	return bfr_sprite[bfr_id].atlas.size()-1;
}


/**
 *	TODO
*/

typedef void (*sprite_buffer_routine)(SpriteBuffer&);

/*
	TODO
*/
void sprite_buffer_idle(SpriteBuffer& sb)
{
	// TODO: what even to do here? maintaining checks, idle update, can this be skipped?
	//		buffers definitely will be in this state and the update has to be handled accordingly (no ee)
}

/*
	// TODO
*/
void sprite_buffer_load(SpriteBuffer& sb)
{
	// memory allocation for vertices and elements
	sb.vertices.resize((sb.sprites.size()+sb.atlas.size())*PATTERN_SPRITE_VERTEX_REPEAT);
	sb.elements.resize((sb.sprites.size()+sb.atlas.size())*PATTERN_SPRITE_ELEMENT_REPEAT);

	// write sprite vertex values to upload list
	size_t t_vsize = 0, t_esize = 0, i_velem = 0;
	for (Sprite& s : sb.sprites) {
		Toolbox::create_sprite_canvas(sb.vertices,t_vsize,
				s.transform.position,s.transform.width,s.transform.height);
		Toolbox::generate_elements(t_esize,i_velem,sb.elements);
	}

	// write animation vertex values to upload list
	for (Atlas& a : sb.atlas) {
		Toolbox::create_sprite_canvas(sb.vertices,t_vsize,
				a.transform.position,a.transform.width,a.transform.height);
		Toolbox::generate_elements(t_esize,i_velem,sb.elements);
	}

	// upload to buffers
	sb.buffer.bind();
	sb.buffer.upload_vertices(sb.vertices), sb.buffer.upload_elements(sb.elements);

	// compile shader
	sb.shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");

	// load textures
	for (Sprite& s : sb.sprites) s.texture.load();
	for (Atlas& a : sb.atlas) a.texture.load();
	sb.shader.upload_int("tex",0);

	// coordinate system & transition buffer into next state
	sb.shader.upload_camera();
	sb.state = (BufferState)(sb.state+sb.auto_stateswitch);
	// TODO: change auto stateswitch on demand later
}
// FIXME: extremely questionable loading processing

/*
	TODO
*/
void sprite_buffer_render(SpriteBuffer& sb)
{
	// prepare shader and bind buffers
	sb.shader.enable();
	sb.shader.upload_int("row",1);
	sb.shader.upload_int("col",1);
	sb.shader.upload_vec2("i_tex",glm::vec2(0));
	sb.buffer.bind();

	// iterate sprites
	for (uint16_t i=0;i<sb.sprites.size();i++) {
		sb.shader.upload_matrix("model",sb.sprites[i].transform.model);
		glBindTexture(GL_TEXTURE_2D,sb.sprites[i].texture.texture);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
	}

	// iterate animations
	// TODO
	// FIXME: this would mean, that all animations will be rendered on top of sprites... not ideal
}

// behaviour mapped towards BufferState enumeration
sprite_buffer_routine routine_sbuffers[RBFR_STATE_COUNT] = {
	sprite_buffer_idle,
	sprite_buffer_load,
	sprite_buffer_render
};


/*
	TODO
*/
void Renderer::update()
{
	// iterate sprite buffer
	for (uint8_t i=0;i<RENDERER_BUFFERS_SPRITE_COUNT;i++) {
		SpriteBuffer& bfr = bfr_sprite[i];
		routine_sbuffers[bfr.state](bfr);
	}
}

/*
	!O(1) /+function -> (public)
	purpose: prepare shader and buffer to render sprites and animated spritesheets
*/
/*
void Renderer::prepare_sprites()
{
	sprite_shader.enable();
	sprite_shader.upload_int("row",1);
	sprite_shader.upload_int("col",1);
	sprite_shader.upload_vec2("i_tex",glm::vec2(0));
	sprite_buffer.bind();
}
*/

/*
	!O(1) /update -> (public)
	purpose: draw indexed sprite
	\param i: sprite index
*/
/*
void Renderer::render_sprite(uint16_t i)
{
	sprite_shader.upload_matrix("model",sprites[i].transform.model);
	glBindTexture(GL_TEXTURE_2D,sprites[i].texture.texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
}
*/

/*
	!O(1) /update -> (public)
	purpose: same basic functionality as the normal render_sprite but with previous texture overwrite
	\param i: sprite index
	\param tex: texture to overwrite the normal sprite texture before render
*/
/*
void Renderer::render_sprite_overwritten(uint16_t i,uint32_t tex)
{
	sprite_shader.upload_matrix("model",sprites[i].transform.model);
	glBindTexture(GL_TEXTURE_2D,tex);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
}
*/

/*
	!O(1) /update -> (public)
	purpose: permanently render a specific frame of a spritesheet
	\param i: spritesheet index
	\param pos: two-dimensional index of spritesheet frame to draw
*/
/*
void Renderer::render_sprite_frame(uint16_t i,glm::vec2 pos)
{
	// shader upload
	sprite_shader.upload_int("row",atlas[i].rows);
	sprite_shader.upload_int("col",atlas[i].columns);
	sprite_shader.upload_vec2("i_tex",pos);
	sprite_shader.upload_matrix("model",atlas[i].transform.model);

	// draw sprite
	glBindTexture(GL_TEXTURE_2D,atlas[i].texture.texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sprites.size())*6*sizeof(uint32_t)));
}
*/

/*
	!O(1) /update -> (public)
	purpose: animate spritesheet and automatically iterate through all contained frames
	\param i: spritesheet index
*/
/*
void Renderer::render_sprite_animated(uint16_t i)
{
	glBindTexture(GL_TEXTURE_2D,atlas[i].texture.texture);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sprites.size())*6*sizeof(uint32_t)));
}
*/
