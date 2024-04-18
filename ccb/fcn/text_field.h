#ifndef CCB_FEATURE_TEXTFIELD
#define CCB_FEATURE_TEXTFIELD

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "../frm/frame.h"
#include "../gfx/renderer.h"
#include "font.h"
#include "text.h"

class TextField
{
public:

	// construction
	TextField(Font* fnt,Renderer* rnd,glm::vec2 pos,const char* tp,glm::vec2 bpos,float width,float height);

	// draw
	void render(Frame* frame,glm::vec4 col);

private:

	Text txt;
	Renderer* m_rnd;
	std::string cnt;
	glm::vec2 m_pos;
	glm::vec2 m_bpos;
	float m_width,m_height;
	int char_amount = 0;int rinst;
	bool input_active = false;
};

#endif