#include <iostream>
#include <vector>

#include <glm/glm.hpp>

#include "../ccb/gfx/shader.h"
#include "../ccb/gfx/renderer2d.h"
#include "../ccb/gfx/renderer3d.h"
#include "../ccb/mat/camera2d.h"
#include "../ccb/mat/camera3d.h"
#include "../ccb/gfx/light3d.h"
#include "../ccb/gfx/material3d.h"
#include "../ccb/fcn/font.h"
#include "../ccb/fcn/text.h"

class MenuDialogue
{
public:
	MenuDialogue();
	MenuDialogue(glm::vec2 pos,float width,float height,Renderer2D* r2d,Renderer3D* r3d,Camera2D* cam2d,
		const char* head,std::vector<const char*> paths,float ewidth,float eheight);
	~MenuDialogue();
	uint8_t stall_input(std::vector<bool*> trg_stall,bool* conf,bool* back);
	void render();
	void open_dialogue();
	uint8_t hit_dialogue(bool confirm);
private:
	// essentials
	Shader m_sh;			// shader
	Renderer2D* m_r2d;		// 2d renderer reference
	Renderer3D* m_r3d;		// 3d renderer reference
	Camera3D cam3d;
	Material3D mat0;
	unsigned int bvao,bvbo;		// vertex array & vertex buffer for background
	bool open = false;		// dialogue is opened

	// head dialogue
	Text thead;			// headings text

	// selection entities
	uint32_t irnd;			// render index
	uint8_t srnd;			// size read from point of index
};
