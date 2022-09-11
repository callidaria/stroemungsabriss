#include "../fcn/text.h"

Text::Text() { } // !!get rid of this pls
Text::Text(Font f)
	: font(f)
{
	buffer = Buffer();
	buffer.add_buffer();
}
int Text::add(char c,glm::vec2 p) // !!passing x increment like this is very bad pracice with public method
{
	// identifying sprite sheet position
	int i = 0;
	while (i<96) { // ??maybe alternate iteration until correct index that is more performant
		if (font.id[i]==(int)c) break;
		i++;
	}

	// character information write
	ibv.push_back(p.x);ibv.push_back(p.y);ibv.push_back(font.x[i]);ibv.push_back(font.y[i]);
	ibv.push_back(font.wdt[i]);ibv.push_back(font.hgt[i]);ibv.push_back(font.xo[i]);ibv.push_back(font.yo[i]);

	return font.xa[i]*(font.mw/83.0f); // ??do this with a vec2 pointer maybe & also with dynamic texdiv
}
void Text::add(const char* s,glm::vec2 p)
{
	for (int i=0;i<strlen(s);i++) {
		if (s[i]!=' ') p.x += add(s[i],p);
		else p.x += 57.0f*(font.mw/83.0f);
	}
}
void Text::clear() { ibv.clear(); }
void Text::load_vertex() // !!no need to have this extra public vertex load function
{
	buffer.bind();
	buffer.upload_vertices(font.v,sizeof(font.v));
}
void Text::load(Camera2D* c)
{
	load_vertex();
	sT.compile2d("shader/vertex_text.shader","shader/fragment_text.shader");
	buffer.bind_index();
	sT.def_indexF(buffer.get_indices(),"offset",2,0,8);
	sT.def_indexF(buffer.get_indices(),"texpos",2,2,8);
	sT.def_indexF(buffer.get_indices(),"bounds",2,4,8);
	sT.def_indexF(buffer.get_indices(),"cursor",2,6,8);
	font.texture();
	sT.upload_float("wdt",font.mw);
	sT.upload_matrix("view",c->view2D); // !!please use a presetted camera matrix with static viewport for text
	sT.upload_matrix("proj",c->proj2D);
}
void Text::prepare()
{
	sT.enable();
	buffer.bind();
	glDisable(GL_CULL_FACE); // !!define standard flagging by majority usage
	glDisable(GL_DEPTH_TEST);
	glActiveTexture(GL_TEXTURE0);
}
void Text::render(int amnt,glm::vec4 col)
{
	sT.upload_vec4("colour",col); // ??shader uploads outside of prepare function
	glBindTexture(GL_TEXTURE_2D,font.tex);
	buffer.upload_indices(ibv);
	// !!buffer data call in main loop ´°___°`
	glDrawArraysInstanced(GL_TRIANGLES,0,6,amnt);
}
void Text::set_scroll(glm::mat4 model) { sT.upload_matrix("model",model); }
