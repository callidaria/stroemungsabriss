#include "menu_dialogue.h"

/*
	Constructor(void)
	purpose: empty constuctor to build dialogue before constructing it for real.
	DEPRECATED: this is a bad way and not even necessary. please remove and fix instances.
*/
MenuDialogue::MenuDialogue() {  }

/*
	Constructor(glm::vec2,float,float,Renderer2D*,Camera2D*,const char*,std::vector<const char*>,float,float)
	pos: position of the dialogue window on screen
	width: width of the dialogue window
	height: height of the dialogue window
	r2d: renderer to handle the dialogue visuals
	cam2d: camera to define the coordinate system, perspective and the camera relative position
	head: title headline of dialogue screen
	paths: list of textures, representing the selectable options
	ewidth: entity width of selectable options inside dialogue window
	eheight: entity height of selectable options inside dialogue window
	purpose: constructor sets up a dialogue window in menu, to give player options to choose from.
*/
MenuDialogue::MenuDialogue(glm::vec2 pos,float width,float height,Renderer2D* r2d,Camera2D* cam2d,
		const char* head,std::vector<const char*> paths,float ewidth,float eheight)
	: m_r2d(r2d)
{
	Buffer buffer;
	m_sh = Shader();
	float border_verts[] = {
		pos.x,pos.y,pos.x,pos.y+height,pos.x+width,pos.y+height,	// 1st TRIANGLE
		pos.x+width,pos.y+height,pos.x+width,pos.y,pos.x,pos.y		// 2nd TRIANGLE
	};
	buffer.bind();
	buffer.upload_vertices(border_verts,sizeof(border_verts));
	m_sh.compile("shader/fbv_mdialogue.shader","shader/fbf_mdialogue.shader");
	m_sh.def_attributeF("position",2,0,2);
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
	render(uint8_t&) -> void
	index: current player selection index (subject to constant change)
	purpose: render all visual components of the menu dialogue feature
*/
void MenuDialogue::render(uint8_t &index)
{
	// setup selection state and interpret openness
	sstate = index*open;
	sstate += (!sstate&&open)-(sstate>srnd);

	// render dialogue background
	m_sh.enable();
	buffer.bind();
	glDrawArrays(GL_TRIANGLES,0,6*open);

	// TODO: render border

	// render dialogue head
	thead.prepare();
	thead.render(128*open,glm::vec4(1,1,1,1));

	// render selection entities
	m_r2d->prepare();
	m_r2d->render_sprite(irnd,irnd+srnd*open);

	index = sstate;
}

/*
	stall_input(std::vector<bool*>,bool*,bool*) -> uint8_t
	trg_stall: list of all input trigger pointers to stall while open
		trg_stall[0]: must be trigger of confirmation button
		trg_stall[1]: must be trigger of cancel button
	conf: confirmation button pointer to receive confirmation of players choice
	back: closing button pointer to be able to close the dialogue and the stalling process
	purpose: as long as the dialogue is open the input regarding the background has to be stalled.
		if open: all inputs will be overwritten, menu is locked for all user inputs
			& dialogue is recieving left/right and confirm/close inputs.
		if closed: all inputs are allowed, menu in bahaving normally.
	return:
		0: no decision has been made
		1: close the dialogue, abort decision
		2: close the dialogue, confirm last selected decision
*/
uint8_t MenuDialogue::stall_input(std::vector<bool*> trg_stall,bool* conf,bool* back)
{
	uint8_t out = ((*back&&!*trg_stall[1])&&open)+2*((*conf&&!*trg_stall[0])&&open);  // check for decision
	bool topen = open-open*((*conf&&!*trg_stall[0])||(*back&&!*trg_stall[1]));  // check if still open
	for (int i=0;i<trg_stall.size()*open;i++) *trg_stall[i] = true;  // stall for all triggers, if open
	open = topen;  // set updated openness
	return out;
}	// FIXME: is it possible to achieve a 3 in decision, and what does it result in?

/*
	open_dialogue() -> void
	close() -> void
	purpose: opens and closes the dialogue according to the names of the methods
	DEPRECATED: join dialogue opening and closing
*/
void MenuDialogue::open_dialogue() { open = true; }
void MenuDialogue::close() { open = false; }

/*
	hit_dialogue(bool) -> uint8_t
	confirm: contains information if dialogue has been confirmed
	purpose: this was a placeholder with no real function yet.
	DEPRECATED: join this function with existing dialogue opening and closing.
		also figure out why this method was created.
*/
uint8_t MenuDialogue::hit_dialogue(bool confirm)
{
	open = false;
	return 0;
}
