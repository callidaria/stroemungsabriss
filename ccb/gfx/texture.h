#ifndef CCB_GRAPHICS_TEXTURE
#define CCB_GRAPHICS_TEXTURE

class Texture
{
public:

	// construction
	Texture();
	~Texture() {  }

	// filter settings
	static void set_texture_parameter_linear_mipmap();
	static void set_texture_parameter_linear_unfiltered();
	static void set_texture_parameter_nearest_unfiltered();
	static void set_cubemap_texture_parameters();
	static void set_cubemap_texture_parameters_mipmap();

	// pattern handling
	static void set_texture_parameter_clamp_to_edge();
	static void set_texture_parameter_clamp_to_border();
	static void set_texture_parameter_texture_repeat();
};

#endif
