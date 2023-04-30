#version 330 core

in vec3 TexCoords;

// out vec4 outColour;
out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

// colour source
uniform samplerCube irradiance_map;

void main()
{
	gbuffer_colour = vec4(texture(irradiance_map,TexCoords).rgb,1.0);
	gbuffer_position = vec4(0);
	gbuffer_normals = vec4(0,0,0,1);
	gbuffer_materials = vec4(0);
}