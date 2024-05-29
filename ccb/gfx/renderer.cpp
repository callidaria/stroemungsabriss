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
	purpose: add new spritesheet to renderer
	\param bfr_id: id of sprite buffer to write animation to
	\param p: origin position of added sprite
	\param w: width of added sprite
	\param h: height of added sprite
	\param t: path to file containing spritesheet
	\param r (default=1): rows on spritesheet
	\param c (default=1): columns on spritesheet
	\param f (default=0): number of frames held by spritesheet
	\param s (default=0): frames the animation takes to fully iterate through all textures
	\returns: memory index the spritesheet can be referenced by later
*/
uint16_t Renderer::add_sprite(uint8_t bfr_id,glm::vec2 p,float w,float h,const char* t,
		uint8_t r,uint8_t c,uint8_t f,uint8_t s)
{
	// information setup
	Sprite sprite = {

		// transform component
		.transform = {
			.position = p,
			.width = w,
			.height = h,
		},

		// texture component
		.texture = { .path = t },

		// attributes
		.rows = r,
		.columns = c,
		.frames = f,
		.span = s
	};

	// data setup
	bfr_sprite[bfr_id].sprites.push_back(sprite);
	return bfr_sprite[bfr_id].sprites.size()-1;
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
	sb.vertices.resize(sb.sprites.size()*PATTERN_SPRITE_VERTEX_REPEAT);
	sb.elements.resize(sb.sprites.size()*PATTERN_SPRITE_ELEMENT_REPEAT);

	// write sprite vertex values to upload list
	size_t t_vsize = 0, t_esize = 0, i_velem = 0;
	for (Sprite& s : sb.sprites) {
		Toolbox::create_sprite_canvas(sb.vertices,t_vsize,
				s.transform.position,s.transform.width,s.transform.height);
		Toolbox::generate_elements(t_esize,i_velem,sb.elements);
	}

	// upload to buffers & compile
	sb.buffer.bind();
	sb.buffer.upload_vertices(sb.vertices), sb.buffer.upload_elements(sb.elements);
	sb.shader.compile2d("./shader/obj/sprite.vs","./shader/standard/direct.fs");

	// load textures
	for (Sprite& s : sb.sprites) s.texture.load();
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
	sb.buffer.bind();

	// iterate sprites
	for (uint16_t i=0;i<sb.sprites.size();i++) {

		// upload sprite attributes
		sb.shader.upload_int("row",sb.sprites[i].rows);
		sb.shader.upload_int("col",sb.sprites[i].columns);
		sb.shader.upload_vec2("i_tex",glm::vec2(0));
		sb.shader.upload_matrix("model",sb.sprites[i].transform.model);

		// draw sprite
		glBindTexture(GL_TEXTURE_2D,sb.sprites[i].texture.texture);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(uint32_t)));
	}
}

// behaviours mapped towards BufferState enumeration
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
