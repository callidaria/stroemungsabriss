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

void main()
{
	// extract colour, specular & position
	gbuffer_colour = vec4(texture(tex,TexCoords).rgb,.0);
	gbuffer_position = vec4(Position.xyz,.0);

	// translate normals & emission
	gbuffer_normals.rgb = Normals;
	gbuffer_normals.a = 1.0;

	// extract surface materials
	gbuffer_materials = vec4(texture(material_map,TexCoords).rgb,.0);
}