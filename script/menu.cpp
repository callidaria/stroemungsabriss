#include "menu.h"

Menu::Menu(Frame f,Renderer2D* r2d, Camera2D* cam2d)
	: m_r2d(r2d)
{
	const char* GVERSION = "0.0.1";

	m_r2d->add(glm::vec2(0,0),100,400,"res/title.png");
	m_r2d->add(glm::vec2(0,0),500,83,"res/en_title.png");

	Font fnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",25,25);
	Font vfnt = Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",15,15);
	tft = Text(&fnt);vtft = Text(&vfnt);
	tft.add("press START if you DARE",glm::vec2(450,250));
	vtft.add("yomisensei by callidaria. danmaku version 0.0.1 - running on cascabel 1.3.1v (OpenGL)",
			glm::vec2(630,20)); // §§FIX: hardcoded version number
	tft.load_wcam(cam2d);vtft.load_wcam(cam2d);

	glGenVertexArrays(1,&svao);glGenBuffers(1,&svbo);
	sshd = Shader();
	float sverts[] = {
		-25,0,25,0,.5f,0,0, 420,720,-25,720,.5f,0,0, 600,720,25,720,.5f,0,0, // title splash
		600,720,25,720,.5f,0,0, 50,0,160,0,.5f,0,0, -25,0,25,0,.5f,0,0,
		0,500,0,500,0,0,1, 0,500,0,550,0,0,1, 0,500,1280,600,0,0,1, // head splash
		0,500,1280,600,0,0,1, 0,500,1280,470,0,0,1, 0,500,0,500,0,0,1,
		630,0,630,0,0,1,0, 630,0,600,720,0,1,0, 650,0,665,720,0,1,0, // select splash
		650,0,665,720,0,1,0, 650,0,650,0,0,1,0, 630,0,630,0,0,1,0
	}; // ??clockwise rotation triangle hardcoded replace
	glBindVertexArray(svao); // §§??
	glBindBuffer(GL_ARRAY_BUFFER,svbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sverts),sverts,GL_STATIC_DRAW);
	sshd.compile_vCols("shader/fbv_select.shader","shader/fbf_select.shader");
	sshd.upload_matrix("view",cam2d->view2D);sshd.upload_matrix("proj",cam2d->proj2D);

	fb = FrameBuffer(f.w_res,f.h_res,"shader/fbv_menu.shader","shader/fbf_sepia.shader",false);
	splash_fb = FrameBuffer(f.w_res,f.h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
}
void Menu::render(Frame f)
{
	if (f.kb.ka[SDL_SCANCODE_BACKSPACE]&&title) title = false;
	else if (f.kb.ka[SDL_SCANCODE_RETURN]&&!title) title = true;
	if (title&&ptrans<1) ptrans+=0.1f;
	else if (!title&&ptrans>0) ptrans-=0.1f;
	pos_title = glm::translate(glm::mat4(1.0f),TITLE_START+title_dir*ptrans);
	pos_entitle = glm::translate(glm::mat4(1.0f),ENTITLE_START+entitle_dir*ptrans);

	splash_fb.bind();
	f.clear(0,0,0);
	sshd.enable();
	sshd.upload_float("ptrans",ptrans);
	glBindVertexArray(svao);glBindBuffer(GL_ARRAY_BUFFER,svbo); // §§??
	glDrawArrays(GL_TRIANGLES,0,18);
	splash_fb.close();

	fb.bind();
	f.clear(1,0.8f,0);
	m_r2d->prepare();
	m_r2d->upload_model(pos_title);m_r2d->render_sprite(0,1);
	m_r2d->upload_model(pos_entitle);m_r2d->render_sprite(1,2);

	if (!title) {
		tft.prepare();tft.render(50,glm::vec4(1,0,0,1));
		vtft.prepare();vtft.render(75,glm::vec4(0,0,0,1));
	} fb.close();f.clear(0,0,0);fb.render_wOverlay(splash_fb.get_tex());
}
