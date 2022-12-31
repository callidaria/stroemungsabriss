#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../../ccb/gfx/shader.h"
#include "../../ccb/gfx/renderer2d.h"

#include "../../ccb/mat/camera2d.h"

#include "../../ccb/fcn/buffer.h"
#include "../../ccb/fcn/font.h"
#include "../../ccb/fcn/text.h"

class MenuDialogue
{
public:

	// construction
	MenuDialogue(glm::vec2 pos,float width,float height,Renderer2D* r2d,Camera2D* cam2d,
		const char* head,std::vector<const char*> paths,float ewidth,float eheight);
	~MenuDialogue();

	// visuals
	void render(uint8_t &index);

	// dialogue state modification
	uint8_t stall_input(std::vector<bool*> trg_stall,bool* conf,bool* back);
	void open_dialogue();

private:

	// essentials
	Buffer buffer;		// buffer for background
	Shader m_sh;		// shader
	Renderer2D* m_r2d;	// 2d renderer reference

	// dialogue state variables
	uint8_t sstate = 0;		// selection state of players choosing
	bool open = false;		// dialogue is opened

	// text
	Font font = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",20,20);
	Text thead = Text(font);

	// selection entities
	uint32_t irnd;	// render index
	uint8_t srnd;	// size read from point of index
};