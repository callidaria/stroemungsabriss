#version 330 core

/*
in vec4 Position;
in vec2 TexCoords;
in mat3 TBN;
*/
in vec2 TexCoords;

/*
out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;
*/
out vec4 outColour;

// object component maps
/*
uniform sampler2D colour_map;
uniform sampler2D normal_map;
uniform sampler2D material_map;
uniform sampler2D emission_map;
*/
uniform sampler2D tex;

void main()
{
	outColour = vec4(TexCoords,0,1);
	return;

/*
	// extract colour & position
	gbuffer_colour = vec4(texture(colour_map,TexCoords).rgb,.0);
	gbuffer_position = vec4(Position.rgb,.0);

	// translate normals & emission
	vec3 normals = texture(normal_map,TexCoords).rgb*2.0-1.0;
	gbuffer_normals = vec4(normalize(TBN*normals),texture(emission_map,TexCoords).r);

	// extract surface materials
	gbuffer_materials = vec4(texture(material_map,TexCoords).rgb,.0);
*/
}