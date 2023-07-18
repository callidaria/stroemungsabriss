#version 330 core

in vec4 Position;
in vec2 TexCoords;
in vec3 Normals;
in vec3 colcheck;

out vec4 gbuffer_colour;
out vec4 gbuffer_position;
out vec4 gbuffer_normals;

uniform sampler2D tex;

void main()
{
	// extract colour & specular
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_colour.rgb = colcheck;
	gbuffer_colour.a = .0;

	// translate position & shadow
	gbuffer_position.rgb = Position.xyz;
	gbuffer_position.a = .0;

	// translate normals & emission
	gbuffer_normals.rgb = Normals;
	gbuffer_normals.a = 1.0;
}