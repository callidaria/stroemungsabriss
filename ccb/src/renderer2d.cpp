#include "../gfx/renderer2d.h"

/*
	constructor()
	purpose: create renderer object to subsequently add 2D objects to and draw them
*/
Renderer2D::Renderer2D()
{
	buffer.add_buffer();
} Renderer2D::~Renderer2D() {  }

/*
	add(vec2,float,float,const char*) -> uint16_t
	p: origin position of added sprite
	w: width of added sprite
	h: height of added sprite
	t: path to file containing sprite texture
	purpose: add new sprite to renderer to draw later
	returns: memory index the sprite is to be referenced by when drawing
*/
uint16_t Renderer2D::add(glm::vec2 p,float w,float h,const char* t)
{
	Sprite tmp = Sprite(p,w,h,t);  // TODO: fall into output while push back
	uint16_t out = sl.size();
	sl.push_back(tmp);
	return out;
}

/*
	add(vec2,float,float,const char*,uint8_t,uint8_t,uint8_t,uint8_t) -> uint16_t
	p: origin position of added animation
	w: width of added animation
	h: height of added animation
	t: path to file containing animation spritesheet
	r: rows of spritesheet located by [t]
	c: columns of spritesheet located by [t]
	itn: number of textures held by spritesheet
	f: frames the animation takes to fully iterate through all textures
	purpose: add new animation to renderer to draw later
	returns: memory index the animation is to be referenced by when drawing
*/
uint16_t Renderer2D::add(glm::vec2 p,float w,float h,const char* t,uint8_t r,uint8_t c,
		uint8_t itn,uint8_t f)
{
	Anim tmp = Anim(p,w,h,t,r,c,itn,f);  // TODO: fall into output while push back also
	uint16_t out = al.size();
	al.push_back(tmp);
	return out;
}

/*
	load_vertex() -> void
	purpose: upload all vertices of sprites and animations to buffer
*/
void Renderer2D::load_vertex()
{
	// getting list sizes
	uint16_t ls = sl.size(),la = al.size();

	// create write arrays
	float v[(ls+la)*16];
	unsigned int e[(ls+la)*6];

	// write values to arrays
	for (int j=0;j<ls+la;j++) {
		for (int i=0;i<16;i++) {
			if (j<ls) v[j*16+i] = sl.at(j).v[i];
			else v[j*16+i] = al.at(j-ls).v[i];
		}

		// create elements for element buffer
		e[j*6] = j*4;e[j*6+1] = j*4+1;e[j*6+2] = j*4+2;
		e[j*6+3] = j*4+2;e[j*6+4] = j*4+3;e[j*6+5] = j*4+0;
	}

	// upload to buffers
	buffer.bind();
	buffer.upload_vertices(v,sizeof(v));
	buffer.upload_elements(e,sizeof(e));
}
// FIXME: this can be way faster, what about the array -> vector && branching removal?
// FIXME: constant branching due to loop combination of animations and sprites. easily removable

/*
	load_texture() -> void
	purpose: load all textures for every added sprite and animation
*/
void Renderer2D::load_texture()
{
	for (int i=0;i<sl.size();i++) sl.at(i).texture();
	for (int i=0;i<al.size();i++) al.at(i).texture();
}

/*
	load() -> void
	purpose: combine vertex and texture loading and compile shader program in between
*/
void Renderer2D::load()
{
	load_vertex();
	s2d.compile2d("shader/vertex2d.shader","shader/fragment2d.shader");
	load_texture();
}

/*
	load_wcam(Camera2D*) -> void
	cam2d: the 2D camera used to render the loaded sprites and animations in relation to
	purpose: additionally to the features of load() the view and projection matrices are uploaded
*/
void Renderer2D::load(Camera2D* cam2d)
{
	load();
	s2d.upload_camera(*cam2d);
}

/*
	prepare() -> void
	purpose: prepare shader, buffer & gl settings to render sprites and animations
*/
void Renderer2D::prepare()
{
	// setup buffer & shader
	s2d.enable();
	reset_shader();
	buffer.bind();

	// gl settings
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
}
// FIXME: which disables are necessary?

/*
	render_sprite(uint16_t,uint16_t) -> void
	b: sprite index to begin the drawing at
	e: sprite index to end the drawing at (sprite with this index is excluded from drawing)
	purpose: draw all sprites within index range, excluding range maximum e:
		the indices are given by the renderer as a return at creation
		add(...) -> [index]
*/
void Renderer2D::render_sprite(uint16_t b,uint16_t e)
{
	for (int i=b;i<e;i++) {
		s2d.upload_matrix("model",sl.at(i).model);
		glBindTexture(GL_TEXTURE_2D,sl.at(i).tex);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
	}
}

/*
	render_sprite(uint16_t,uint16_t,unsigned int) -> void
	b: sprite index to begin the drawing at
	e: sprite index to end the drawing at (sprite with this index is excluded from drawing)
	tex: texture, replacing the loaded texture before render
	purpose: same basic functionality as the normal render_sprite but with previous texture change
	DEPRECATED: begin/end method of rendering a bunch is outdated due to branch reasons
*/
void Renderer2D::render_sprite(uint16_t b,uint16_t e,unsigned int tex)
{
	for (int i=b;i<e;i++) {
		s2d.upload_matrix("model",sl.at(i).model);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
	}
}
// FIXME: change last parameter from automatically declared unsigned int

/*
	render_state(uint16_t,vec2) -> void
	s: index of the animation previously returned at creation
	i: index of animation frame to permanently render
	purpose: permanently render a specific frame of an animation
*/
void Renderer2D::render_state(uint16_t s,glm::vec2 i)
{
	glBindTexture(GL_TEXTURE_2D,al.at(s).tex);

	// upload spritesheet specifications
	s2d.upload_int("row",al.at(s).r);
	s2d.upload_int("col",al.at(s).c);
	s2d.upload_vec2("i_tex",i);

	// transform and draw
	s2d.upload_matrix("model",al.at(s).model);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((s+sl.size())*6*sizeof(int)));
}

/*
	render_anim(uint16_t) -> void
	i: animation index previously returned at creation
	purpose: render animation and automatically iterate through all textures on the spritesheet
*/
void Renderer2D::render_anim(uint16_t i)
{
	// get rows, columns and current index
	int index = al.at(i).setup();
	int r = al.at(i).r;
	int c = al.at(i).c;

	// get 2D index from address reference integer at current frame
	glm::vec2 ind = glm::vec2((int)(index%c),(int)(index/c));

	// upload spritesheet specifications
	s2d.upload_int("row",r);;
	s2d.upload_int("col",c);
	s2d.upload_vec2("i_tex",ind);

	// transform and draw
	s2d.upload_matrix("model",al.at(i).model);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sl.size())*6*sizeof(int)));
}

/*
	reset_shader() -> void
	purpose: reset all animation relevant uniform variables in shader program
*/
void Renderer2D::reset_shader()
{
	s2d.upload_int("row",1);
	s2d.upload_int("col",1);
	s2d.upload_vec2("i_tex",glm::vec2(0,0));
}

/*
	get_max_sprite() -> uint16_t
	returns: length of sprite list
*/
uint16_t Renderer2D::get_max_sprite()
{
	return sl.size();
}

/*
	get_max_anim() -> uint16_t
	returns: length of animation list
*/
uint16_t Renderer2D::get_max_anim()
{
	return al.size();
}