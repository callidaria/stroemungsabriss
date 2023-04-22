#version 330 core

in vec4 Position;
in vec2 TexCoords;
in mat3 TBN;
in vec3 ltp;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;

// object material textures
uniform sampler2D tex;
uniform sampler2D sm;
uniform sampler2D emit;
uniform sampler2D shadow_map;
uniform sampler2D nmap;

float calculate_shadow();

void main()
{
	// extract colour & specular
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_colour.a = texture(sm,TexCoords).r;

	// translate position & shadow
	gbuffer_position.rgb = Position.rgb;
	gbuffer_position.a = calculate_shadow();

	// translate normals & emission
	vec3 normals = texture(nmap,TexCoords).rgb*2-1;
	gbuffer_normals.rgb = normalize(TBN*normals);
	gbuffer_normals.a = texture(emit,TexCoords).r;
}
// FIXME: specular & emission map only use one colour channel, merge them?

// dynamic shadow map generation
float calculate_shadow()
{
	// depth extractions
	float curr_depth = ltp.z;
	float pcf_depth = texture(shadow_map,ltp.xy).r;

	// project shadow
	vec2 raster = 1.0/textureSize(shadow_map,0);
	return float(curr_depth>pcf_depth);
}