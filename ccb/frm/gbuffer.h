#ifndef CCB_FRAME_GBUFFER
#define CCB_FRAME_GBUFFER

#include "frame.h"
#include "../mat/toolbox.h"

class GBuffer
{
public:

	// construction
	GBuffer();
	GBuffer(float w_res,float h_res);
	~GBuffer() {  }

	// creation
	void add_colour_component(bool fcomp=false);
	uint8_t add_depth_component();
	void finalize_buffer();

	// bind
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,buffer); }

public:

	// attributes
	float w_res,h_res;

	// component textures
	std::vector<uint32_t> t_colour_components;
	std::vector<uint32_t> t_value_components;

private:

	// content
	uint32_t buffer;
	std::vector<uint32_t> def_colour_components;

	// attributes
	bool has_depth = false;
};

#endif
