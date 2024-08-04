#ifndef CCB_POSTPROCESSING_BLOOM
#define CCB_POSTPROCESSING_BLOOM

#include "../fcn/buffer.h"
#include "../gfx/shader.h"
#include "../frm/framebuffer.h"
#include "../ppe/blur.h"


class Bloom
{
public:

	Bloom()
	{
		float verts[] = {
			-1.0f,1.0f,0.0f,1.0f,
			-1.0f,-1.0f,0.0f,0.0f,
			1.0f,-1.0f,1.0f,0.0f,
			-1.0f,1.0f,0.0f,1.0f,
			1.0f,-1.0f,1.0f,0.0f,
			1.0f,1.0f,1.0f,1.0f
		};
		// TODO: replace with toolbox function
		buffer = Buffer();
		buffer.bind();
		buffer.upload_vertices(verts,sizeof(verts));
		sfb.compile2d("./shader/standard/framebuffer.vs","./shader/post/bloom.fs");

		// FIXME: shookz there's no greyscale shader!
		fb = FrameBuffer("./shader/standard/framebuffer.vs","./shader/post/greyscale.fs", false);
		blm = FrameBuffer("./shader/standard/framebuffer.vs","./shader/standard/direct.fs", false);
		blr = Blur();
	}
	void bloom() { fb.bind();Frame::clear(); }
	void stop() { fb.close(); }
	void setup() { blr.blur();fb.render();blr.render_to(&blm); }
	void render()
	{
		sfb.enable();
		buffer.bind();

		glBindTexture(GL_TEXTURE_2D,fb.tex);
		sfb.upload_int("tex",0);

		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D,blm.tex);
		sfb.upload_int("bloom",1);

		glDrawArrays(GL_TRIANGLES,0,6);
		glActiveTexture(GL_TEXTURE0);
	}

private:

	Buffer buffer;
	Shader sfb;
	FrameBuffer fb,blm;
	Blur blr;
};
// TODO: implementation in header is so 2014

#endif