#include "../mat/toolbox.h"

std::vector<float> Toolbox::create_sprite_canvas(glm::vec2 pos,float width,float height)
{
	std::vector<float> out = {
		pos.x,pos.y+height,0.0f,0.0f,
		pos.x+width,pos.y+height,1.0f,0.0f,
		pos.x+width,pos.y,1.0f,1.0f,
		pos.x,pos.y,0.0f,1.0f
	};
	return out;
}