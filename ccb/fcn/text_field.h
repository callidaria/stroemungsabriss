#pragma once

#include <iostream>
#include <string>
#include <glm/glm.hpp>
#include "../frm/frame.h"
#include "../gfx/renderer2d.h"
#include "font.h"
#include "text.h"

class TextField
{
public:
	TextField(Font* fnt,Renderer2D* r2d,glm::vec2 pos,const char* tp,glm::vec2 bpos,float width,float height);
	void render(Frame* frame,Camera2D* cam2d,glm::vec4 col);
private:
	Text txt;
	Renderer2D* m_r2d;
	std::string cnt;
	glm::vec2 m_pos;
	glm::vec2 m_bpos;
	float m_width,m_height;
	int char_amount = 0;int rinst;
	bool input_active = false;
};
