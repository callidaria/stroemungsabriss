#pragma once

#include <iostream>
#include "../frm/frame.h"
#include "../gfx/shader.h"
#include "../frm/framebuffer.h"
#include "../ppe/blur.h"
#include "../fcn/buffer.h"

class Bloom
{
public:
	Bloom() {  }
	Bloom(Frame* fr)
		: f(fr)
	{
		float verts[] = {
			-1.0f,1.0f,0.0f,1.0f,
			-1.0f,-1.0f,0.0f,0.0f,
			1.0f,-1.0f,1.0f,0.0f,
			-1.0f,1.0f,0.0f,1.0f,
			1.0f,-1.0f,1.0f,0.0f,
			1.0f,1.0f,1.0f,1.0f
		};
		buffer = Buffer();
		buffer.bind();
		buffer.upload_vertices(verts,sizeof(verts));
		sfb.compile2d("shader/fbv_standard.shader","shader/fbf_bloom.shader");

		fb=FrameBuffer(f->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_greyscale.shader", false);
		blm=FrameBuffer(fr->w_res,f->h_res,"shader/fbv_standard.shader","shader/fbf_standard.shader", false);
		blr=Blur(f);
	}
	void bloom() { fb.bind(); f->clear(0.0f,0.0f,0.0f); }
	void stop() { fb.close(); }
	void setup() { blr.blur();fb.render();blr.render_to(&blm); }
	void render()
	{
		sfb.enable();
		buffer.bind();
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D,fb.get_tex());
		sfb.upload_int("tex",0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,blm.get_tex());
		sfb.upload_int("bloom",1);

		glDrawArrays(GL_TRIANGLES,0,6);
		glActiveTexture(GL_TEXTURE0);
	}
private:
	Frame* f; Shader sfb;
	Blur blr; FrameBuffer fb,blm;
	Buffer buffer;
};
