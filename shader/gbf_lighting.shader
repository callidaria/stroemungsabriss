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
uniform light_sun sunlight[5];
uniform light_point pointlight[128];

// light processing definitions
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl);
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl);

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

	// return colour composition
	outColour = vec4(lgt_colours,1);
}

// sunlight processing
vec3 lumen_sun(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_sun sl)
{
	// precalculations
	vec3 light_dir = normalize(sl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);
	float fresnel = max(1-dot(camera_dir,normals),.1);

	// diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*sl.colour;

	// specular
	float spec = pow(max(dot(camera_dir,spec_dir),0),spec_exponent)*spec_intensity*pow(fresnel,.25);
	vec3 specular = colour*spec*sl.colour*in_speculars;

	// combine sunlight shading components
	return mix(diffusion,vec3(diff*.4),in_speculars*pow(fresnel,4))+specular;
}

// pointlight processing
vec3 lumen_point(vec3 colour,vec3 position,vec3 normals,float in_speculars,light_point pl)
{
	// precalculations
	vec3 light_dir = normalize(pl.position-position);
	vec3 spec_dir = reflect(-light_dir,normals);

	// diffusion
	float diff = max(dot(light_dir,normals),0);
	vec3 diffusion = colour*diff*pl.colour;

	// specular
	float spec = pow(max(dot(camera_dir,spec_dir),0),spec_exponent)*spec_intensity;
	vec3 specular = colour*spec*pl.colour*in_speculars;

	// calculate point light influence
	float distance = length(pl.position-position);
	float attenuation = 1/(pl.constant+pl.linear*distance+pl.quadratic*(distance*distance));
	return (diffusion+specular)*attenuation;
}