#include "menu_dialogue.h"

MenuDialogue::MenuDialogue() {  }

/*

*/
MenuDialogue::MenuDialogue(glm::vec2 pos,float width,float height,Renderer2D* r2d,Renderer3D* r3d,Camera2D* cam2d,
		const char* head,std::vector<const char*> paths,float ewidth,float eheight)
	: m_r2d(r2d),m_r3d(r3d)
{
	// dialogue background
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

	// selection identifier
	/*cam3d = Camera3D(glm::vec3(0,1,10),16,9,90);
	cam3d.view3D = glm::lookAt(cam3d.pos,glm::vec3(0,0,0),cam3d.up);
	m_r3d->add("res/menu/selector.obj","res/dnormal.png","res/menu/dlg_all.png","res/dnormal.png",
			"res/menu/dlg_nan.png",glm::vec3(0,0,0),1,glm::vec3(0,0,0));
	m_r3d->load(&cam3d);*/

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

	// 3D lighting of selection identifier
	/*Light3D l3d = Light3D(m_r3d,0,glm::vec3(1000,-750,-100),glm::vec3(1,1,1),1);
	l3d.upload();
	l3d.set_amnt(1);
	mat0 = Material3D(r3d,4,32,.2);*/
} MenuDialogue::~MenuDialogue() {  }

/*

*/
uint8_t MenuDialogue::stall_input(std::vector<bool*> trg_stall,bool* conf,bool* back)
{
	uint8_t out = ((*back&&!*trg_stall[1])&&open)+2*((*conf&&!*trg_stall[0])&&open);
	bool topen = open-open*((*conf&&!*trg_stall[0])||(*back&&!*trg_stall[1]));
	for (int i=0;i<trg_stall.size()*open;i++) *trg_stall[i] = true;
	open = topen;
	return out;
}

/*

*/
void MenuDialogue::render(uint8_t &index)
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

	// §§ POLICE POLICE -- NOTHING TO SEE HERE -- POLICE POLICE §§

	// render selection icosphere
	/*glEnable(GL_DEPTH_TEST);
	m_r3d->prepare_wcam(&cam3d);
	mat0.upload();
	m_r3d->upload_model(glm::rotate(glm::mat4(1.0f),glm::radians(mrot),glm::vec3(.5f,1,.2f)));
	mrot += 2;
	mrot = (int)mrot%360;
	m_r3d->render_mesh(0,1);*/

	// §§ POLICE POLICE -- OUT OF COMMISSION -- POLICE POLICE §§

	index = open;
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
