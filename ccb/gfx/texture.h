#ifndef CCB_GRAPHICS_TEXTURE
#define CCB_GRAPHICS_TEXTURE

#include "../mat/toolbox.h"

// information about image data
struct ImageData
{
	int32_t width,height,format;
	void* data;
};

// representing textures
class Texture
{
public:

	// construction
	Texture(const char* path,bool corrected=false);
	~Texture() {  }

	// activation
	inline void bind() { glBindTexture(GL_TEXTURE_2D,texture); }
	static inline void unbind() { glBindTexture(GL_TEXTURE_2D,0); }

	// interaction
	void gpu_upload();
	void cleanup();

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
	static inline void set_texture_filter_bias(float bias) { glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_LOD_BIAS,bias); }
	static inline void generate_mipmap() { glGenerateMipmap(GL_TEXTURE_2D); }

public:

	// data
	uint32_t texture;

private:

	// image
	ImageData image;
};

#endif
