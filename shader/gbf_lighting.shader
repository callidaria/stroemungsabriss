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

// lights
uniform light_sun sunlight;

void main()
{
	// read g-buffer information
	vec4 colour_spec = texture(gbuffer_colour,TexCoords);
	vec3 colour = colour_spec.rgb;
	float speculars = colour_spec.a;
	vec3 position = texture(gbuffer_position,TexCoords).rgb;
	vec3 normals = texture(gbuffer_normals,TexCoords).rgb;

	// process orthogonal sunlight
	vec3 light_dir = normalize(sunlight.position-position);
	float diff = max(dot(light_dir,normals),0);
	vec3 vdiff = diff*sunlight.colour;

	// return colour composition
	outColour = vec4(vdiff*colour,1);
}