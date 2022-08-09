#include "../gfx/renderer2d.h"

/*
	constructor()
	[...]
*/
Renderer2D::Renderer2D()
{
	buffer.add_buffer();
} Renderer2D::~Renderer2D() {  }

/*
	add(vec2,float,float,const char*) -> uint16_t
	[...]
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
	[...]
*/
uint16_t Renderer2D::add(glm::vec2 p,float w,float h,const char* t,uint8_t r,uint8_t c,
		uint8_t f,uint8_t itn)
{
	Anim tmp = Anim(p,w,h,t,r,c,f,itn);  // TODO: fall into output while push back also
	uint16_t out = al.size();
	al.push_back(tmp);
	return out;
}

/*
	load_vertex() -> void
	[...]
*/
void Renderer2D::load_vertex()
{
	// getting list sizes
	uint16_t ls = sl.size();
	uint16_t la = al.size();

	// create write arrays
	float v[(ls+la)*16];
	unsigned int e[(ls+la)*6];

	// write values to arrays
	for (int j=0;j<ls+la;j++) {
		for (int i=0;i<16;i++) {
			if (j<ls)
				v[j*16+i] = sl.at(j).v[i];
			else
				v[j*16+i] = al.at(j-ls).v[i];
		}

		// create elements for element buffer
		e[j*6] = j*4;
		e[j*6+1] = j*4+1;
		e[j*6+2] = j*4+2;
		e[j*6+3] = j*4+2;
		e[j*6+4] = j*4+3;
		e[j*6+5] = j*4+0;
	}

	// upload to buffers
	buffer.bind();
	buffer.upload_vertices(v,sizeof(v));
	buffer.upload_elements(e,sizeof(e));
}
// FIXME: this can be way faster, what about the array -> vector && branching removal?

/*
	load_texture() -> void
	[...]
*/
void Renderer2D::load_texture()
{
	for (int i=0;i<sl.size();i++)
		sl.at(i).texture();
	for (int i=0;i<al.size();i++)
		al.at(i).texture();
}

/*
	load() -> void
	[...]
*/
void Renderer2D::load()
{
	load_vertex();
	s2d.compile2d("shader/vertex2d.shader","shader/fragment2d.shader");
	load_texture();
}

/*
	load_wcam(Camera2D*) -> void
	[...]
*/
void Renderer2D::load_wcam(Camera2D* c)
{
	load();
	upload_view(c->view2D);
	upload_proj(c->proj2D);
}

/*
	prepare() -> void
	[...]
*/
void Renderer2D::prepare()
{
	s2d.enable();
	reset_shader();
	buffer.bind();
	glDisable(GL_CULL_FACE);
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
}
// FIXME: which diables are necessary?

/*
	render_sprite(uint16_t,uint16_t) -> void
	[...]
*/
void Renderer2D::render_sprite(uint16_t b,uint16_t e)
{
	for (int i=b;i<e;i++) {
		upload_model(sl.at(i).model); // ??how expensive exactely
		glBindTexture(GL_TEXTURE_2D,sl.at(i).tex);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
	}
}

/*
	render_sprite(uint16_t,uint16_t,unsigned int) -> void
	[...]
*/
void Renderer2D::render_sprite(uint16_t b,uint16_t e,unsigned int tex)
{
	for (int i=b;i<e;i++) {
		upload_model(sl.at(i).model);
		glBindTexture(GL_TEXTURE_2D, tex);
		glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)(i*6*sizeof(int)));
	}
}

/*
	render_state(uint16_t,vec2) -> void
	[...]
*/
void Renderer2D::render_state(uint16_t s,glm::vec2 i)
{
	glBindTexture(GL_TEXTURE_2D,al.at(s).tex);

	// upload spritesheet specifications
	upload_row(al.at(s).r);
	upload_col(al.at(s).c);
	upload_tindex(i);

	// transform and draw
	upload_model(al.at(s).model);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((s+sl.size())*6*sizeof(int)));
}

/*
	render_anim(uint16_t) -> void
	[...]
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
	upload_row(r);
	upload_col(c);
	upload_tindex(ind);

	// transform and draw
	upload_model(al.at(i).model);
	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,(void*)((i+sl.size())*6*sizeof(int)));
}

/*
	reset_shader() -> void
	[...]
*/
void Renderer2D::reset_shader()
{
	upload_row(1);
	upload_col(1);
	upload_tindex(glm::vec2(0,0));
}

/*
	upload_model(mat4) -> void
	[...]
*/
void Renderer2D::upload_model(glm::mat4 m)
{
	s2d.upload_matrix("model",m);
}

/*
	upload_view(mat4) -> void
	[...]
*/
void Renderer2D::upload_view(glm::mat4 m)
{
	s2d.upload_matrix("view",m);
}

/*
	upload_proj(mat4) -> void
	[...]
*/
void Renderer2D::upload_proj(glm::mat4 m)
{
	s2d.upload_matrix("proj",m);
}

/*
	upload_row(uint8_t) -> void
	[...]
*/
void Renderer2D::upload_row(uint8_t i)
{
	s2d.upload_int("row",i);
}

/*
	upload_col(uint8_t) -> void
	[...]
*/
void Renderer2D::upload_col(uint8_t i)
{
	s2d.upload_int("col",i);
}

/*
	upload_tindex(vec2) -> void
	[...]
*/
void Renderer2D::upload_tindex(glm::vec2 v)
{
	s2d.upload_vec2("i_tex",v);
}

/*
	get_max_sprite() -> uint16_t
	[...]
*/
uint16_t Renderer2D::get_max_sprite()
{
	return sl.size();
}

/*
	get_max_anim() -> uint16_t
	[...]
*/
uint16_t Renderer2D::get_max_anim()
{
	return al.size();
}