#ifndef CCB_POSTPROCESSING_BLUR
#define CCB_POSTPROCESSING_BLUR

#include <iostream>
#include "../frm/frame.h"
#include "../frm/framebuffer.h"

class Blur
{
public:
	Blur() {  }
	Blur(Frame* fr)
		: f(fr)
	{
		b0 = FrameBuffer(f->w_res,f->h_res,"./shader/post/blur_vertical.vs","./shader/post/blur.fs", false);
		b1 = FrameBuffer(f->w_res,f->h_res,"./shader/post/blur_horizontal.vs","./shader/post/blur.fs", false);
	}
	// FIXME: blur fragments are extremely inefficient. they need to be optimized when used
	void blur() { b0.bind(); f->clear(0.0f,0.0f,0.0f); }
	void stop() { b0.close(); }
	void render()
	{
		b1.bind();f->clear(0.0f,0.0f,0.0f);b0.render();
		b1.close();f->clear(0.0f,0.0f,0.0f);b1.render();
	}
	void render_to(FrameBuffer* fb)
	{
		b1.bind();f->clear(0.0f,0.0f,0.0f);b0.render();
		fb->bind();f->clear(0.0f,0.0f,0.0f);b1.render();
		fb->close();
	}
private:
	Frame* f;
	FrameBuffer b0,b1;
};

#endif