#version 330 core

in vec4 Position;
in vec2 TexCoords;
in vec3 Normals;
in mat3 TBN;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

// object component maps
uniform sampler2D colour_map;
uniform sampler2D normal_map;
uniform sampler2D material_map;
uniform sampler2D emission_map;

uniform mat4 proj;

void main()
{
	// extract colour
	vec4 inv_depth = inverse(proj)*vec4(0,0,gl_FragCoord.z*2.0-1.0,1.0);
	float gdepth = (inv_depth/inv_depth.w).z;
	gbuffer_colour = vec4(texture(colour_map,TexCoords).rgb,gl_FragCoord.z);
	gbuffer_colour = vec4(gl_FragCoord.z,Position.w/10.0,0,gl_FragCoord.z);

	// translate position & shadow
	gbuffer_position = vec4(Position.rgb,0.0);

	// translate normals & emission
	vec3 normals = texture(normal_map,TexCoords).rgb*2-1;
	gbuffer_normals = vec4(normalize(TBN*normals),texture(emission_map,TexCoords).r);

	// extract surface materials
	gbuffer_materials = vec4(texture(material_map,TexCoords).rgb,0.0);
}