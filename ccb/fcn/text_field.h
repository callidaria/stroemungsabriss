#ifndef CCB_FEATURE_TEXTFIELD
#define CCB_FEATURE_TEXTFIELD

#include "../core.h"
#include "font.h"
#include "text.h"

class TextField
{
public:

	// construction
	TextField(Font* fnt,glm::vec2 pos,const char* tp,glm::vec2 bpos,float width,float height);

	// draw
	void render(glm::vec4 col);

private:

	Text txt;
	std::string cnt;
	glm::vec2 m_pos;
	glm::vec2 m_bpos;
	float m_width,m_height;
	int char_amount = 0;int rinst;
	bool input_active = false;
};

#endif