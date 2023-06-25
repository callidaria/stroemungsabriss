#version 330 core

in vec2 TexCoords;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;
out vec4 gbuffer_materials;

uniform sampler2D tex;

void main()
{
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_position = vec4(0);
	gbuffer_normals = vec4(0);
	gbuffer_materials = vec4(0);
}