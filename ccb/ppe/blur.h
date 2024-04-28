#ifndef CCB_POSTPROCESSING_BLUR
#define CCB_POSTPROCESSING_BLUR

#include "../frm/framebuffer.h"

class Blur
{
public:

	Blur()
	{
		b0 = FrameBuffer("./shader/post/blur_vertical.vs","./shader/post/blur.fs",false);
		b1 = FrameBuffer("./shader/post/blur_horizontal.vs","./shader/post/blur.fs",false);
	}
	// FIXME: blur fragments are extremely inefficient. they need to be optimized when used
	void blur() { b0.bind();Frame::clear(); }
	void stop() { b0.close(); }
	void render()
	{
		b1.bind();Frame::clear();b0.render();
		b1.close();Frame::clear();b1.render();
	}
	void render_to(FrameBuffer* fb)
	{
		b1.bind();Frame::clear(0.0f,0.0f,0.0f);b0.render();
		fb->bind();Frame::clear(0.0f,0.0f,0.0f);b1.render();
		fb->close();
	}

private:

	FrameBuffer b0,b1;
};
// TODO: has to be improved............

#endif