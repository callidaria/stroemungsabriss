#version 330 core

in vec4 Position;
in vec2 TexCoords;
in mat3 TBN;
in vec3 ltp;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

// object component maps
uniform sampler2D colour_map;
uniform sampler2D normal_map;
uniform sampler2D material_map;
uniform sampler2D shadow_map;

float calculate_shadow();

void main()
{
	// extract colour
	gbuffer_colour = vec4(texture(colour_map,TexCoords).rgb,0.0);

	// translate position & shadow
	gbuffer_position.rgb = Position.rgb;
	gbuffer_position.a = calculate_shadow();

	// translate normals & emission
	vec3 normals = texture(normal_map,TexCoords).rgb*2-1;
	gbuffer_normals = vec4(normalize(TBN*normals),0.0);

	// extract surface materials
	gbuffer_materials = vec4(texture(material_map,TexCoords).rgb,0.0);
}
// TODO: merge metal, roughness and ambient occlusion mapping into one texture to reduce load time

// dynamic shadow map generation
float calculate_shadow()
{
	// depth extractions
	float curr_depth = ltp.z;
	float pcf_depth = texture(shadow_map,ltp.xy).r;

	// project shadow
	vec2 raster = 1.0/textureSize(shadow_map,0);
	return float(curr_depth>pcf_depth+.0005);
}