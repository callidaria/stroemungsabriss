#ifndef SCR_INTERFACE_CURSOR
#define SCR_INTERFACE_CURSOR

#include "../../ccb/core.h"

class Cursor
{
public:

	Cursor();
	void render();

public:

	glm::vec2 position;

private:

	// draw
	uint16_t rindex;

	// animation
	float bgr_crot = 0,front_crot = 0;
};

#endif