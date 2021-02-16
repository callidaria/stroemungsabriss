#pragma once

#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <glm/glm.hpp>
#include "renderer2d.h"

class Button
{
public:
	Button(Renderer2D* ir,glm::vec2 ip, float iw, float ih, const char* dir, bool inext)
		: r(ir), p(ip), w(iw), h(ih), next(inext)
	{
		ri = r->get_max_anim(); ti = r->get_max_sprite(); c = false;
		
		std::vector<const char*> t;
		t.push_back("res/colour/deselect.png");
		t.push_back("res/colour/select.png");
		r->add(p,w,h,t,0);

		sdir = strlen(dir);
		for (int i = 0; i < sdir; i++) {
			const char* path = "res/alpha/";
			const char* cpath = addend(path,dir[i]);
			r->add(glm::vec2((iw/2+ip.x)-((sdir/2.0f)*20-i*20+3),
						ip.y+5),25,25,cpath);
		} if (next) { r->add(glm::vec2(p.x+w,p.y+5),
				30,30,"res/alpha/>.png"); sdir++; }
	}
	void render(int mx, int my, bool mc)
	{
		if (mx<p.x+w&&mx>p.x&&my<720-p.y&&my>720-(p.y+h)) 
		{
			if (mc) c = true;
			r->render_state(ri,1);
		}
		else r->render_state(ri,0);
		r->render_sprite(ti,ti+sdir);
	}
	bool get_trigger() { return c; }
	void retrigger() { c = false; }
private:
	const char* addend(const char* as, char a)
	{
		char* o = charadd(as,a);o = charadd(o,'.');
		o = charadd(o,'p');o = charadd(o,'n');o = charadd(o,'g');
		return o;
	}
	char* charadd(const char* as, char a)
	{
		size_t s = strlen(as); char* o = new char[s+2];
		strcpy(o,as); o[s] = a; o[s+1] = '\0';
		return o;
	}
private:
	Renderer2D* r;
	glm::vec2 p; float w, h;
	int ri,ti; size_t sdir;
	bool c, next;
};
