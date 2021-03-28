#include "menu.h"

Menu::Menu(Frame* f,Renderer2D* r2d, Camera2D* cam2d)
	: m_r2d(r2d)
{
	const char* GVERSION = "0.0.1";

	CCBLInterpreter ccbl = CCBLInterpreter(r2d,nullptr,"lvload/menu.ccb");
	msindex = ccbl.load_level();

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
		// title splash
		-25,0,25,0,.5f,0,0, 420,720,-25,720,.5f,0,0, 600,720,25,720,.5f,0,0,
		600,720,25,720,.5f,0,0, 50,0,160,0,.5f,0,0, -25,0,25,0,.5f,0,0,
		// head splash
		0,500,0,500,.245f,.606f,.564f, 0,500,0,550,.245f,.606f,.564f, 0,500,1280,600,.245f,.606f,.564f,
		0,500,1280,600,.245f,.606f,.564f, 0,500,1280,470,.245f,.606f,.564f, 0,500,0,500,.245f,.606f,.564f,
		// select splash
		630,0,630,0,.341f,.341f,.129f, 630,0,600,720,.341f,.341f,.129f, 650,0,665,720,.341f,.341f,.129f,
		650,0,665,720,.341f,.341f,.129f, 650,0,650,0,.341f,.341f,.129f, 630,0,630,0,.341f,.341f,.129f
	}; // ??clockwise rotation triangle hardcoded replace
	glBindVertexArray(svao); // §§??
	glBindBuffer(GL_ARRAY_BUFFER,svbo);
	glBufferData(GL_ARRAY_BUFFER,sizeof(sverts),sverts,GL_STATIC_DRAW);
	sshd.compile_vCols("shader/fbv_select.shader","shader/fbf_select.shader");
	sshd.upload_matrix("view",cam2d->view2D);sshd.upload_matrix("proj",cam2d->proj2D);

	fb = FrameBuffer(f->w_res,f->h_res,"shader/fbv_menu.shader","shader/fbf_menu.shader",false);
	splash_fb = FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
	title_fb = FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);
	select_fb = FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader",false);

	if (f->m_gc.size()>0) {
		cnt_b = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_B];
		cnt_start = &f->xb.at(0).xbb[SDL_CONTROLLER_BUTTON_START];
	} else {
		cnt_b = &f->kb.ka[SDL_SCANCODE_BACKSPACE];
		cnt_start = &f->kb.ka[SDL_SCANCODE_RETURN];
	}

	ccbl.edit_level("lvload/menu.ccb",0,glm::vec2(0,0),0,0,"newlines");
}
void Menu::render(Frame f,bool &running)
{
	if (*cnt_b&&title) title = false;
	else if (*cnt_start&&!title) title = true;

	if (title&&ptrans<1) ptrans+=0.1f;
	else if (!title&&ptrans>0) ptrans-=0.1f;
	pos_title = glm::translate(glm::mat4(1.0f),TITLE_START+title_dir*ptrans);
	pos_entitle = glm::translate(glm::mat4(1.0f),ENTITLE_START+entitle_dir*ptrans);

	sshd.enable();
	sshd.upload_float("ptrans",ptrans);
	glBindVertexArray(svao);glBindBuffer(GL_ARRAY_BUFFER,svbo); // §§??
	splash_fb.bind();f.clear(0,0,0);glDrawArrays(GL_TRIANGLES,0,6);splash_fb.close();
	title_fb.bind();f.clear(0,0,0);glDrawArrays(GL_TRIANGLES,6,6);title_fb.close();
	select_fb.bind();f.clear(0,0,0);glDrawArrays(GL_TRIANGLES,12,6);select_fb.close();

	fb.bind();
	f.clear(0,0,0);
	m_r2d->prepare();
	m_r2d->s2d.upload_float("ptrans",ptrans);
	m_r2d->sl.at(msindex).model = pos_title;m_r2d->sl.at(msindex+1).model = pos_entitle;
	m_r2d->render_sprite(msindex,msindex+2);

	if (title) {
		for (int i=2;i<8;i++) m_r2d->sl.at(msindex+i).scale(ptrans,ptrans);
		m_r2d->render_sprite(msindex+2,msindex+8);
	} else {
		tft.prepare();tft.render(50,glm::vec4(1,0,0,1));
		vtft.prepare();vtft.render(75,glm::vec4(0,0,.5f,1));
	} fb.close();f.clear(0,0,0);
	fb.render_wOverlay(splash_fb.get_tex(),title_fb.get_tex(),select_fb.get_tex(),ptrans);
}
