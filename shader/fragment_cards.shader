#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normals;
in vec3 ltp;

//out vec4 outColour;
out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;

uniform sampler2D tex;
uniform sampler2D shadow_map;

struct light_sun {
	vec3 pos;
	vec3 col;
	float ins;
};
//uniform light_sun al[1];

float calculate_shadow();

void main()
{
	// process texture
	vec4 mix = texture(tex,TexCoords);

	// simulate lighting
	//vec3 light_dir = normalize(al[0].pos-Position);
	//float diffusion = max(abs(dot(Normals,light_dir)),.1);
	//vec3 mix_lmult = diffusion*al[0].col;

	// receive shadows
	float shadow = calculate_shadow();
	//mix_lmult *= (1-shadow);

	// return colour mix
	//outColour = vec4(mix_lmult*mix.rgb,mix.a);
	gbuffer_colour.rgb = mix.rgb;
	gbuffer_colour.a = 0;
	gbuffer_position.rgb = Position;
	gbuffer_position.a = shadow;
	gbuffer_normals.rgb = abs(Normals);
	gbuffer_normals.a = 0;
}

// dynamic shadow map generation
float calculate_shadow()
{
	// depth extractions
	float curr_depth = ltp.z;
	float pcf_depth = texture(shadow_map,ltp.xy).r;

	// project shadow
	vec2 raster = 1.0/textureSize(shadow_map,0);
	return float(curr_depth-.005>pcf_depth);
}
// TODO: code repetition