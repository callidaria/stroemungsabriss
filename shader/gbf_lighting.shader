#version 330 core

in vec2 TexCoords;

out vec4 outColour;

// sunlight structure definition
struct light_sun {
	vec3 position;
	vec3 colour;
	float intensity;
};

// pointlight structure definition
struct light_point {
	vec3 position;
	vec3 colour;
	float constant;
	float linear;
	float quadratic;
	float intensity;
};

// spotlight structure definition
struct light_spot {
	vec3 position;
	vec3 colour;
	vec3 direction;
	float cut_in;
	float cut_out;
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
uniform int sunlight_count = 0;
uniform int pointlight_count = 0;
uniform int spotlight_count = 0;
uniform light_sun sunlight[5];
uniform light_point pointlight[128];
uniform light_spot spotlight[32];

// light processing definitions
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl);
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl);
vec3 lumen_spot(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_spot sl);

// global
vec3 camera_dir;

void main()
{
	// read g-buffer information
	vec4 colour_spec = texture(gbuffer_colour,TexCoords);
	vec3 colour = colour_spec.rgb;
	float speculars = colour_spec.a;
	vec3 position = texture(gbuffer_position,TexCoords).rgb;
	vec3 normals = texture(gbuffer_normals,TexCoords).rgb;

	// precalculations
	camera_dir = normalize(view_pos-position);

	// process light sources
	vec3 lgt_colours = vec3(0);
	for (int i=0;i<sunlight_count;i++)
		lgt_colours += lumen_sun(colour,position,normals,speculars,sunlight[i]);
	for (int j=0;j<pointlight_count;j++)
		lgt_colours += lumen_point(colour,position,normals,speculars,pointlight[j]);
	for (int k=0;k<spotlight_count;k++)
		lgt_colours += lumen_spot(colour,position,normals,speculars,spotlight[k]);

	// return colour composition
	outColour = vec4(lgt_colours,1);
}
// FIXME: at higher resolutions there can be a raster-esque pattern when moving the camera
// FIXME: also ?MORE? texture flickering at ???HIGHER??? resolutions?!?

// specular processing
vec3 process_specular(vec3 colour,vec3 lgt_colour,float in_speculars,vec3 spec_dir,float fresnel)
{
	float spec = pow(max(dot(camera_dir,spec_dir),0),spec_exponent)*spec_intensity*pow(fresnel,.25);
	return spec*lgt_colour*in_speculars;
}

// sunlight processing
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*sl.colour;
	vec3 specular = process_specular(colour,sl.colour,in_speculars,spec_dir,fresnel);

	// combine sunlight shading components
	return mix(diffusion,vec3(diff*.4),in_speculars*pow(fresnel,4))+specular;
}

// pointlight processing
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl)
{
	// precalculations
	vec3 light_dir = normalize(pl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*pl.colour;
	vec3 specular = process_specular(colour,pl.colour,in_speculars,spec_dir,fresnel);

	// calculate pointlight influence
	float distance = length(pl.position-position);
	float attenuation = 1/(pl.constant+pl.linear*distance+pl.quadratic*(distance*distance));
	return (diffusion+specular)*attenuation;
}

// spotlight processing
vec3 lumen_spot(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_spot sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// specular & diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*sl.colour;
	vec3 specular = process_specular(colour,sl.colour,in_speculars,spec_dir,fresnel);

	// calculate spotlight influence
	float theta = dot(light_dir,normalize(-sl.direction));
	float epsilon = sl.cut_in-sl.cut_out;
	float intensity = clamp((theta-sl.cut_out)/epsilon,0,1);
	return (diffusion+specular)*intensity;
}