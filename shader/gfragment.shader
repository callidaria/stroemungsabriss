#version 330 core

in vec4 Position;
in vec2 TexCoords;
in vec3 Normals;

out vec4 gbuffer_colour;
out vec3 gbuffer_position;
out vec3 gbuffer_normals;

uniform sampler2D tex;
uniform sampler2D sm;
uniform sampler2D emit;
uniform sampler2D shadow_map;
uniform sampler2D nmap;

void main()
{
	// extract colour & specular
	gbuffer_colour = texture(tex,TexCoords);
	gbuffer_colour.a = texture(sm,TexCoords).r;

	// translate position
	gbuffer_position = Position.rgb;

	// translate normals
	gbuffer_normals = normalize(Normals);
}