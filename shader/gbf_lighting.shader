#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// light structure definitions
struct light_sun {
	vec3 position;
	vec3 colour;
	float intensity;
};

// g-buffer components
uniform sampler2D gbuffer_colour;
uniform sampler2D gbuffer_position;
uniform sampler2D gbuffer_normals;

// camera information
uniform vec3 view_pos;
uniform int spec_exponent = 16;
uniform float spec_intensity = 1.0;

// lights
uniform light_sun sunlight;

// light processing definitions
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl);

void main()
{
	// read g-buffer information
	vec4 colour_spec = texture(gbuffer_colour,TexCoords);
	vec3 colour = colour_spec.rgb;
	float speculars = colour_spec.a;
	vec3 position = texture(gbuffer_position,TexCoords).rgb;
	vec3 normals = texture(gbuffer_normals,TexCoords).rgb;

	// process orthogonal sunlight
	vec3 c_colours = vec3(0);
	c_colours = c_colours+lumen_sun(colour,position,normals,speculars,sunlight);

	// return colour composition
	outColour = vec4(c_colours*colour,1);
}

// sunlight processing
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 camera_dir = normalize(view_pos-position);

	// diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = diff*sl.colour;

	// specular
	vec3 spec_dir = reflect(-light_dir,normals);
	float spec = pow(max(dot(camera_dir,spec_dir),0),spec_exponent)*spec_intensity;
	vec3 specular = spec*in_speculars*sl.colour;

	// combine sunlight shading components
	return diffusion+specular;
}