#include "menu_dialogue.h"

MenuDialogue::MenuDialogue() {  }

/*

*/
MenuDialogue::MenuDialogue(glm::vec2 pos,float width,float height,Renderer2D* r2d,Camera2D* cam2d,
		const char* head,std::vector<const char*> paths,float ewidth,float eheight)
	: m_r2d(r2d)
{
	// dialogue backround
	glGenVertexArrays(1,&bvao);
	glGenBuffers(1,&bvbo);
	m_sh = Shader();
	float border_verts[] = {
		pos.x,pos.y,pos.x,pos.y+height,pos.x+width,pos.y+height,	// 1st TRIANGLE
		pos.x+width,pos.y+height,pos.x+width,pos.y,pos.x,pos.y		// 2nd TRIANGLE
	};
	glBindVertexArray(bvao);
	glBindBuffer(GL_ARRAY_BUFFER,bvbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(border_verts),border_verts,GL_STATIC_DRAW);
	m_sh.compile_mDlg("shader/fbv_mdialogue.shader","shader/fbf_mdialogue.shader");
	m_sh.upload_matrix("view",cam2d->view2D);
	m_sh.upload_matrix("proj",cam2d->proj2D);

	// TODO: dialogue border

	// dialogue text heading
	Font thfont = Font("./res/fonts/nimbus_roman.fnt","./res/fonts/nimbus_roman.png",20,20);
	thead = Text(thfont);
	thead.add(head,pos+glm::vec2(15,height-15));
	thead.load_wcam(cam2d);

	// selection entities
	irnd = m_r2d->sl.size();
	srnd = paths.size();
	int32_t blank = (width-srnd*ewidth)/srnd+ewidth;
	for (int i=0;i<srnd;i++) m_r2d->add(pos+glm::vec2((blank-ewidth)/2+i*blank,15),ewidth,eheight,paths[i]);
} MenuDialogue::~MenuDialogue() {  }

/*

*/
bool MenuDialogue::stall_input(std::vector<bool*> trg_stall,bool* conf,bool* back)
{
	bool out = *conf&&open;
	open -= open*((*conf&&!*trg_stall[0])||(*back&&!*trg_stall[1]));
	for (int i=0;i<trg_stall.size()*open;i++) *trg_stall[i] = true;
	return out;
}

/*

*/
void MenuDialogue::render()
{
	// render dialogue background
	m_sh.enable();
	glBindVertexArray(bvao);
	glDrawArrays(GL_TRIANGLES,0,6*open);

	// TODO: render border
	// TODO: handle selections

	// render dialogue head
	thead.prepare();
	thead.render(128*open,glm::vec4(1,1,1,1));

	// render selection entities
	m_r2d->prepare();
	m_r2d->render_sprite(irnd,irnd+srnd*open);
}

/*

*/
void MenuDialogue::open_dialogue() { open = true; }

/*

*/
uint8_t MenuDialogue::hit_dialogue(bool confirm)
{
	open = false;
	return 0;
}
