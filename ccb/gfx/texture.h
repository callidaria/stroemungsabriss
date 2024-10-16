#ifndef CCB_GRAPHICS_TEXTURE
#define CCB_GRAPHICS_TEXTURE

#include "../mat/toolbox.h"


class Texture
{
public:

	// construction
	Texture(std::string path,bool corrected=false);
	~Texture() {  }

	// activation
	inline void bind() { glBindTexture(GL_TEXTURE_2D,texture); }
	static inline void unbind() { glBindTexture(GL_TEXTURE_2D,0); }

	// interaction
	void load();
	void upload();

	// filter settings
	static void set_texture_parameter_linear_mipmap();
	static void set_texture_parameter_linear_unfiltered();
	static void set_texture_parameter_nearest_unfiltered();
	static void set_cubemap_texture_parameters();
	static void set_cubemap_texture_parameters_mipmap();

	// pattern handling
	static void set_texture_parameter_clamp_to_edge();
	static void set_texture_parameter_clamp_to_border();
	static void set_texture_parameter_repeat();

	// filter features
	static inline void set_texture_filter_bias(float bias)
		{ glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,bias); }
	static void set_texture_border_colour(glm::vec4 colour);

public:

	// data
	uint32_t texture;

private:

	// image
	std::string m_Path;
	int32_t m_Width,m_Height,m_Format;
	void* m_Data;
};


struct Glyph
{
	// utility
	inline void bind() { glBindTexture(GL_TEXTURE_2D,texture); }
	void upload();

	// data
	void* buffer;
	uint32_t texture;
	glm::vec2 scale;
	glm::vec2 bearing;
	int64_t advance;
};


class FrameBuffer
{
public:

	FrameBuffer();
	FrameBuffer(uint8_t n_components);
	~FrameBuffer() {  }

	void add_colour_component(float width,float height,bool floatbuffer=false,uint8_t component=0);
	void add_depth_component(float width,float height);
	void combine_attachments();

	// upload
	inline void upload_colour_component(uint8_t i) { glBindTexture(GL_TEXTURE_2D,m_ColourComponents[i]); }
	inline void upload_depth_component() { glBindTexture(GL_TEXTURE_2D,m_DepthComponent); }

	// binding
	inline void bind() { glBindFramebuffer(GL_FRAMEBUFFER,m_Buffer); }
	static inline void unbind() { glBindFramebuffer(GL_FRAMEBUFFER,0); }

private:

	// data
	uint32_t m_Buffer;
	std::vector<uint32_t> m_ColourComponents;
	uint32_t m_DepthComponent;
};

#endif
