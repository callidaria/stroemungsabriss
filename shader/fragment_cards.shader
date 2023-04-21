#version 330 core

in vec3 Position;
in vec2 TexCoords;
in vec3 Normals;
in vec3 ltp;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;

uniform sampler2D tex;
uniform sampler2D shadow_map;

float calculate_shadow();

void main()
{
	// process texture
	vec4 mix = texture(tex,TexCoords);

	// export colour mix & specular mapping
	gbuffer_colour.rgb = mix.rgb;
	gbuffer_colour.a = 0;

	// export position & shadow map
	gbuffer_position.rgb = Position;
	gbuffer_position.a = calculate_shadow();

	// export normals & light emission
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