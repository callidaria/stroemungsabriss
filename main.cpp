#define GLM_ENABLE_EXPERIMENTAL 1
#include <iostream>
#include <glm/gtx/rotate_vector.hpp>

#ifdef __WIN32__
#include <windows.h>
#endif

#include "ccb/frm/frame.h"
#include "ccb/gfx/renderer2d.h"
#include "ccb/gfx/renderer3d.h"
#include "ccb/gfx/rendereri.h"
#include "ccb/mat/camera2d.h"
#include "ccb/mat/camera3d.h"
#include "ccb/ppe/msaa.h"
#include "ccb/ppe/bloom.h"
#include "ccb/fcn/text.h"
#include "ccb/fcn/init.h"
#include "ccb/aud/audio.h"
#include "ccb/aud/listener.h"
#include "ccb/fcn/ccb_manager.h"
#include "ccb/gfx/light3d.h"
#include "ccb/gfx/material3d.h"
#include "script/menu.h"

#define MVMT_SPEED 4
#define MENU_RENDER 1
#define BUILD_DEV_MODE 1

int main(int argc,char** argv)
{
	Init init = Init("config.ini");
	Frame f = Frame("黄泉先生",init.rINT(init.FRAME_DISPLAY_ID),init.rINT(init.FRAME_RESOLUTION_WIDTH),
		init.rINT(init.FRAME_RESOLUTION_HEIGHT),(SDL_WindowFlags)init.rINT(init.FRAME_SET_FULLSCREEN));

	// AUDIO
	Listener listener=Listener();

	// RENDERERS
	Renderer2D r2d = Renderer2D();
	Renderer3D r3d = Renderer3D();
	RendererI ri = RendererI();
	Camera2D cam2d = Camera2D(1280.0f,720.0f);
	Camera3D cam3d = Camera3D(glm::vec3(.1f,-.1f,1.5f),1280.0f,720.0f,45.0f);

	bool dactive = false;
#if MENU_RENDER
	CCBManager ccbm = CCBManager(&f,&r2d,&cam2d);
	Menu menu = Menu(&ccbm,&f,&r2d,&r3d,&ri,&cam2d,&cam3d);
#else
	r2d.add(glm::vec2(0,0),50,50,"./res/flyfighter.png");
	r2d.add(glm::vec2(0,0),50,50,"./res/flyfighter.png");
	r2d.add(glm::vec2(0,0),50,50,"./res/flyfighter_hit.png");
	r2d.add(glm::vec2(0,0),75,720,"./res/blast_wall.png");
	r2d.add(glm::vec2(1205,0),75,720,"./res/blast_wall.png");
	r2d.add(glm::vec2(12.5f,-5),25,25,"./res/gatling.png");

	ri.add(glm::vec2(0,0),25,25,"./res/bllt_norm.png");
	ri.add(glm::vec2(0,0),20,20,"./res/bllt_proj.png");
	ri.add(glm::vec2(0,0),10,10,"./res/fast_bullet.png");
	ri.add(glm::vec2(0,0),10,10,"./res/fast_bullet.png");
	ri.add(glm::vec2(0,0),20,20,"./res/norm_shot.png");

	// GAME
	int money = 100000000;
	glm::vec2 ppos = glm::vec2(615,10);
	int shot_clear = 0; glm::vec2 shot_dir[1000] = { glm::vec2(0) }; int shot_idx = 0;
	glm::vec2 chett_pos = glm::vec2(615,400);
	float ex = 0; int state = 0; int mult = -1;
	int bullets = 0; int blt_clear = 4;
	int projectiles = 0; int proj_clear = 2; glm::vec2 bllt_dir[1000] = { glm::vec2(0) };
	int fbs[] = { 0,0 }; int fbs_clear = 1; glm::vec2 fbs_dir[2000] = { glm::vec2(0) }; bool swap_idx = false;
	glm::vec2 md = glm::vec2(0,-1); glm::mat4 mmd = glm::mat4(1.0f); int chmult = 1; int cutback = 4;

	// TEXT
	Font fnt=Font("res/fonts/nimbus_roman.fnt","res/fonts/nimbus_roman.png",20,20);
	Text tft=Text(&fnt);
	tft.add("Chett, The Dancing Pilot",glm::vec2(10,710));
	std::string cnv_money = std::to_string(money);
	tft.add(cnv_money.c_str(),glm::vec2(1175,710));
#endif

	// CAMERAS
	r2d.load(&cam2d);
	ri.load(&cam2d);
	uint32_t run=1,pause=false;
	bool reboot = false;
	glm::mat4 model = glm::mat4(1.0f);
	while (run) {
		f.print_fps();f.input(run,dactive);

		// INPUT
		if (f.kb.ka[SDL_SCANCODE_ESCAPE]) break;

#if MENU_RENDER
		menu.render(run,reboot);
#if BUILD_DEV_MODE
		ccbm.dev_console(run,dactive);
#endif
#else
		// ENEMY
		if (state==0) { // use vector function later
			if (ex>=600||ex<=-600) state++;
			else { chett_pos = glm::vec2(615+ex,650+mult*20000/(ex/2-100*mult)+50);ex-=4*mult; }
			if (proj_clear==0&&projectiles<1000) {
				ri.set_offset(1,projectiles,glm::vec2(chett_pos.x+25,chett_pos.y-25));
				glm::vec4 pnv = glm::vec4(0,-1,0,0)*glm::rotate(glm::mat4(1.0f),
						glm::radians((ex/615*90.0f)+(-3+rand()%7)),glm::vec3(0,0,1));
				bllt_dir[projectiles] = glm::vec2(pnv.x,pnv.y);
				projectiles++;
				proj_clear = rand()%2;
			} else proj_clear--;
		} else if (state==1) {
			if (ex<=0&&mult==-1) { state=0;mult=1; }
			else if (ex>=0&&mult==1) { state=0;mult=-1; }
			else { chett_pos = glm::vec2(615+ex,650+ex*ex/2400-150);ex+=8*mult; }
			if (blt_clear==0&&bullets<1000) {
				ri.set_offset(0,bullets,glm::vec2(chett_pos.x+25,chett_pos.y-25));
				bullets++;
				blt_clear = rand()%5;
			} else blt_clear--;
		} if (fbs[swap_idx]>=1000) { swap_idx=!swap_idx;fbs[swap_idx]=0; }
		if (fbs_clear==0) {
			for (int i=-2;i<3;i++) {
				glm::vec4 pnv = glm::vec4(0,-1,0,0)*mmd*glm::rotate(glm::mat4(1.0f),
						glm::radians(i*5.0f),glm::vec3(0,0,1));
				fbs_dir[swap_idx*1000+fbs[swap_idx]] = glm::vec2(pnv.x,pnv.y);
				ri.set_offset(2+swap_idx,fbs[swap_idx],
						chett_pos+fbs_dir[swap_idx*1000+fbs[swap_idx]]*glm::vec2(20)
						+glm::vec2(20,0));
				fbs[swap_idx]++;
			} fbs_clear = 3;
			if (cutback==0) { cutback=8;chmult*=-1; }
			mmd = glm::rotate(mmd,glm::radians(chmult*15.0f),glm::vec3(0,0,1));
			cutback--;
		} else fbs_clear--;
		glm::vec2 rpos = ppos+glm::vec2(25,25);
		for (int i=0;i<bullets;i++) {
			ri.il.at(0).o[i].y -= .5f;
			float dist = glm::length((ri.il.at(0).o[i]+glm::vec2(12.5f,12.5f))-rpos);
			if (dist<10) coll = true;
		} for (int i=0;i<projectiles;i++) {
			ri.il.at(1).o[i] += glm::vec2(2)*bllt_dir[i];
			float dist = glm::length((ri.il.at(1).o[i]+glm::vec2(10,10))-rpos);
			if (dist<10) coll = true;
		} for (int j=0;j<2;j++) {
			for (int i=0;i<fbs[j];i++) {
				ri.il.at(2+j).o[i] += glm::vec2(3)*fbs_dir[j*1000+i];
				float dist = glm::length((ri.il.at(2+j).o[i]+glm::vec2(5,5))-rpos);
				if (dist<4) coll = true;
			}
		} for (int i=0;i<shot_idx;i++) ri.il.at(4).o[i] += glm::vec2(8)*shot_dir[i];

		// RENDER
		if (pause) bgr_pause.bind();
		f.clear(0.1f,0.1f,0.1f);

		r2d.prepare();
		r2d.upload_model(glm::translate(glm::mat4(1.0f),glm::vec3(ppos.x,ppos.y,0.0f)));
		r2d.render_sprite(0,1);
		r2d.render_sprite(2,2+coll);
		glm::mat4 emod = glm::translate(glm::mat4(1.0f),glm::vec3(chett_pos.x,chett_pos.y,0.0f));
		r2d.upload_model(emod);
		r2d.render_sprite(1,2);
		r2d.upload_model(emod*glm::translate(glm::mat4(1.0f),glm::vec3(25,15,0))*mmd
				*glm::translate(glm::mat4(1.0f),glm::vec3(-25,-15,0)));
		r2d.render_sprite(5,6);
		r2d.upload_model(glm::mat4(1.0f));
		r2d.render_sprite(3,5);

		ri.prepare();
		ri.render(0,bullets);
		ri.render(1,projectiles);
		ri.render(2,fbs[0]);ri.render(3,fbs[1]);
		ri.render(4,shot_idx);

		tft.prepare();
		tft.render(92,glm::vec4(1,1,1,1));

		if (pause) { bgr_pause.close();f.clear(0,0,0);bgr_pause.render(); }
#endif
		f.update();
	}

	if (reboot)
#ifdef __WIN32__
		ShellExecute(NULL,NULL,"yomisensei.exe",NULL,NULL,SW_SHOW);
#else
		system("./yomisensei &");
#endif

	f.vanish();
	return 0;
}
